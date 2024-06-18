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

  EstablishConnection(ssid, pass, broker, port);                     // ssid, pass, broker, port
  SensorInt();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {

  mqttClient.poll();                                                  // Keep connection established

  uint8_t CustomerState = CustomerPing();
  
  if (CustomerState == 1)                                             // Checks if the customer has pressed the button, where the value of 1 is returned if the LED is green. Green meaning customer requested assistance.
    {
      const char* button_msg = "Customer requested assistance.";
      SendMessageString(button_msg, topic);
      const char* button_rsp = "Help is on the way, give us just a moment.";
      SendMessageString(button_rsp, topic2);
    }
    


 const char* OrderState = Order();                                     // Order(); Function running in loop to detect and store a String OrderState if Order(); returns a string.

 if (OrderState != NULL && OrderState[0] != '\0')                       // Checks if order has been placed, the condition is that if the string is NOT empty, then execute the if code. Else return a no order placed.
    {                                                                   // SendMessageString can only send const char* instead of Strings, In order to concatenate it with " Customer has ordered ", we do conversions.       
      String order_msg_str = "Customer has ordered " + String(OrderState);
      const char* order_msg = order_msg_str.c_str();                            // Strings can be concatenated together, just like in C++.
      SendMessageString(order_msg, topic3);
      

      const char* order_rsp;                                            // If condition to handle invalid response on the Keypad. Arduino allows direct checking of strings.
      if (OrderState == "Invalid selection") 
      {
        order_rsp = "Invalid Selection, please provide a correct input after 1 second.";
        delay(5000);
      } 
      else 
      {
        String order_rsp_str = "You have ordered " + String(OrderState) + ", thank you for placing your order!!";
        const char* order_rsp = order_msg_str.c_str();
        delay(5000);
      }

      SendMessageString(order_rsp, topic4);
    }

}


