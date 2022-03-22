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
    bool isCardFound;

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

        Serial.print("Filname chosen: ");
        Serial.println(dataFilename);

        return;
    }

public:
    void begin() {
        isCardFound = false;

        sprintf(dataFilename, "undone");    // default filename
        datasetNumber = 0;

        pinMode(ATA_SD_WRITE_ENABLE_PIN, INPUT_PULLUP);

        Serial.print("Initializing SD card -- ");
        if (!SD.begin(ATA_SD_CHIP_SELECT_PIN)) {
            Serial.println("Card failed, or not present");
            delay(1000);

            // TODO: Set status light BAD - blink error
            isCardFound = false;
        } else {
            Serial.println("card initialized.");
            isCardFound = true;

            findNextDatasetFilename();

            Serial.print("Opening data file [");
            Serial.print(dataFilename);
            Serial.print("] -- ");
            dataFile = SD.open(dataFilename, FILE_WRITE);
            Serial.println("done.");
        }
    }

    bool isNominal() {
        return isCardFound;
    }

    void println(char * str, bool echo = false) {
        char endl[] = "\r\n";

        print(str, echo);
        print(endl, echo);
    }

    void print(const char * str, bool echo = false) {
        if( isWriteEnabled() ) {
            if(echo) { Serial.print(str); }
            dataFile.print(str);
        }
    }

    void close() {
        dataFile.close();
    }

    void flush() {
        if( isWriteEnabled() ) {
          dataFile.flush();
        }
    }

    bool isWriteEnabled() {
      return (isCardFound && digitalRead(ATA_SD_WRITE_ENABLE_PIN) == HIGH);
    }
};


#endif
