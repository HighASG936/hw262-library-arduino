
#include "hw262.h"


#if currentTempSensor == LM35
    Lm35 Hw262::tempSensor(DEFAULT_PRECISION);
#endif

void Hw262::begin(String nameCurrentTempSensor)
{    
    if(nameCurrentTempSensor != "") this->nameCurrentTempSensor = nameCurrentTempSensor;
}

uint16_t Hw262::readA5()
{
    return analogRead(A5);
}

uint16_t Hw262::readVoltageA5()
{
    return  (SOURCE_VOLTAGE/MAX_VALUE_ADC)*readA5();
}

void Hw262::resetAllLeds()
{
  for(uint8_t i=0; i<sizeof(Pinesled);i++)
  {
    ledOff(Pinesled[i]);
  }
}
