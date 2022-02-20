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
 
#include <TaskScheduler.h>

#include "StatusLight.h"
#include "Environmental.h"
#include "IMU.h"
#include "ATA_GPS.h"
#include "ATA_SD.h"
#include "ATA_RFM96.h"

// Top defines for operational tasks
Scheduler taskScheduler;

void t1Callback();
void ataEnvironmental_TaskCallback();
void ataGPS_TaskCallback();

Task t1(2000, 10, &t1Callback, &taskScheduler, true);
Task taskEnvironmental(2000, TASK_FOREVER, &ataEnvironmental_TaskCallback, &taskScheduler, true);
Task taskGPS(1000, TASK_FOREVER, &ataGPS_TaskCallback, &taskScheduler, true);


// Setup hardware driver handles/objects
StatusLight statusLight = StatusLight();
Environmental env = Environmental();
IMU ata_imu = IMU();
ATA_GPS ata_gps = ATA_GPS();
ATA_SD ata_sd = ATA_SD();
ATA_RFM96 ata_rfm96 = ATA_RFM96();

// General string buffer for data reads
char buf[120];

bool lightBlinkOn = true;

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
    ata_sd.enableVerbose();

    delay(1000);
    Serial.println(F("ATA MainBoard initialized - beginning operations."));

    statusLight.setNominal();
    taskScheduler.startNow();  // set point-in-time for task scheduling start
}


// ** ************************************************************************
void loop() {


    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //statusLight.setNominal();
    //delay(100);                       // wait for a second
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //statusLight.setOff();
    //delay(100);                       // wait for a second

    //env.getValues(buf);
    //ata_rfm96.sendAsync(buf);
    //ata_sd.println(buf);

    //Serial.println(buf);

    //ata_imu.getValues(buf);
    //ata_rfm96.sendAsync(buf);
    //ata_sd.println(buf);

    //Serial.println(buf);

    //ata_gps.getValues(buf);
    //ata_rfm96.sendAsync(buf);
    //ata_sd.println(buf);
    
    //ata_sd.flush();

    // Check the task event queue
    taskScheduler.execute();
    delay(10);  // Tiny "polite" delay
}


void t1Callback() {
    Serial.print("************************** t1: ");
    Serial.println(millis());
}

void ataEnvironmental_TaskCallback() {
    env.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}

void ataGPS_TaskCallback() {
    ata_gps.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}
