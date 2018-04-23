/*
 * Project GarageController
 * Description: Controls the garage door hardware using a particle photon board
 * Author: Jack Hamilton, Kenneth McNelly
 * Date: April 2018
 */

#include "GarageHardware.h"

enum State
{
  Open,
  Closed,
  Opening,
  Closing
};

String states[4] = {"Open", "Closed", "Opening", "Closing"};

enum LightState
{
  On,
  Fading,
  Off,
  ForceOn
};

enum State s = Open; // Current State
enum LightState ls = Off; // Current State of the Light

unsigned long lastDebounceTime = 0;
const int debounceDelay = 100; // 100ms debounce delay for main button
bool debouncedButtonState = false;
bool lastButtonState = false; // last button state, for use in debouncing
bool buttonStateChanged = false;

const int lightFadeDelay = 5000; // 5 seconds to fade the light from 100 to 0
unsigned long lightFadeStartTime = 0; // Time since fading started
int lightBrightness = 50; // brightness of the light (100 = 100% brightness)
int autoOffTime = 1000; // time after door stops moving to turn off light

bool autoCloseEnabled = true; // whether the autoclose function is enabled or not
int autoCloseTime = 5000; // time to autoclose after in milliseconds
unsigned long doorStopMovingTime = 0;

// HELPER METHODS

void nextState() {
  switch (s)
  {
    case Open:
      s = Closing;
      startMotorClosing();
      ls = ForceOn;
      break;
    case Closed:
      s = Opening;
      startMotorOpening();
      ls = ForceOn;
      break;
    case Opening:
      doorStopMovingTime = millis();
      s = Open;
      ls = On;
      stopMotor();
      break;
    case Closing:
      doorStopMovingTime = millis();
      s = Closed;
      ls = On;
      stopMotor();
      break;
  }
  publishState("");
}

void switchLight() {
  switch (ls)
  {
    case On:
      ls = Off;
      Serial.println("to off?");
      break;
    case Fading:
      ls = Off;
      Serial.println("to on from fade?");
      break;
    case Off:
      ls = On;
      Serial.println("to on?");
      break;
    case ForceOn:
      ls = Off;
      Serial.println("to off?");
      break;
  }
  Serial.println("Hi");
  Serial.println(ls);
  publishState("");
}

void stopMoving() {
  switch (s)
  {
    case Opening:
      doorStopMovingTime = millis();
      stopMotor();
      s = Open;
      break;
    case Closing:
      doorStopMovingTime = millis();
      stopMotor();
      s = Closed;
      break;
  }
  publishState("");
}

void updateState () {
  if (buttonStateChanged)
  {
    buttonStateChanged = false;
    if (debouncedButtonState)
    {
      nextState();
    }
    Serial.println(states[s]);
  }
  else if (isFaultActive())
  {
    sendDebug("fault");
    stopMoving();
    ls = On;
    Serial.println(states[s]);
  }
  else if (isDoorFullyOpen() && s == Opening)
  {
    sendDebug("fully open");
    doorStopMovingTime = millis();
    s = Open;
    ls = On;
    publishState("");
    Serial.println(states[s]);
  }
  else if (isDoorFullyClosed() && s == Closing)
  {
    sendDebug("fully closed");
    doorStopMovingTime = millis();
    s = Closed;
    ls = On;
    publishState("");
    Serial.println(states[s]);
  }
  else if (millis() - doorStopMovingTime > autoCloseTime && s == Open)
  {
    sendDebug("It's been long enough, start Auto Closing");
    s = Closing;
    publishState("");
  }
}

void updateLightState() {
  if (ls == On) {
    if((s == Open || s == Closed) && millis() - doorStopMovingTime > lightFadeDelay) {
      lightFadeStartTime = millis();
      ls = Fading;
    }
    Serial.println(" l is on ");
    setLight(true);
  } else if (ls == Fading) {
    Serial.println(" l is fading");
    if(millis() - lightFadeStartTime < lightFadeDelay + autoOffTime) {
      int lightValue = (((lightFadeDelay + autoOffTime) - (millis() - lightFadeStartTime)) * lightBrightness) / 5000;
      setLightPWM(lightValue);
    } else {
      ls = Off;
    }
  } else if (ls == Off) {
    Serial.println(" l is off");
    setLight(false);
  } else if (ls == ForceOn) {
    Serial.println(" l is forcibly turned on ");
    setLight(true);
  }
}

void getDebouncedButtonInput () {
  bool buttonReading = isButtonPressed();
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
      Serial.println("button reading changed");
      buttonStateChanged = true;
      Serial.print("debounced button state changed to ");
      Serial.println(buttonReading);
      debouncedButtonState = buttonReading;
    }
  }
}

//Particle functions ============================================

const String topic = "cse222Garage/thisGarage";
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
  data += ", ";
  //
  data += "\"aCE\":";
  if(autoCloseEnabled) {
    data += "1";
  } else {
    data += "0";
  }
  data += ", ";
  //
  data += "\"aCT\":";
  data += autoCloseTime;
  data += ", ";
  //
  data += "\"lS\":";
  data += ls;
  data += ", ";
  //
  data += "\"lB\":";
  data += lightBrightness;
  data += ", ";
  //
  data += "\"aOT\":";
  data += autoOffTime;
  //
  data += "}";

  Serial.print("Data to send...");
  Serial.println(data);

  Particle.publish(topic, data, 60, PRIVATE);

  return 0;
}

//button activated from online
int trigButton(String arg) {
  nextState();
  Serial.println("buttonPressed");
  return 0;
}

//autoclose changed from online
int changeAC(String arg) {
  if (atoi(arg) == 1)
  {
    autoCloseEnabled = true;
  }
  else
  {
    autoCloseEnabled = false;
  }
  Serial.print("autoclose changed to ");
  Serial.println(arg);
  return 0;
}

//autoclose time changed from online
int changeACT(String arg) {
  autoCloseTime = atoi(arg);
  Serial.print("autoclose time changed to ");
  Serial.println(arg);
  return 0;
}

//light button activated from online
int trigLButton(String arg) {
  switchLight();
  Serial.println("lightbuttonPressed");
  return 0;
}

//change the light brightness from the web
int changeLB(String arg) {
  lightBrightness = atoi(arg);
  Serial.print("light brightness changed to ");
  Serial.println(arg);
  return 0;
}

//change the light auto off time from the web
int changeLOT(String arg) {
  autoOffTime = atoi(arg);
  Serial.print("light auto off time changed to ");
  Serial.println(arg);
  return 0;
}

void triggLButton(String event, String data) {
  switchLight();
  Serial.println("lightbuttonPressed");
}

void triggButton(String event, String data) {
  nextState();
  Serial.println("lightbuttonPressed");
}

// SETUP AND LOOP FUNCTIONS

void setup() {
  Serial.begin(9600);
  // init hardware
  Serial.println("Initializing Hardware");
  setupHardware();

  Particle.function("publishState",publishState);
  Particle.function("trigButton",trigButton);
  Particle.function("changeACT",changeACT);
  Particle.function("changeAC",changeAC);
  Particle.function("trigLButton",trigLButton);
  Particle.function("changeLB", changeLB);
  Particle.function("changeLOT", changeLOT);

//Subsribe to remot changes
  Particle.subscribe("buttonPush",triggButton, MY_DEVICES);
  Particle.subscribe("lightPush",triggLButton, MY_DEVICES);
}

void loop() {
  getDebouncedButtonInput(); // Get the debounced input
  updateLightState();
  updateState();

  // Test if enough time has passed to close the door

  // Test if enough time has passed to begin fading the light
}
