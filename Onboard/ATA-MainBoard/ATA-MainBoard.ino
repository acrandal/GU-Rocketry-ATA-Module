/**
 * Alternative To Aluminum (ATA) Main Board
 * 
 * Telemetry Payload for Gonzaga University's
 * ASME Rocketry competition rocket, 2021-2022
 * 
 * Collects inertial, environmental, and GPS data
 * Stores locally on an SD card
 * Transmits via RFM9x LoRa for real time visualization
 * 
 * Authors:
 *   Aaron S. Crandall <crandall@gonzaga.edu>
 *   
 * Copyright: 2021
 * License: GPL v3.0
 */

#include "StatusLight.h"

StatusLight statusLight;


// the setup function runs once when you press reset or power the board
void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting up my light!");
    statusLight = StatusLight();
    //statusLight.setRed();
    statusLight.setBooting();
    delay(1000);
    Serial.println("Light started");
}

// the loop function runs over and over again forever
void loop() {


    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    statusLight.setNominal();
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    statusLight.setOff();
    delay(1000);                       // wait for a second
}
