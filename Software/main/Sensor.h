#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

uint8_t Button = 13;
uint8_t Led_Red = 12;
uint8_t Led_Green = 11;
uint8_t Led_Blue = 10;

const uint8_t LED_STATE_RED = 0;
const uint8_t LED_STATE_GREEN = 1;


void CustomerPing(uint8_t State)
{
    uint8_t Press; // Temporary variable
    
    Press = digitalRead(Button); // The current signal at the sensor is read out
  
    if (Press == HIGH) // Active Low, so that means when button is pressed, the logic value will be low, as by default it is high due to the pull-up resistor.
    {
        digitalWrite(Led_Red, HIGH); // LED is RED
        digitalWrite(Led_Green, LOW); 
        digitalWrite(Led_Blue, LOW); 
        State = LED_STATE_RED;
        return State;
    }
    else
    {
        digitalWrite(Led_Red, LOW); // LED is GREEN
        digitalWrite(Led_Green, HIGH); 
        digitalWrite(Led_Blue, LOW);
        State = LED_STATE_GREEN;
        return State;
        delay(5000); // Delay to inform customer that their request has been processed.
    }
}



#endif