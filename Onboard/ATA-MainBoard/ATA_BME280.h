/**
 * BME280 Barometric Pressure / Humidity Sensor Wrapper
 * Based on Sparkfun Environmental Combo
 * 
 * Temp/Baro/Altitude/Humid: BME280
 */

#ifndef __ATA_BME280_H
#define __ATA_BME280_H

#include <Wire.h>
#include <SparkFunBME280.h>

#define BME280_ADDR 0x77    // Default BME280 I2C address

// ** Environmental Sensor Package Facade
class ATA_BME280 {

private:
    BME280 * myBME280;
    bool verbose;

    void initBME280() {
        myBME280 = new BME280();

        //For I2C, enable the following and disable the SPI section
        myBME280->settings.commInterface = I2C_MODE;
        myBME280->settings.I2CAddress = BME280_ADDR;
        myBME280->settings.runMode = 3; //Normal mode
        myBME280->settings.tStandby = 0;
        myBME280->settings.filter = 4;
        myBME280->settings.tempOverSample = 5;
        myBME280->settings.pressOverSample = 5;
        myBME280->settings.humidOverSample = 5;

        //Calling .begin() causes the settings to be loaded
        delay(10); //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
        myBME280->begin();
        Serial.println(F("BME280 initialized"));
    }

    // Verbose debugging dump of data
    void dumpReadings() {
        Serial.println("BME280 data:");
        Serial.print(" Temperature: ");
        Serial.print(myBME280->readTempC(), 2);
        Serial.println(" degrees C");

        Serial.print(" Pressure: ");
        Serial.print(myBME280->readFloatPressure(), 2);
        Serial.println(" Pa");

        Serial.print(" Altitude: ");
        Serial.print(myBME280->readFloatAltitudeMeters(), 2);
        Serial.println("m");

        Serial.print(" %Relative Humidity: ");
        Serial.print(myBME280->readFloatHumidity(), 2);
        Serial.println("%");

        Serial.println(); 
    }

    // Assemble results string for output
    void buildResultString(char * res) {
        sprintf(res, "BME: %ld,%s,%s,%s,%s",
            millis(),
            String(myBME280->readTempC(), 2).c_str(),
            String(myBME280->readFloatPressure(), 2).c_str(),
            String(myBME280->readFloatAltitudeMeters(), 2).c_str(),
            String(myBME280->readFloatHumidity(), 2).c_str()
        );
    }

public:
    void begin() {
        verbose = false;
        Serial.println(F("Initializing BME280 Temp/Pressure/Alt/Humidity Package"));
        Wire.begin();       // Init I2C bus if needed
        initBME280();
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    // Primary interface to retrieve values
    void getValues(char * res) {
        res[0] = 0;     // Ensure a null terminated string
        if(verbose) { dumpReadings(); }
        buildResultString(res); // Generate data string
    }

    float getAltitude() {
      return myBME280->readFloatAltitudeMeters();
    }
};

#endif
