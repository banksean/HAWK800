#include <MIDI.h>
#include "Controller.h"
#include "CCMessages.h"

/*
MIDI controller for the Korg EX-800 with the HAWK-800 mod installed.

The HAWK-800 adds a bunch of new MIDI CC messages to control various
parameters in realtime, but they way it implements these messages
makes it difficult to use a typical generic MIDI controller's CC support.

In particular, it takes multiple CC messages for HAWK-800 to change
a single parameter. There are three different methods that it will
accept, but the simplest for our purposes is one that sends two
CC messages per paraemter. The first CC message specifies the parameter
you want to change, and the second CC message sets the value for that
parameter.

The purpose of this project is to provide a hardware interface that
sends CC messages using this HAWK-800 specific protocol.

Connections:

It works sort of like a hardware MIDI proxy. Instead of plugging your
controller keyboard's MIDI OUT directly into the EX-800's MIDI IN, 
plug this box in between the two.

Keyboard MIDI OUT -> MIDI IN on this box, MIDI OUT -> MIDI IN on EX-800

Then you can play the keys on a proper keybard controller, and use this
arduino box to wiggle the HAWK-800 parameters at the same time.

*/

MIDI_CREATE_DEFAULT_INSTANCE();

//************************************************************
//***SET THE NUMBER OF CONTROLS USED**************************
//************************************************************
//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 0;

//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 6;

byte MIDI_CH = 1;

// lastCtrl keeps track of the last parameter we sent. This is
// a slight optimization, so that in cases where you start
// sweeping a parameter it only has to send the parameter select
// CC for the first one, then just keep sending values after that,
// until the user starts changing a different parameter.
//
// CC 83 explicitly not used as of the HAWK800 version I have
// installed.
byte lastCtrl = 83;

//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)
//**Command parameter is for future use**

Pot PO1(A0, 0, VCF_CUT_OFF_SET_POINT, MIDI_CH);
Pot PO2(A1, 0, DCO2_INTERVAL, MIDI_CH);
Pot PO3(A2, 0, NOISE_LEVEL, MIDI_CH);
Pot PO4(A3, 0, DCO1_HARMONIC_SELECTOR, MIDI_CH);
Pot PO5(A4, 0, DCO2_HARMONIC_SELECTOR, MIDI_CH);
Pot PO6(A5, 0, EG1_RELEASE, MIDI_CH);

//*******************************************************************
//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

// Just a test note.
Button BU1(2, 0, 60, MIDI_CH, 5 );

//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {&BU1};


void handleControlChange(byte channel, byte number, byte value) {
  if (channel != MIDI_CH) {
    return;
  }
  // Reset the last control, since we don't know what the other
  // MIDI device sent or why. Just assume we need to send the 
  // parameter select CC again next time.
  //lastCtrl = 83;
  MIDI.sendControlChange(number, value, channel);
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (channel != MIDI_CH) {
    return;
  }
    MIDI.sendNoteOn(pitch, velocity, channel);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    if (channel != MIDI_CH) {
    return;
  }

    MIDI.sendNoteOff(pitch, velocity, channel);
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CH);
  MIDI.turnThruOff();
}

void loop() {
  MIDI.read();
  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
}


//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          digitalWrite(LED_BUILTIN, HIGH);
          MIDI.sendNoteOn(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          digitalWrite(LED_BUILTIN, LOW);
          MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}

//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();
    if (potmessage != 255) {
      // This logic is specific to the HAWK-800 mod:
      /*
       * 1) Send MIDI CC 84 with the data value set to select the desired target parameter where 00-63 matches parameters in the P1 bank (P1 11-88) and 64-127 matches the parameters in the P2 bank (P2 11-88).
       * 2) Send MIDI CC 85 with the data value set to the desired value for the target parameter (but ranged to fit within 0-127 or an absolute value depending upon global parameter 47).
       */
      byte ctrl = POTS[i]->Pcontrol;
      // Only send a parameter select if it changed since the last time we sent
      // a CC.
      if (ctrl != lastCtrl) {
        MIDI.sendControlChange(84, ctrl, POTS[i]->Pchannel);
        // In practice, the HAWK800 will crash if you send too many of these messages too quickly.
        delay(10);
      }
      lastCtrl = ctrl;
   
      MIDI.sendControlChange(85, potmessage, POTS[i]->Pchannel);
      // MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
    }
  }
}
