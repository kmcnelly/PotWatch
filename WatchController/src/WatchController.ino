#include "Adafruit_SSD1306/Adafruit_SSD1306.h"
#include <OneWire.h>
/*

* Project Watch Controller
*/

// use hardware SPI

#define OLED_DC D3

#define OLED_CS D4

#define OLED_RESET D5

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);





const int button_pin = 0;
const int light_pin = 1;

const int debounceDelay = 100; // 100ms debounce delay for main button

unsigned long lastDebounceTime = 0;
bool debouncedButtonState = false;
bool lastButtonState = false; // last button state, for use in debouncing
bool buttonStateChanged = false;


//Temp Sensor===================================

const int updateDelay = 3000;//reads temp every 5 sec


OneWire ds = OneWire(D0);  // 1-wire signal on pin D4

unsigned long lastUpdate = 0;

float lastTemp;
float curTemp;

float tempTolerance = 10; //degrees

//STATE====================================
enum State
{
  Idle,
  Prep,
  Cooking,
  Cooling
};

enum State s = Idle;

//LIGHT=====================================
enum LightState
{
  On,
  Off,
};
enum LightState ls = On; // Current State of the Light

//Targets===================================

//temperature
int tempTarget = NULL;
bool tempEnabled = false;


//timer
int timeTarget = NULL;
bool timerEnabled = false;

bool waitState = true;
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

//CODE FROM: Particle TUTORIAL#4 – *uses OneWire library
//https://docs.particle.io/tutorials/projects/maker-kit/#tutorial-4-temperature-logger
//Used to retrieve the temperature data with OneWire library
void readTemp(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  // The order is changed a bit in this example
  // first the returned address is printed

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  // second the CRC is checked, on fail,
  // print error and just return to try again

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();

  // we have a good address at this point
  // what kind of chip do we have?
  // we will set a type_s value for known types or just return

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS1820/DS18S20");
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    case 0x26:
      Serial.println("  Chip = DS2438");
      type_s = 2;
      break;
    default:
      Serial.println("Unknown device type.");
      return;
  }

  // this device has temp so let's read it

  ds.reset();               // first clear the 1-wire bus
  ds.select(addr);          // now select the device we just found
  // ds.write(0x44, 1);     // tell it to start a conversion, with parasite power on at the end
  ds.write(0x44, 0);        // or start conversion in powered mode (bus finishes low)

  // just wait a second while the conversion takes place
  // different chips have different conversion times, check the specs, 1 sec is worse case + 250ms
  // you could also communicate with other devices if you like but you would need
  // to already know their address to select them.

  delay(1000);     // maybe 750ms is enough, maybe not, wait 1 sec for conversion

  // we might do a ds.depower() (parasite) here, but the reset will take care of it.

  // first make sure current values are in the scratch pad

  present = ds.reset();
  ds.select(addr);
  ds.write(0xB8,0);         // Recall Memory 0
  ds.write(0x00,0);         // Recall Memory 0

  // now read the scratch pad

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE,0);         // Read Scratchpad
  if (type_s == 2) {
    ds.write(0x00,0);       // The DS2438 needs a page# to read
  }

  // transfer and print the values

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s == 2) raw = (data[2] << 8) | data[1];
  byte cfg = (data[4] & 0x60);

  switch (type_s) {
    case 1:
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
      celsius = (float)raw * 0.0625;
      break;
    case 0:
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      // default is 12 bit resolution, 750 ms conversion time
      celsius = (float)raw * 0.0625;
      break;

    case 2:
      data[1] = (data[1] >> 3) & 0x1f;
      if (data[2] > 127) {
        celsius = (float)data[2] - ((float)data[1] * .03125);
      }else{
        celsius = (float)data[2] + ((float)data[1] * .03125);
      }
  }

  // remove random errors
  if((((celsius <= 0 && celsius > -1) && lastTemp > 5)) || celsius > 125) {
      celsius = lastTemp;
  }

  fahrenheit = celsius * 1.8 + 32.0;
  lastTemp = celsius;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

  curTemp = fahrenheit;

}

