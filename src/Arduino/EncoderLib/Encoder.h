#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>

//***********************************************************************
class Encoder 
{
  public:
    Encoder(byte pinA, byte pinB, byte pinPush, int min, int max);
    int8_t updateRotary();
    int getValue();
    byte getMode();
    byte getButtonPressed();
    byte checkButtonUp();
    void setValue(int val, int min, int max);
    void setMode(byte mode);
    
  private:
    byte _pinA;
    byte _pinB;
    byte _pinPush;
    uint8_t _prevNextCode;
    uint16_t _store;
    int _min;
    int _max;
    int _val;
    byte _mode;
    byte _lastBtnState;

    int8_t _inc;
    unsigned long _lastButtonPress;
};
#endif
