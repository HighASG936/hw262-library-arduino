#ifndef HW262_H
#define HW262_H

#include <Arduino.h>
#include "buzzer.h"
#include "led.h"
#include "pot.hpp"
#include "switches.h"
#include "display4Digits.h"
#include "lm35.h"
#include "ds18.h"
#include "bluetooth.h"
#include "constants.h"

#define DEFAULT_PRECISION 1
#define LM35_SENSOR  lm35

class Hw262 :   public Led, 
                public Buzzer, 
                public Pot, 
                public display4Digits, 
                public Bluetooth,
                public Switches
{
    protected:
        uint8_t sensorPrecision;
        static uint16_t readA5();
        static uint16_t readVoltageA5();                       

    public:
        void begin(uint8_t sensorPrecision=DEFAULT_PRECISION);
        static Lm35 tempSensor;
        static void resetAllLeds();
};

extern Hw262 HW262;

#endif

