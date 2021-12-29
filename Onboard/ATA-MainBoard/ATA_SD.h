/**
 * ATA's SD card management and recording
 */

#ifndef __ATA_SD_H
#define __ATA_SD_H

#include <SPI.h>
#include <SD.h>

#include "PinDefinitions.h"

#define MAX_DATASET_NUMBER 100000

// ATA's SD card manager & recoding interface
class ATA_SD {

private:
    char dataFilename[13];
    int datasetNumber;
    File dataFile;
    bool verbose;

    void findNextDatasetFilename() {
        int currFileNum = 0;
        bool done = false;
        while(!done and currFileNum < MAX_DATASET_NUMBER) {
            sprintf(dataFilename, "ATA%05d.dat", currFileNum);
            Serial.print("Testing for file: "); Serial.print(dataFilename);

            if( SD.exists(dataFilename) ) {
                Serial.println(" -- Exists, trying next");
                currFileNum++;
            } else {
                Serial.println(" -- Free, using for data");
                done = true;
            }
        }
        Serial.print("Filname chosen: "); Serial.println(dataFilename);
        return;
    }

public:
    void begin() {
        verbose = false;
        sprintf(dataFilename, "undone");    // default name
        datasetNumber = 0;

        Serial.print("Initializing SD card -- ");
        if (!SD.begin(ATA_SD_CHIP_SELECT_PIN)) {
            Serial.println("Card failed, or not present");
            // TODO: Set status light BAD - blink error
        } else {
            Serial.println("card initialized.");
        }

        findNextDatasetFilename();

        Serial.print("Opening data file [");
        Serial.print(dataFilename);
        Serial.print("] -- ");
        dataFile = SD.open(dataFilename, FILE_WRITE);
        Serial.println("done.");
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    void println(char * str) {
        print(str);
        print("\n");
    }

    void print(char * str) {
        if(verbose) { Serial.print(str); }
        dataFile.print(str);
    }

    void close() {
        dataFile.close();
    }
};







#endif