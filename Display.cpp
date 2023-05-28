#include "Display.h"
/*------------------------------------------------------------------------*/

VariableScreen::VariableScreen(uint8_t _screen , uint8_t _screenMax , int _lcdInterval) {
  screen = _screen;
  screenMax = _screenMax;
  lcdInterval = _lcdInterval;
}

long _previousLCDMillis = 0;
long _currentLCDMillis = 0;

int VariableScreen::Change() {
  return _screen;
}

void VariableScreen::refreshs(int _menu , void Changingloop()) {
  _currentLCDMillis = millis();
  if ( _menu ==  1) {
    if (_currentLCDMillis - _previousLCDMillis > lcdInterval) {
      _previousLCDMillis = _currentLCDMillis;
      if (screen > screenMax) {
        screen = 0;
        _screen = 0;
      }
      Changingloop();
      screen++;
      _screen = screen;
    }
  }
}

/*------------------------------------------------------------------------*/

Encoder_internal_state_t * Encoder::interruptArgs[];
/*------------------------------------------------------------------------*/
Encoder::Encoder(uint8_t _pin1, uint8_t _pin2,  uint8_t _Button,  uint8_t _mPin) {
  mPin = _mPin;
  pin1 = _pin1;
  pin2 = _pin2;
  Button = _Button;

#ifdef INPUT_PULLUP
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
#endif
  encoder.pin1_register = PIN_TO_BASEREG(pin1);
  encoder.pin1_bitmask = PIN_TO_BITMASK(pin1);
  encoder.pin2_register = PIN_TO_BASEREG(pin2);
  encoder.pin2_bitmask = PIN_TO_BITMASK(pin2);

  encoder.position = 0;
  delayMicroseconds(2000);

  uint8_t s = 0;
  if (DIRECT_PIN_READ(encoder.pin1_register, encoder.pin1_bitmask)) s |= 1;
  if (DIRECT_PIN_READ(encoder.pin2_register, encoder.pin2_bitmask)) s |= 2;
  encoder.state = s;
}

/*-------------------------------------------------------------------------------*/

void Encoder::beepBuzzer(unsigned int frequency, unsigned int const onDuration, byte const beeps) {
  mFreq = frequency;
  mOnDuration = onDuration;
  mBeeps = beeps;
  mStartTime = max(millis(), 1);
  mLastRunTime = 0;
  updateBuzzer();
}

void Encoder::updateBuzzer() {
  unsigned long currentTime = millis();

  unsigned long elapsedTime = currentTime - mStartTime;
  unsigned int blinkDuration = mOnDuration + 100;
  unsigned int sequenceDuration = blinkDuration * mBeeps ;

  if (!sequenceDuration || (1 != 0 && elapsedTime / sequenceDuration >= 1))
  {
    mStartTime = 0;
    return;
  };

  unsigned int blinkingDuration = blinkDuration * mBeeps;
  unsigned int timeInSequence = elapsedTime % sequenceDuration;

  if (timeInSequence < blinkingDuration && timeInSequence % blinkDuration < mOnDuration)
  {
    tone(mPin, mFreq);
  }
  else
  {
    noTone(mPin);
  };
}

/*------------------------------------------------------------------------*/

int32_t Encoder::read() {
  update(&encoder);
  int value =  encoder.position;
  return value / 2;
}

int32_t Encoder::readAndReset() {
  update(&encoder);
  int32_t ret = encoder.position;
  encoder.position = 0;
  return ret;
}

void Encoder::write(int32_t p) {
  encoder.position = p;
}

/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/

bool _LastconfigStatus;
bool Encoder::configMode() {
  if (digitalRead(Button) == HIGH) {
    if (_configStatus == false) {
      encoder.position = 0;
      _configStatus = true;
    }
    else {
      encoder.position = 0;
      _configStatus = false;
    }
  }
  return _configStatus;
}

bool Encoder::configStatus() {

  return _configStatus;
}
int newvalues ;
int oldPosition  = 0;
void Encoder::updateDisplay(void Display() , int value) {
  update(&encoder);
  int newPosition =  encoder.position / 2 ;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Display();
    if (value != 1) {
      Encoder::beepBuzzer(3000, 100, 1);
    }
    delayMicroseconds(2000);
  }
}
//----------------------------------------------------------------------
float Encoder::ConfigsValue(float newConfigsValue , int maximum , int minimum ,  float sensevity ) {
  if (_configStatus == true) {
    newConfigsValue =  ((encoder.position / 2 ) * sensevity ) + newConfigsValue;
    newConfigsValue = constrain(newConfigsValue, minimum , maximum );
    encoder.position = 0 ;
  }
  return  newConfigsValue;
}
void Encoder::executeAction(void updatas(), void executeActions() , int value)  {
  ButtonState = digitalRead(Button);
  if (LastButtonState == false && ButtonState == true) {
    if (value != 1) {
      Encoder::beepBuzzer(3000, 1000, 1);
    }
    executeActions();
    updatas();
  }
  LastButtonState = ButtonState;
  delayMicroseconds(2000);
}

/*------------------------------------------------------------------------*/

int Encoder::rotateMenu(int Menu , int maximum , int minimum) {
  if (_configStatus == false) {
    Menu =  (encoder.position / 2 ) + Menu;
    Menu = constrain(Menu, minimum , maximum );
    encoder.position = 0 ;
  }
  return  Menu;
}
