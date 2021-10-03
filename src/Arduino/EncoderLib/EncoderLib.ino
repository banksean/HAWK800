#include "Encoder.h"
#include "CCMessages.h"

#include <MIDI.h>
//#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Random note: https://www.midimonitor.com/ is super helpful in debugging some of this stuff.

#define MIDI_CH 1

// Female midi DIN pins, left to right: 1 4 2 5 3
// Male midi DIN pins, left to right: 3 5 2 4 1

// MIDI TRS: pin 2 == SLV, 5 == ring, 4 == tip

// Thonkicon stereo TRS (top and middle are the close ones): top pin slv (midi pin 2), middle pin ring (midi pin 5), bottom pin tip (midi pin 4)

MIDI_CREATE_DEFAULT_INSTANCE();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool update = false;

// Rotary Encoder Inputs
#define NUM_ENCODERS 2
Encoder ENC01(8, 9, 10, 1, 3);
Encoder ENC02(2, 3, 4, 1, 2);
//Encoder ENC03(5, 6, 7, 0, 127);
Encoder *ENCODERS[] {&ENC01, &ENC02}; //, &ENC03};

int SELECTED_PARAM[NUM_ENCODERS] = {0, 1};
byte PARAM_VALUES[NUM_PARAMS];

/*
 * Generic parameter encoder behavior:
 * Mode 0: Turn selects parameter
 * Mode 1: Turn sets parameter value
 * Click toggles mode between 0 and 1
 * We have an array of parameter metadata.
 * Encoders maintain state of current values.
 * Iterate through all of the metadata values
 *    create an encoder for each one
 *    set their pins to don't care
 * Make a special "Parameter Select" encoder instance.
 */

void setup() {
  //Serial.begin (115200);
  //Serial.println(F("start"));
  //ENC02.setMode(1);

  pinMode(LED_BUILTIN, OUTPUT);

  //MIDI.setHandleNoteOn(handleNoteOn);
  //MIDI.setHandleNoteOff(handleNoteOff);
  //MIDI.setHandleControlChange(handleControlChange);
  //MIDI.begin(MIDI_CH);
  //MIDI.turnThruOff();

  for (int i = 0; i < NUM_PARAMS; i++) {
          Param param;
          memcpy_P(&param, &(ALL_PARAMS[i]), sizeof(Param));
    PARAM_VALUES[i] = param.min;
  }

 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true); // Use full 256 char 'Code Page 437' font
  display.setCursor(0, 10);
  display.print(F("ok"));
  display.display();
}


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

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel != MIDI_CH) {
    return;
  }

  MIDI.sendNoteOn(pitch, velocity, channel);
  display.setCursor(0, 40);
  // Display static text
  display.fillRect(0, 40, SCREEN_WIDTH, 10, BLACK);
  display.println("Note ON");
  update = true;
  //display.display(); 
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel != MIDI_CH) {
    return;
  }
  
  MIDI.sendNoteOff(pitch, velocity, channel);
  MIDI.sendNoteOn(pitch, velocity, channel);
  display.setCursor(0, 40);
  // Display static text
  display.fillRect(0, 40, SCREEN_WIDTH, 10, BLACK);
  display.println("Note OFF");
  update = true;
}


void loop() {
  update = false;
  static char buff[16];
  for (int i = 0; i < NUM_ENCODERS; i = i + 1) {

    // getButtonPressed is really "button down" state. So it's true as long as the encoder is
    // held down.
    if (ENCODERS[i]->checkButtonUp() == 1) {
        update = true;
        if (ENCODERS[i]->getMode() == 1) {
          // just entered into param select mode
          // don't really care what getValue() returns at this point.
          // Set the encoder's value to the currently selected parameter index.
          ENCODERS[i]->setValue(SELECTED_PARAM[i], 0, NUM_PARAMS-1);
        } else {
          // just entered into param set mode
          // getValue() should return 0-127, the just-selected index into the parameter list
          int newParamIndex = ENCODERS[i]->getValue();
          SELECTED_PARAM[i] = newParamIndex;
          Param param;
          memcpy_P(&param, &(ALL_PARAMS[newParamIndex]), sizeof(Param));
          //Param * param = pgm_read_ptr(&ALL_PARAMS [newParamIndex]);
          ENCODERS[i]->setValue(PARAM_VALUES[newParamIndex], param.min, param.max);
        }
    }

    if( ENCODERS[i]->updateRotary() ) {
      update = true;
      int v = ENCODERS[i]->getValue();
      if (ENCODERS[i]->getMode() == 1) {
        SELECTED_PARAM[i] = v;
      } else {
        PARAM_VALUES[SELECTED_PARAM[i]] = v;
        // TODO: Speed this up. Too much copying just to get the cc msg.
        //Param param;
        //memcpy_P(&param, &(ALL_PARAMS[SELECTED_PARAM[i]]), sizeof(Param));
        //MIDI.sendControlChange(84, param.cc, MIDI_CH);
        //MIDI.sendControlChange(85, v, MIDI_CH);
      }
    }

    // Update the display.
    if (update) {
        int v = ENCODERS[i]->getValue();
      display.setCursor(0, 10 * (i + 1));
      display.fillRect(0, 10 * (i + 1), SCREEN_WIDTH, 10, BLACK);
      int param = SELECTED_PARAM[i];
      char * ptr = (char *) pgm_read_word (&PARAM_NAMES [param]);
      // TODO: just copy this straight into buff, instead of intermediate paramName?
      char paramName [16]; // must be large enough!
      strcpy_P (paramName, ptr);

      // Render string based on encoder's mode.
      if (ENCODERS[i]->getMode() == 1) {
        sprintf(buff, "%d: [%s]", v, paramName);
      } else {
        sprintf(buff, "%s: %d", paramName, v);
      }
      display.println(buff);
    }
    
  }
  if (update) {
    display.display(); 
  }
}