//switch to next state idle => prep => cooking => cooling
void nextState() {
  switch (s)
  {
    case Idle:
      s = Prep;
      ls = Off;
      break;
    case Prep:
      s = Cooking;
      ls = Off;
      break;
    case Cooking:
      s = Cooling;
      ls = Off;
      break;
    case Cooling:
      s = Idle;
      ls = On;
      break;
  }
  publishState("");
}

bool isDone(){
  if(tempEnabled && timerEnabled){
    if(abs(curTemp - tempTarget) < tempTolerance && (millis() > timeTarget)){
      return true;
    }
  }
  else if(tempEnabled && !timerEnabled){
    if(abs(curTemp - tempTarget) < tempTolerance){
      return true;
    }
  }
  else if(tempEnabled && !timerEnabled){
    if(millis() >= timeTarget){
      return true;
    }
  }
  return false;
}



//check if button pressed: then move to next State
void updateState () {
  if (buttonStateChanged)
  {
    buttonStateChanged = false;
    if (debouncedButtonState)
    {
      if(tempEnabled || timerEnabled){
        if(waitState){
          waitState = false;

          Serial.println("change state");
          nextState();

        }
      }
      else{
        //sendAlert;
        // alert('Enter one or both target values for next stage');
      }
    }
  }
}

void updateWaitState(){
  waitState = isDone();
  if(waitState){
    ls = On;

    tempEnabled = false;
    timerEnabled = false;
  }
}

const String topic = "thisWatch";
//publishing
int publishState(String arg) {
  // Goal: Publish a valid JSON string containing the state of the light:
  //   Ex:   "{"powered":true, "r":255, "g":255, "b":255}"
  //   Note that each property has double quotes!

//fix JSON string===============================
  String data = "{";
  //
  data += "\"state\":";
  data += s;
  // data += ", ";
  // //
  // data += "\"aCE\":";
  // if(autoCloseEnabled) {
  //   data += "1";
  // } else {
  //   data += "0";
  // }
  // data += ", ";
  // //
  // data += "\"aCT\":";
  // data += autoCloseTime;
  // data += ", ";
  // //
  // data += "\"lS\":";
  // data += ls;
  // data += ", ";
  // //
  // data += "\"lB\":";
  // data += lightBrightness;
  // data += ", ";
  // //
  // data += "\"aOT\":";
  // data += autoOffTime;
  //
  data += "}";

  Serial.print("Data to send...");
  Serial.println(data);

  Particle.publish(topic, data, 60, PRIVATE);

  return 0;
}

int changeTime(String arg){
  timeTarget = atoi(arg);
  Serial.print("timeTarget changed to ");
  Serial.println(arg);
  return 0;
}

int changeTemp(String arg){
  tempTarget = atoi(arg);
  Serial.print("timeTarget changed to ");
  Serial.println(arg);
  return 0;
}

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);
  //setup buttons
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(light_pin, OUTPUT);

  Particle.function("publishState",publishState);
  Particle.function("changeTime",changeTime);
  Particle.function("changeTemp",changeTemp);

  //setup display
  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  display.setTextSize(2);       // text size
  display.setTextColor(WHITE); // text color
  display.setTextWrap(false);

  // Particle.subscribe("thisWatch",getState, MY_DEVICES);

}


// loop() runs over and over again, as quickly as it can execute.

void loop() {
  getDebouncedButtonInput();
  updateWaitState();
  updateState();

  if(ls == On){
    digitalWrite(light_pin, HIGH);
  }
  else{
    digitalWrite(light_pin, LOW);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  // The core of your code will likely live here.
  display.println("HI:");
  display.println(s);

  display.display();
  if(millis() - lastUpdate >= updateDelay){
    readTemp();
    lastUpdate = millis();
    // Particle.publish("temp," "" + curTemp, 60, PRIVATE);
  }

}
