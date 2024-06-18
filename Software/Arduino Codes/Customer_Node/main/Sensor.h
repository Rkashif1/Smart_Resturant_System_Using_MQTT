#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include <Key.h>
#include <Keypad.h>


uint8_t Button = 13;
uint8_t Led_Red = 12;
uint8_t Led_Green = 11;
uint8_t Led_Blue = 10;

// They keypad is assigned pins as follows: R1, R2, R3, R4 are: 9, 8, 7, 6.   C1, C2, C3 are: 5, 4, 3.    The pins are defined within the function itself.  

const uint8_t LED_STATE_RED = 0;
const uint8_t LED_STATE_GREEN = 1;

void SensorInt()
{
  pinMode (Button, INPUT);
  pinMode (Led_Red, OUTPUT); 
  pinMode (Led_Green, OUTPUT);
  pinMode (Led_Blue, OUTPUT);
}


uint8_t CustomerPing()
{
    uint8_t Press; // Temporary variable
    uint8_t State; // Variable for LED state
    
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


String Order()
{
  const uint8_t ROWS = 4; 
  const uint8_t COLS = 3; 

  char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
  };

  uint8_t rowPins[ROWS] = {9, 8, 7, 6}; 
  uint8_t colPins[COLS] = {5, 4, 3}; 

  Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // So far, we have only initialised the Keypad, which is assumed to be 3x4. 

  char key = customKeypad.getKey();   // This function takes the input from the Keypad immediately.

  if (key) {
    switch (key) {
      case '1':
        return "Pizza";
        delay(5000);
      case '2':
        return "Spagetti";
        delay(5000);
      case '3':
        return "Spargel";
        delay(5000);
      case '4':
        return "Curry";
        delay(5000);
      case '5':
        return "Soup";
        delay(5000);
      case '6':
        return "Schnitzel";
        delay(5000);
      case '7':
        return "Tequila";
        delay(5000);
      case '8':
        return "Biryani";
        delay(5000);
      case '9':
        return "Burger";
        delay(5000);
      default:
        return "Invalid selection";
        delay(1000);
    }
  }
}




#endif