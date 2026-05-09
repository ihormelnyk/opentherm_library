/*
OpenTherm Gateway/Monitor Example
By: Ihor Melnyk
Date: May 1st, 2019
http://ihormelnyk.com
*/

#include <Arduino.h>
#include <OpenTherm.h>

const int mInPin = 2;  // for Arduino, 4 for ESP8266 (D2), 21 for ESP32
const int mOutPin = 4; // for Arduino, 5 for ESP8266 (D1), 22 for ESP32

const int sInPin = 3;  // for Arduino, 12 for ESP8266 (D6), 19 for ESP32
const int sOutPin = 5; // for Arduino, 13 for ESP8266 (D7), 23 for ESP32

OpenTherm mOT(mInPin, mOutPin);
OpenTherm sOT(sInPin, sOutPin, true);

OpenTherm* g_ptr2_mOTinstance = nullptr; // Global bridge
OpenTherm* g_ptr2_sOTinstance = nullptr; // Global bridge


void IRAM_ATTR mHandleInterrupt()
{
  if (nullptr != g_ptr2_mOTinstance)
  {
    g_ptr2_mOTinstance->handleInterrupt();
  }
}

void IRAM_ATTR sHandleInterrupt()
{
  if (nullptr != g_ptr2_sOTinstance)
  {
    g_ptr2_sOTinstance->handleInterrupt();
  }
}

void processRequest(unsigned long request, OpenThermResponseStatus status)
{
    Serial.println("T" + String(request, HEX)); // master/thermostat request
    unsigned long response = mOT.sendRequest(request);
    if (response)
    {
        Serial.println("B" + String(response, HEX)); // slave/boiler response
        sOT.sendResponse(response);
    }
}

void setup()
{
    Serial.begin(9600);          // 9600 supported by OpenTherm Monitor App
	
	g_ptr2_mOTinstance = &mOT;
	g_ptr2_sOTinstance = &sOT;
	
    mOT.begin(mHandleInterrupt); // for ESP ot.begin(); without interrupt handler can be used
    sOT.begin(sHandleInterrupt, processRequest);
}

void loop()
{
    sOT.process();
}
