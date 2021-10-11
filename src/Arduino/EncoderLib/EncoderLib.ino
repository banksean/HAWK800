#include "Encoder.h"
#include "CCMessages.h"

#include <MIDI.h>
//#include <SoftwareSerial.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <U8g2lib.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_SH110X.h>

// Random note: https://www.midimonitor.com/ is super helpful in debugging some of this stuff.

#define MIDI_CH 1

// Female midi DIN pins, left to right: 1 4 2 5 3
// Male midi DIN pins, left to right: 3 5 2 4 1

// MIDI TRS: pin 2 == SLV, 5 == ring, 4 == tip

// Thonkicon stereo TRS (top and middle are the close ones): top pin slv (midi pin 2), middle pin ring (midi pin 5), bottom pin tip (midi pin 4)

MIDI_CREATE_DEFAULT_INSTANCE();

//#define SCREEN_WIDTH 132 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, A5, A4);

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8G2_R0, A5, A4);

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

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CH);
  MIDI.turnThruOff();

  for (int i = 0; i < NUM_PARAMS; i++) {
          Param param;
          memcpy_P(&param, &(ALL_PARAMS[i]), sizeof(Param));
    PARAM_VALUES[i] = param.min;
  }
  u8x8.begin();
  u8x8.setFont(u8x8_font_5x7_f);
  u8x8.drawString(0, 0, "ok");
  
  /*
  u8g2.initDisplay();
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g_font_5x8);
  u8g2.setCursor(0, 8);
  u8g2.print("ok");
  u8g2.setCursor(0, 16);
  u8g2.print("ok");
  u8g2.setCursor(0, 24);
  u8g2.print("ok");
  u8g2.sendBuffer();
  */
/*
 if(!display.begin(0x3C, true)) { //SH110X_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  //display.cp437(true); // Use full 256 char 'Code Page 437' font
  display.setCursor(0, 10);
  display.print(F("ok"));
  display.display();
  (
  */
}

int lastCC = 83;

void handleControlChange(byte channel, byte number, byte value) {
  if (channel != MIDI_CH) {
    return;
  }
  // Reset the last control, since we don't know what the other
  // MIDI device sent or why. Just assume we need to send the 
  // parameter select CC again next time.
  lastCC = 83;
  
  MIDI.sendControlChange(number, value, channel);
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (channel != MIDI_CH) {
    return;
  }

  MIDI.sendNoteOn(pitch, velocity, channel);
  //display.setCursor(0, 40);
  // Display static text
  //display.fillRect(0, 40, SCREEN_WIDTH, 10, SH110X_BLACK);
  //display.println("Note ON");
  u8x8.clearLine(5);
  u8x8.drawString(0, 5, "Note ON");
  update = true;
  //display.display(); 
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (channel != MIDI_CH) {
    return;
  }
  
  MIDI.sendNoteOff(pitch, velocity, channel);
  MIDI.sendNoteOn(pitch, velocity, channel);
  //display.setCursor(0, 40);
  // Display static text
  //display.fillRect(0, 40, SCREEN_WIDTH, 10, SH110X_BLACK);
  //display.println("Note OFF");
  u8x8.clearLine(5);
  u8x8.drawString(0, 5, "Note OFF");

  update = true;
}


void loop() {
  MIDI.read();
  update = false;
  static char buff[16];
  bool updateRow[NUM_ENCODERS];
  
  for (int i = 0; i < NUM_ENCODERS; i = i + 1) {

    // getButtonPressed is really "button down" state. So it's true as long as the encoder is
    // held down.
    updateRow[i] = false;
    if (ENCODERS[i]->checkButtonUp() == 1) {
        updateRow[i] = true;
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
      updateRow[i] = true;
      int v = ENCODERS[i]->getValue();
      if (ENCODERS[i]->getMode() == 1) {
        SELECTED_PARAM[i] = v;
      } else {
        PARAM_VALUES[SELECTED_PARAM[i]] = v;
        // TODO: Speed this up. Too much copying just to get the cc msg.
        Param param;
        memcpy_P(&param, &(ALL_PARAMS[SELECTED_PARAM[i]]), sizeof(Param));
        if (lastCC != param.cc) {
          MIDI.sendControlChange(84, param.cc, MIDI_CH);
          lastCC = param.cc;
        }
        // scale v from [param.min, param.max] to [0, 127]
        // *EXCEPT* for VCF cutoff. No idea why, but this one doesn't like to be scaled. Just send
        // the raw value.
        int val = 127 * (v - (int)param.min) / (int)(param.max - param.min);
        if (param.cc == VCF_CUT_OFF_SET_POINT.cc) {
          val = v;
        }
        MIDI.sendControlChange(85, val, MIDI_CH);
      }
    } 
    update = update || updateRow[i];
  }

  if (update) {
    //u8g2.firstPage();
  //u8g2.nextPage();
    //do {
  for (int i = 0; i < NUM_ENCODERS; ++i) {
        // Update the display.
    if (updateRow[i]) {
      u8x8.clearLine(i);
      //u8g2.clearBuffer();
          int v = ENCODERS[i]->getValue();
          int param = SELECTED_PARAM[i];
          char * ptr = (char *) pgm_read_word (&PARAM_NAMES [param]);
          // TODO: just copy this straight into buff, instead of intermediate paramName?
          char paramName [16]; // must be large enough!
          strcpy_P (paramName, ptr);
  
          // Render string based on encoder's mode.
          if (ENCODERS[i]->getMode() == 1) {
            u8x8.setInverseFont(1);
            sprintf(buff, "%s: %d", paramName, PARAM_VALUES[v]);
          } else {
            u8x8.setInverseFont(0);
            sprintf(buff, "%s: %d", paramName, v);
          }
          u8x8.drawString(0, i, buff);
          //u8g2.setDrawColor(0);
          //u8g2.drawBox(0, 8*i, 128, 8);
          //u8g2.setDrawColor(1);
          //u8g2.setCursor(0, 8*(i+1));
          //u8g2.print(buff);

          //u8g2.updateDisplay();
          //u8g2.updateDisplayArea(0, i, 128/8, 1);
          //u8g2.drawStr(0, 8 * (i + 1), buff);
    }
    } 
    //} while (u8g2.nextPage());
  }
}
