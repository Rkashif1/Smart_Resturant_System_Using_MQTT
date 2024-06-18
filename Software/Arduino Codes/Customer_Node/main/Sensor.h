#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include <Key.h>
#include <Keypad.h>

const uint8_t LED_STATE_RED = 0;
const uint8_t LED_STATE_GREEN = 1;
const uint8_t ROWS = 4; 
const uint8_t COLS = 3;

// They keypad is assigned pins as follows: R1, R2, R3, R4 are: 9, 8, 7, 6.   C1, C2, C3 are: 5, 4, 3.    The pins are defined within the function itself.  R

uint8_t Button = 13;
uint8_t Led_Red = 12;
uint8_t Led_Green = 11;
uint8_t Led_Blue = 10;
uint8_t rowPins[ROWS] = {9, 8, 7, 6}; 
uint8_t colPins[COLS] = {5, 4, 3};

void SensorInt()
{
  pinMode (Button, INPUT);
  digitalWrite(Button, HIGH);
  pinMode (Led_Red, OUTPUT); 
  pinMode (Led_Green, OUTPUT);
  pinMode (Led_Blue, OUTPUT);
}


uint8_t CustomerPing()
{
    uint8_t Press;                                // Temporary variable
    uint8_t State;                              // Variable for LED state
    
    Press = digitalRead(Button);                // The current signal at the sensor is read out
  
    if (Press == HIGH)                        // Active Low, so that means when button is pressed, the logic value will be low, as by default it is high due to the pull-up resistor.
    {
        digitalWrite(Led_Red, HIGH);          // LED is RED
        digitalWrite(Led_Green, LOW); 
        digitalWrite(Led_Blue, LOW); 
        State = LED_STATE_RED;
        return State;
    }
    else
    {
        digitalWrite(Led_Red, LOW);          // LED is GREEN
        digitalWrite(Led_Green, HIGH); 
        digitalWrite(Led_Blue, LOW);
        State = LED_STATE_GREEN;
        delay(3000);
        return State;                       //  to inform customer that their request has been processed.
    }
}


const char* Order()
{

  char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
  };

  Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // So far, we have only initialised the Keypad, which is assumed to be 3x4. 

  char key = customKeypad.getKey();   // This function takes the input from the Keypad immediately.

  if (key) {
    switch (key) {
      case '1':
        return "Pizza";
      case '2':
        return "Spagetti";
      case '3':
        return "Spargel";
      case '4':
        return "Curry";
      case '5':
        return "Soup";
      case '6':
        return "Schnitzel";
      case '7':
        return "Tequila";
      case '8':
        return "Biryani";
      case '9':
        return "Burger";
      default:
        return "Invalid selection";
    }
  }
}




#endif