/*
OpenTherm Slave Example
By: Ihor Melnyk
Date: May 1st, 2019
http://ihormelnyk.com
*/

#include <Arduino.h>
#include <OpenTherm.h>

const int inPin = 2;  //for Arduino, 12 for ESP8266 (D6), 19 for ESP32
const int outPin = 3; //for Arduino, 13 for ESP8266 (D7), 23 for ESP32
OpenTherm ot(inPin, outPin, true);

void ICACHE_RAM_ATTR handleInterrupt() {
    ot.handleInterrupt();
}

void processRequest(unsigned long request, OpenThermResponseStatus status) {
    //receive request
    Serial.println("T" + String(request, HEX)); //master/thermostat request

    //build UNKNOWN-DATAID response
    unsigned long response = ot.buildResponse(OpenThermMessageType::UNKNOWN_DATA_ID, ot.getDataID(request), 0);

    //send response
    ot.sendResponse(response);
    Serial.println("B" + String(response, HEX)); //slave/boiler response
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Start");

    ot.begin(handleInterrupt, processRequest);
}


void loop()
{
    ot.process();
}
