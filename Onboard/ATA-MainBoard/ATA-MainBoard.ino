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
#include "ATA_GPS.h"
#include "ATA_SD.h"
#include "ATA_RFM96.h"

StatusLight statusLight = StatusLight();
Environmental env = Environmental();
IMU ata_imu = IMU();
ATA_GPS ata_gps = ATA_GPS();
ATA_SD ata_sd = ATA_SD();
ATA_RFM96 ata_rfm96 = ATA_RFM96();

char buf[120];

// ** ************************************************************************
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    delay(1000);

    statusLight.begin();
    statusLight.setBooting();

    ata_rfm96.begin();          // Setup packet radio

    env.begin();
    ata_imu.begin();
    ata_gps.begin();

    // env.enableVerbose();
    // ata_gps.enableVerbose();

    ata_sd.begin();
    // ata_sd.enableVerbose();



    delay(1000);
    Serial.println(F("ATA MainBoard initialized - beginning operations."));
}


// ** ************************************************************************
void loop() {


    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    statusLight.setNominal();
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    statusLight.setOff();
    delay(100);                       // wait for a second

    env.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);

    //Serial.println(buf);

    ata_imu.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);

    //Serial.println(buf);

    ata_gps.getValues(buf);
    ata_rfm96.sendAsync(buf);

    ata_sd.println(buf);
    
    ata_sd.flush();
}
