#include "MQTT.h"
#include "Sensor.h"

char ssid[] = "Crimson";                                         // your network SSID (name)
char pass[] = "JoJopart5";                                       // your network password (use for WPA, or use as key for WEP)


const char broker[] = "test.mosquitto.org";                      
int        port     = 1883;
const char topic[]  = "BUTTON_STATUS";                           // Sent to backend subscriber, checking for request of human assistance.
const char topic2[]  = "BUTTON_RESPONSE";                        // Sent to customer subscriber, acknowledge response showcased to customer.
const char topic3[]  = "ORDER_STATUS";                           // Sent to backend subscriber, checking for if order is placed.
const char topic4[]  = "ORDER_RESPONSE";                         // Sent to customer subscriber, acknowledging that their order is placed or is invalid.


void setup() {

  Serial.begin(9600);

  EstablishConnection(ssid, pass, broker, port);                  // ssid, pass, broker, port

}

void loop() {

  mqttClient.poll();                                              // Keep connection established

  uint8_t CustomerState = CustomerPing();
  
  if (CustomerState == 1)                                         // Checks if the customer has pressed the button, where the value of 1 is returned if the LED is green. Green meaning customer requested assistance.
    {
      const char* button_msg = "Customer requested assistance.";
      SendMessageString(button_msg, topic);
      const char* button_rsp = "Help is on the way, give us just a moment.";
      SendMessageString(button_rsp, topic2);
    }
  else 
    {
      const char* button_default = "Button not pressed. Customer did not request assistance.";
      SendMessageString(button_default, topic);
    } 
    



  String OrderState = Order();                                     // Order Function running in loop to detect and store a String if the function returns a string.

  if (OrderState != "")                                            // Checks if order has been placed, the condition is that if the string is NOT empty, then execute the if code. Else return a no order placed.
    {                                                              // OrderState is a string, however SendMessageString only accepts a character array, which .c_str is used for, to convert that string into a char array.        
      String order_msg = "Customer has ordered " + OrderState;     // Strings can be concatenated together, just like in C++.
      SendMessageString(order_msg.c_str(), topic3); 

      String order_rsp;                                            // If condition to handle invalid response on the Keypad. Arduino allows direct checking of strings.
      if (OrderState == "Invalid selection") 
      {
        order_rsp = "Invalid Selection, please provide a correct input after 1 second.";
      } 
      else 
      {
        order_rsp = "You have ordered " + OrderState + ", thank you for placing your order!!";
      }

      SendMessageString(order_rsp.c_str(), topic4);
    }


  else 
    {
      const char* order_default = "Order has not been placed yet.";
      SendMessageString(order_default, topic3);
    }

}


