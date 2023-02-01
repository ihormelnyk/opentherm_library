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

void IRAM_ATTR handleInterrupt() {
    ot.handleInterrupt();
}

void processRequest(unsigned long request, OpenThermResponseStatus status) {
    Serial.println("T" + String(request, HEX)); //master/thermostat request

    unsigned long response = 0;
    OpenThermMessageID id = ot.getDataID(request);
    uint16_t data = ot.getUInt(request);
    float f = ot.getFloat(request);
    switch(id)
    {
      case OpenThermMessageID::Status:
      {
        uint8_t statusRequest = data >> 8;
        uint8_t chEnable = statusRequest & 0x1;
        uint8_t dhwEnable = statusRequest & 0x2;
        data &= 0xFF00;
        //data |= 0x01; //fault indication
        if (chEnable) data |= 0x02; //CH active
        if (dhwEnable) data |= 0x04; //DHW active
        if (chEnable || dhwEnable) data |= 0x08; //flame on
        //data |= 0x10; //cooling active
        //data |= 0x20; //CH2 active
        //data |= 0x40; //diagnostic/service event
        //data |= 0x80; //electricity production on

        response = ot.buildResponse(OpenThermMessageType::READ_ACK, id, data);
        break;
      }
      case OpenThermMessageID::TSet:
      {
        response = ot.buildResponse(OpenThermMessageType::WRITE_ACK, id, data);
        break;
      }
      case OpenThermMessageID::Tboiler:
      {
        data = ot.temperatureToData(45);
        response = ot.buildResponse(OpenThermMessageType::READ_ACK, id, data);
        break;
      }
      default:
      {
        //build UNKNOWN-DATAID response
        response = ot.buildResponse(OpenThermMessageType::UNKNOWN_DATA_ID, ot.getDataID(request), 0);   
      }
    }
    Serial.println("B" + String(response, HEX)); //slave/boiler response

    //send response
    delay(20); //20..400ms, usually 100ms
    ot.sendResponse(response);
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
