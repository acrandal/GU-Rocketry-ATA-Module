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

#include "ATA_StatusLight.h"
#include "ATA_BME280.h"
#include "ATA_IAQ.h"
#include "ATA_IMU.h"
#include "ATA_GPS.h"
#include "ATA_SD.h"
#include "ATA_RFM96.h"

#define TASK_IAQ_INTERVAL_MS 2000               // Min value 1000
#define TASK_BME_INTERVAL_MS 200
#define TASK_GPS_INTERVAL_MS 2000               // Min value 1000
#define TASK_IMU_INTERVAL_MS 50                 // Targeting 20 Hz
#define TASK_SD_FLUSH_INTERVAL_MS 5000          // Flush buffers
#define TASK_LED_HEARTBEAT_INTERVAL_MS 2000
#define TASK_STATUS_LIGHT_OFF_DELAY_MS 60000    // 1 Minute

// Top defines for operational tasks
Scheduler taskScheduler;

// Forward definitions for taskScheduler callback functions
void ataIAQ_TaskCallback();
void ataBME280_TaskCallback();
void ataGPS_TaskCallback();
void ataIMU_TaskCallback();
void ataSDFlush_TaskCallback();
void ataLEDHeartbeatBlink_TaskCallback();
void ataStatusLightOff_TaskCallback();

// Task Scheduler task definitions for various tasks
Task taskIAQ(TASK_IAQ_INTERVAL_MS, TASK_FOREVER, &ataIAQ_TaskCallback, &taskScheduler, true);
Task taskBME280(TASK_BME_INTERVAL_MS, TASK_FOREVER, &ataBME280_TaskCallback, &taskScheduler, true);
Task taskGPS(TASK_GPS_INTERVAL_MS, TASK_FOREVER, &ataGPS_TaskCallback, &taskScheduler, true);
Task taskIMU(TASK_IMU_INTERVAL_MS, TASK_FOREVER, &ataIMU_TaskCallback, &taskScheduler, true);
Task taskSDFlush(TASK_SD_FLUSH_INTERVAL_MS, TASK_FOREVER, &ataSDFlush_TaskCallback, &taskScheduler, true);
Task taskLEDHeartbeat(TASK_LED_HEARTBEAT_INTERVAL_MS, TASK_FOREVER, &ataLEDHeartbeatBlink_TaskCallback, &taskScheduler, true);
Task taskStatusLightOff(TASK_STATUS_LIGHT_OFF_DELAY_MS, TASK_ONCE, &ataStatusLightOff_TaskCallback, &taskScheduler, false);

// Setup hardware driver handles/objects
ATA_StatusLight statusLight = ATA_StatusLight();
ATA_BME280 ata_bme280 = ATA_BME280();
ATA_IAQ ata_iaq = ATA_IAQ();
ATA_IMU ata_imu = ATA_IMU();
ATA_GPS ata_gps = ATA_GPS();
ATA_SD ata_sd = ATA_SD();
ATA_RFM96 ata_rfm96 = ATA_RFM96();

// General string buffer for data reads
char buf[120];

// Tracking current onboard LED heartbeat light status
bool lightBlinkOn = true;

// Setup the various sensors and SD card hardware
void initializeDevices() {
    int checkingDelay = 200;
    int bootingDelay = 100;

    statusLight.setChecking(); delay(checkingDelay);
    ata_rfm96.begin();          // Setup packet radio
    statusLight.setBooting(); delay(bootingDelay);

    statusLight.setChecking(); delay(checkingDelay);
    ata_bme280.begin();         // Barmometric pressure / altitude init
    statusLight.setBooting(); delay(bootingDelay);

    statusLight.setChecking(); delay(checkingDelay);
    ata_iaq.begin();            // Indoor air quality sensor init
    statusLight.setBooting(); delay(bootingDelay);

    statusLight.setChecking(); delay(checkingDelay);
    ata_imu.begin();            // Motion sensor (accel/gyro) initialized
    statusLight.setBooting(); delay(bootingDelay);

    statusLight.setChecking(); delay(checkingDelay);
    ata_gps.begin();            // GPS radio initialized
    statusLight.setBooting(); delay(bootingDelay);

    statusLight.setChecking(); delay(checkingDelay);
    ata_sd.begin();             // Initialize SD card for onboard logging
    statusLight.setBooting(); delay(bootingDelay);
}

// ** ************************************************************************
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);       // Initialize the serial port

    statusLight.begin();        // Initialize the RGB LED status light
    statusLight.setBooting();   // Show user that we're booting the board

    delay(3000);                // Delay to let serial port settle & user to connect

    initializeDevices();        // Initialize sensors and hardware

    // ata_env.enableVerbose();
    // ata_gps.enableVerbose();
    // ata_imu.enableVerbose();

    // Set status light to system boot results
    if( !ata_sd.isNominal() ) {
        statusLight.setErrorSD();   // SD card failed - wireless, but no data storage
    }
    else {
        statusLight.setNominal();   // We're running nominally, let the user know
    }

    // Initialize taskScheduler
    taskScheduler.startNow();   // Set task scheduling system to start from now (millis())
    taskStatusLightOff.enableDelayed();    // Start countdown to set status light off

    Serial.println(F("ATA MainBoard initialized - beginning operations."));
}


// ** ************************************************************************
void loop() {
    // Check the task event queue
    taskScheduler.execute();
    //delay(10);  // Tiny "polite" delay
}

// TaskScheduler callback functions/tasks
void ataIAQ_TaskCallback() {
    ata_iaq.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}

void ataBME280_TaskCallback() {
    ata_bme280.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}

void ataGPS_TaskCallback() {
    ata_gps.getValues(buf);
    ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}

void ataIMU_TaskCallback() {
    ata_imu.getValues(buf);
    // ata_rfm96.sendAsync(buf);
    ata_sd.println(buf);
}
 
void ataSDFlush_TaskCallback() {
    // Serial.println("Flushing SD buffers");
    ata_sd.flush();
}

void ataLEDHeartbeatBlink_TaskCallback() {
    if(lightBlinkOn) {
        digitalWrite(LED_BUILTIN, HIGH);
        lightBlinkOn = false;
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        lightBlinkOn = true;
    }
}

// Runs once @ after a while to turn off the status light
//  This saves a lot of power during flight
void ataStatusLightOff_TaskCallback() {
    statusLight.setOff();
}
