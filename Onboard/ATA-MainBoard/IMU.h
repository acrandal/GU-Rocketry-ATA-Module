/**
 * 
 */

#ifndef __ATA_IMU_H
#define __ATA_IMU_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// BNO055's are either 0x28 or 0x29
#define BNO055_I2C_ADDR 0x29
#define BNO_TYPE 55             // type of sensor (there's 80, and others)


// ATA's IMU sensor package
class IMU {
private:
    Adafruit_BNO055 * bno;
    bool verbose;

public:
    void begin() {
        verbose = false;
        Serial.print(F("Initializing IMU / BNO055 Sensor -- "));
        bno = new Adafruit_BNO055(BNO_TYPE, BNO055_I2C_ADDR);

        if( !bno->begin() ) {
            Serial.println(F("Failed to initialize BNO055 IMU "));
        }

        Serial.println(F("DONE"));
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    void getValues(char * res) {
        res[0] = 0;
        sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
        bno->getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
        bno->getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno->getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno->getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        bno->getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        bno->getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

        printEvent(&orientationData);
        printEvent(&angVelocityData);
        printEvent(&linearAccelData);
        printEvent(&magnetometerData);
        printEvent(&accelerometerData);
        printEvent(&gravityData);
    }

    void printEvent(sensors_event_t* event) {
        double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
        if (event->type == SENSOR_TYPE_ACCELEROMETER) {
            Serial.print("Accl:");
            x = event->acceleration.x;
            y = event->acceleration.y;
            z = event->acceleration.z;
        }
        else if (event->type == SENSOR_TYPE_ORIENTATION) {
            Serial.print("Orient:");
            x = event->orientation.x;
            y = event->orientation.y;
            z = event->orientation.z;
        }
        else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
            Serial.print("Mag:");
            x = event->magnetic.x;
            y = event->magnetic.y;
            z = event->magnetic.z;
        }
        else if (event->type == SENSOR_TYPE_GYROSCOPE) {
            Serial.print("Gyro:");
            x = event->gyro.x;
            y = event->gyro.y;
            z = event->gyro.z;
        }
        else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
            Serial.print("Rot:");
            x = event->gyro.x;
            y = event->gyro.y;
            z = event->gyro.z;
        }
        else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
            Serial.print("Linear:");
            x = event->acceleration.x;
            y = event->acceleration.y;
            z = event->acceleration.z;
        }
        else if (event->type == SENSOR_TYPE_GRAVITY) {
            Serial.print("Gravity:");
            x = event->acceleration.x;
            y = event->acceleration.y;
            z = event->acceleration.z;
        }
        else {
            Serial.print("Unk:");
        }

        Serial.print("\tx= ");
        Serial.print(x);
        Serial.print(" |\ty= ");
        Serial.print(y);
        Serial.print(" |\tz= ");
        Serial.println(z);
    }
};


#endif