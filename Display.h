#ifndef Display_h_
#define Display_h_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#include "utility/direct_pin_read.h"
#define ENCODER_ARGLIST_SIZE 0
#include <LiquidCrystal_I2C.h>

typedef struct {
  volatile IO_REG_TYPE * pin1_register;
  volatile IO_REG_TYPE * pin2_register;
  IO_REG_TYPE            pin1_bitmask;
  IO_REG_TYPE            pin2_bitmask;
  uint8_t                state;
  int32_t                position;
} Encoder_internal_state_t;

class VariableScreen {
  public:
    VariableScreen:: VariableScreen(uint8_t screen , uint8_t screenMax , int lcdInterval);
    int Change();
    int _screen;
    void refreshs(int _menu , void Changingloop()) ;
  private:
    int8_t screen;
    uint8_t screenMax;
    int lcdInterval;

};

/*------------------------------------------------------------------------*/
class Encoder
{
  public:
    Encoder(uint8_t pin1, uint8_t pin2,  uint8_t Button,  uint8_t mPin);
    int32_t read();
    int32_t readAndReset();
    void write(int32_t p) ;
    bool configMode();
    bool LastButtonState = false;
    bool ButtonState = false;
    bool _configStatus = false;
    float ConfigsValue(float newConfigsValue , int maximum , int minimum , float sensevity );

    void updateDisplay(void Display() , int value);
    void executeAction(void updatas(), void executeActions() , int value);
    int  rotateMenu(int Menu , int maximum , int minimum) ;
    int Screensreturn();
    bool configStatus();
    /*-------------------------------------------------------------------*/
    void beepBuzzer(unsigned int frequency, unsigned int const onDuration, byte const beeps) ;
    void updateBuzzer();
    /*-------------------------------------------------------------------*/
    int returnvalues();
    /*------------------------------------------------------------------------*/

  private:
    Encoder_internal_state_t encoder;
    uint8_t pin1;
    uint8_t pin2;
    uint8_t Button;
    /*------------------------------------------------------------------------*/

    /*-------------------------------------------------------------------*/
    bool isArduino = true;
    uint8_t mPin;
    unsigned int mFreq;
    byte mBeeps;
    unsigned int mOnDuration = 100;
    unsigned long mStartTime;
    unsigned long mLastRunTime;
    /*-------------------------------------------------------------------*/

  public:
    static Encoder_internal_state_t * interruptArgs[ENCODER_ARGLIST_SIZE];

  public:
    static void update(Encoder_internal_state_t *arg) {
      uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
      uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register, arg->pin2_bitmask);
      uint8_t state = arg->state & 3;
      if (p1val) state |= 4;
      if (p2val) state |= 8;
      arg->state = (state >> 2);
      switch (state) {
        case 1: case 7: case 8: case 14:
          arg->position++;
          return;
        case 2: case 4: case 11: case 13:
          arg->position--;
          return;
        case 3: case 12:
          arg->position += 2;
          return;
        case 6: case 9:
          arg->position -= 2;
          return;
      }
    }
  private:
};


#endif
