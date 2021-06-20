/*
OpenTherm Master Communication Example
By: Ihor Melnyk
Date: January 19th, 2018

Uses the OpenTherm library to get/set boiler status and water temperature
Open serial monitor at 115200 baud to see output.

Hardware Connections (OpenTherm Adapter (http://ihormelnyk.com/pages/OpenTherm) to Arduino/ESP8266):
-OT1/OT2 = Boiler X1/X2
-VCC = 5V or 3.3V
-GND = GND
-IN  = Arduino (3) / ESP8266 (5) Output Pin
-OUT = Arduino (2) / ESP8266 (4) Input Pin

Controller(Arduino/ESP8266) input pin should support interrupts.
Arduino digital pins usable for interrupts: Uno, Nano, Mini: 2,3; Mega: 2, 3, 18, 19, 20, 21
ESP8266: Interrupts may be attached to any GPIO pin except GPIO16,
but since GPIO6-GPIO11 are typically used to interface with the flash memory ICs on most esp8266 modules, applying interrupts to these pins are likely to cause problems
*/


#include <Arduino.h>
#include <OpenTherm.h>

const int inPin = 2;  //for Arduino, 4 for ESP8266 (D2), 21 for ESP32
const int outPin = 3; //for Arduino, 5 for ESP8266 (D1), 22 for ESP32
OpenTherm ot(inPin, outPin);

void ICACHE_RAM_ATTR handleInterrupt() {
    ot.handleInterrupt();
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Start");

    ot.begin(handleInterrupt);
}

void loop()
{
    //Set/Get Boiler Status
    bool enableCentralHeating = true;
    bool enableHotWater = true;
    bool enableCooling = false;
    unsigned long response = ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
    OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
    if (responseStatus == OpenThermResponseStatus::SUCCESS) {
        Serial.println("Central Heating: " + String(ot.isCentralHeatingActive(response) ? "on" : "off"));
        Serial.println("Hot Water: " + String(ot.isHotWaterActive(response) ? "on" : "off"));
        Serial.println("Flame: " + String(ot.isFlameOn(response) ? "on" : "off"));
    }
    if (responseStatus == OpenThermResponseStatus::NONE) {
        Serial.println("Error: OpenTherm is not initialized");
    }
    else if (responseStatus == OpenThermResponseStatus::INVALID) {
        Serial.println("Error: Invalid response " + String(response, HEX));
    }
    else if (responseStatus == OpenThermResponseStatus::TIMEOUT) {
        Serial.println("Error: Response timeout");
    }

    //Set Boiler Temperature to 64 degrees C
    ot.setBoilerTemperature(64);

    //Get Boiler Temperature
    float ch_temperature = ot.getBoilerTemperature();
    Serial.println("CH temperature is " + String(ch_temperature) + " degrees C");

    //Set DHW setpoint to 40 degrees C
    ot.setDHWSetpoint(40);

    //Get DHW Temperature
    float dhw_temperature = ot.getDHWTemperature();
    Serial.println("DHW temperature is " + String(dhw_temperature) + " degrees C");

    Serial.println();
    delay(1000);
}