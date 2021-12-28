/**
 * Environmental module
 * Based on Sparkfun Environmental Combo
 * 
 * CO2 and TVOC: CCS811
 * Temp/Baro/Humid: BME280
 */

#ifndef __ENVIRONMENTAL_H
#define __ENVIRONMENTAL_H

#include <Wire.h>
#include <SparkFunBME280.h>
#include <SparkFunCCS811.h>


#define CCS811_ADDR 0x5B    //Default CCS811 I2C Address
#define BME280_ADDR 0x77    // Default BME280 I2C address


// ** Environmental Sensor Package Facade
class Environmental {

private:
    CCS811 * myCCS811;
    BME280 * myBME280;
    bool verbose;

    void initCCS811() {
        myCCS811 = new CCS811(CCS811_ADDR);
        CCS811Core::CCS811_Status_e returnCode = myCCS811->beginWithStatus();
        Serial.print(F("CCS811 begin exited with: "));
        Serial.println(myCCS811->statusString(returnCode));
    }

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

    // Use Temp & Humid to tune CO2 & TVOC readings properly
    void compensateCO2ForEnv() {
        float BMEtempC = myBME280->readTempC();
        float BMEhumid = myBME280->readFloatHumidity();
        myCCS811->setEnvironmentalData(BMEhumid, BMEtempC);
    }

    // Verbose debugging dump of data
    void dumpReadings() {
         //getCO2() gets the previously read data from the library
        Serial.println("CCS811 data:");
        Serial.print(" CO2 concentration : ");
        Serial.print(myCCS811->getCO2());
        Serial.println(" ppm");

        //getTVOC() gets the previously read data from the library
        Serial.print(" TVOC concentration : ");
        Serial.print(myCCS811->getTVOC());
        Serial.println(" ppb");

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

        Serial.print(" %RH: ");
        Serial.print(myBME280->readFloatHumidity(), 2);
        Serial.println(" %");

        Serial.println(); 
    }

    // Assemble results string for output
    void buildResultString(char * res) {
        sprintf(res, "ENV: %d,%d,%d,%s,%s,%s,%s",
            millis(),
            myCCS811->getCO2(),
            myCCS811->getTVOC(),
            String(myBME280->readTempC(), 2).c_str(),
            String(myBME280->readFloatPressure(), 2).c_str(),
            String(myBME280->readFloatAltitudeMeters(), 2).c_str(),
            String(myBME280->readFloatHumidity(), 2).c_str()
        );
    }

public:
    void begin() {
        verbose = false;
        Serial.println(F("Initializing Environmental Package"));
        Wire.begin();       // Init I2C bus if needed
        initCCS811();
        initBME280();
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    // Primary interface to retrieve values
    void getValues(char * res) {
        if( myCCS811->dataAvailable() ) {
            //Calling this function updates the global tVOC and eCO2 variables
            myCCS811->readAlgorithmResults();

            if(verbose) { dumpReadings(); }

            buildResultString(res);
            compensateCO2ForEnv();  // Use BME to correct CCS811 readings
        }
    }
};



#endif