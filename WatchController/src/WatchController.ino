#include "Adafruit_SSD1306/Adafruit_SSD1306.h"
/*

* Project Watch Controller
*/

// use hardware SPI

#define OLED_DC D3

#define OLED_CS D4

#define OLED_RESET D5

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


String gState = "Closed";

String lState = "Off";

const int button_pin = 0;
const int light_pin = 1;

const int debounceDelay = 100; // 100ms debounce delay for main button

unsigned long lastDebounceTime = 0;
bool debouncedButtonState = false;
bool lastButtonState = false; // last button state, for use in debouncing
bool buttonStateChanged = false;

unsigned long lastDebounceLightTime = 0;
bool debouncedLightState = false;
bool lastLightState = false; // last button state, for use in debouncing
bool lightStateChanged = false;


//Methods=================================================================


void getDebouncedButtonInput() {
  int button_state = digitalRead(button_pin);
  // Serial.println(button_state);
  bool buttonReading;
  if (button_state == LOW)
  {
    buttonReading = true;
  }
  else
  {
    buttonReading = false;
  }

  //Serial.println(buttonReading);
  if (buttonReading != lastButtonState)
  {
    lastDebounceTime = millis();
    lastButtonState = buttonReading;
  }
  else if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (buttonReading != debouncedButtonState)
    {
      buttonStateChanged = true;
      debouncedButtonState = buttonReading;
      Serial.println("button changed!");
    }
  }
}
//
//
// int getState(String event, String data){
//   Serial.print(event);
//   Serial.print(", data: ");
//
//   if (data != NULL){
//     Serial.println(data);
//
//     int stateIndex = data.indexOf("state");
//
//     String curState = data.substring(stateIndex + 7, stateIndex + 8);
//     Serial.println(curState);
//
//     int curStateVal = curState.toInt();
//     switch (curStateVal){
//       case 0:
//       gState = "Open";
//       break;
//
//       case 1:
//       gState = "Closed";
//       break;
//
//       case 2:
//       gState = "Opening";
//       break;
//
//       case 3:
//       gState = "Closing";
//       break;
//     }
//
//
//     int lightIndex = data.indexOf("lS");
//
//     String curLight = data.substring(lightIndex + 4, lightIndex + 5);
//     Serial.print("CURRRR LIGHT: ");
//     Serial.println(curLight);
//
//     int curLightVal = curLight.toInt();
//
//     if(curLightVal < 1){
//       lState = "On";
//     }
//     else{
//       lState = "Off";
//     }
//   }
//
//   else{
//     Serial.println("NULL");
//   }
// }

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);
  //setup buttons
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(light_pin, INPUT_PULLUP);


  //setup display
  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  display.setTextSize(2);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false);

  // Particle.subscribe("cse222Garage/thisGarage",getState, MY_DEVICES);

}


// loop() runs over and over again, as quickly as it can execute.

void loop() {
  getDebouncedButtonInput();

  if (buttonStateChanged)
  {
    buttonStateChanged = false;
    if (debouncedButtonState)
    {
      Particle.publish("buttonPush", "", 60, PRIVATE);
      Serial.println("publish button");
    }
  }

  getDebouncedLightInput();
  if (lightStateChanged)
  {

    lightStateChanged = false;
    if (debouncedLightState)
    {
      Particle.publish("lightPush", "", 60, PRIVATE);
      Serial.println("publish light");
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  // The core of your code will likely live here.
  display.println("Garage:");
  display.println(gState);
  display.println("Light:");
  display.println(lState);
  display.display();

//  delay(500);

}
