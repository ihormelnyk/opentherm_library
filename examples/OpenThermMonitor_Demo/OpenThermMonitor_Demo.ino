/*
OpenTherm Monitor Example
By: Ihor Melnyk
Date: May 1st, 2019
*/


#include <Arduino.h>
#include <OpenTherm.h>

const int mInPin = 4; //ESP8266
const int mOutPin = 5;  //ESP8266

const int sInPin = 12;  //ESP8266
const int sOutPin = 13;  //ESP8266

OpenTherm mOT(mInPin, mOutPin);
OpenTherm sOT(sInPin, sOutPin, true);

void mHandleInterrupt() {
    mOT.handleInterrupt();
}

void sHandleInterrupt() {
    sOT.handleInterrupt();
}

void processRequest(unsigned long request, OpenThermResponseStatus status) {
    Serial.println("T" + String(request, HEX));  //master/thermostat request
    unsigned long response = mOT.sendRequest(request);
    if (response) {
        Serial.println("B" + String(response, HEX)); //slave/boiler response
        sOT.sendResponse(response);
    }
}

void setup()
{
    Serial.begin(9600);	//9600 supported by OpenTherm Monitor App
    mOT.begin(mHandleInterrupt);
    sOT.begin(sHandleInterrupt, processRequest);
}

void loop()
{
    sOT.process();
}
