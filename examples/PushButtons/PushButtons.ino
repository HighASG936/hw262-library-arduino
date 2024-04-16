
#include <hw262.h> //Add HW262 library

void setup()
{
    Serial.begin(115200);
    HW262.begin();
    HW262.clearDisplay(); // Clear display    
}


void loop()
{
    if(HW262.IsPressed(s1)) HW262.ledOn();
    if(HW262.IsPressed(s2)) HW262.ledOff();
    if(HW262.IsPressed(s3))
    {
        HW262.ledToggle();
        delay(300);
    }
}
