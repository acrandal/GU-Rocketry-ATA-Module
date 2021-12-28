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
#include "Environmental.h"
#include "IMU.h"

StatusLight statusLight = StatusLight();
Environmental env = Environmental();
IMU ata_imu = IMU();

// ** ************************************************************************
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    delay(1000);

    statusLight.begin();
    statusLight.setBooting();

    env.begin();
    // env.enableVerbose();

    ata_imu.begin();
    delay(1000);
}


// ** ************************************************************************
void loop() {


    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    statusLight.setNominal();
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    statusLight.setOff();
    delay(1000);                       // wait for a second

    char buf[120];
    env.getValues(buf);
    Serial.println(buf);

    ata_imu.getValues(buf);
    Serial.println(buf);
}
