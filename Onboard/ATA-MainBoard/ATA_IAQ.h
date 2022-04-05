/**
 * Indoor Air Quality (IAQ)
 * Based on Sparkfun Environmental Combo
 *  Notably the CCS811 air sensor
 * 
 * CO2 and TVOC: CCS811
 */


#ifndef __ATA_IAQ_H
#define __ATA_IAQ_H


#include <Wire.h>
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B    // Default CCS811 I2C Address


// ** Environmental Sensor Package for Indoor Air Quality
class ATA_IAQ {


private:
    CCS811 * myCCS811;
    bool verbose;

    void initCCS811() {
        myCCS811 = new CCS811(CCS811_ADDR);
        CCS811Core::CCS811_Status_e returnCode = myCCS811->beginWithStatus();
        Serial.print(F("CCS811 begin exited with: "));
        Serial.println(myCCS811->statusString(returnCode));
    }

    // Assemble results string for output
    void buildResultString(char * res) {
        sprintf(res, "ENV: %d,%d,%d",
            millis(),
            myCCS811->getCO2(),
            myCCS811->getTVOC(),
        );
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
    }

public:
    void begin() {
        verbose = false;
        Serial.println(F("Initializing IAQ Package"));
        Wire.begin();       // Init I2C bus if needed
        initCCS811();
    }

    // Use Temp & Humid to tune CO2 & TVOC readings properly
    void compensateCO2ForEnv(float newBMEhumidity_pct, float newBMEtemp_C) {
        myCCS811->setEnvironmentalData(newBMEhumidity_pct, newBMEtemp_C);
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    // Primary interface to retrieve values
    void getValues(char * res) {
        res[0] = 0;     // Ensure a null terminated string
        if( myCCS811->dataAvailable() ) {
            myCCS811->readAlgorithmResults(); // Update tVOC and eCO2 values
            if(verbose) { dumpReadings(); }
            buildResultString(res); // Generate data string
        }
    }


}





#endif

