
#include "Encoder.h"

Encoder::Encoder(byte pinA, byte pinB, byte pinPush, int min, int max)
{
  _pinA = pinA;
  _pinB = pinB;
  _pinPush = pinPush;
  _store = 0;
  _min = min;
  _max = max;
  _val = 0;
  _mode = 0;
  _lastBtnState = 0;
  pinMode(_pinA, INPUT);
  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT);
  pinMode(_pinB, INPUT_PULLUP);
  pinMode(_pinPush, INPUT);
  pinMode(_pinPush, INPUT_PULLUP);
}


int8_t Encoder::updateRotary() 
{
  // A vald CW or  CCW move returns 1, invalid returns 0.
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  _prevNextCode <<= 2;
  if (digitalRead(_pinA)) _prevNextCode |= 0x02;
  if (digitalRead(_pinB)) _prevNextCode |= 0x01;
  _prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[_prevNextCode] ) {
      _store <<= 4;
      _store |= _prevNextCode;
      if ((_store&0xff)==0x2b) _inc = -1;
      if ((_store&0xff)==0x17) _inc = 1;
   } else {
    _inc = 0;
   }
   return _inc;
}

void Encoder::setValue(int val, int min, int max) {
  _val = val;
  _min = min;
  _max = max;
  _store = 0;
  _prevNextCode = 0;
  _inc = 0;
}

int Encoder::getValue() 
{
  if (_val + _inc <= _max && _val + _inc >= _min) {
    _val += _inc;
    _inc = 0;
  }
  return _val;
}

byte Encoder::getMode() { return _mode; }
void Encoder::setMode(byte mode) { _mode = mode; }

byte Encoder::checkButtonUp()
{
  int btnState = getButtonPressed();
  //bool change = btnState != _lastBtnState;

  // Went from LOW to HIGH, meaning pressed to not-pressed.
  if (btnState != _lastBtnState && btnState == 1) {
    _lastBtnState = btnState;
    _mode = _mode == 0 ? 1 : 0;
    return 1;
  }
  
  _lastBtnState = btnState;
  return 0;
}

byte Encoder::getButtonPressed()
{
  // Read the button state
  int btnState = digitalRead(_pinPush);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - _lastButtonPress > 50) {
      return 1;
    }

    // Remember last button press event
    _lastButtonPress = millis();
  }

  return 0;
}
