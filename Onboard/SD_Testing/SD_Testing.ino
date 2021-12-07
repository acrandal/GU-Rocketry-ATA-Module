/*
  SD card file dump

  This example shows how to read a file from the SD card using the
  SD library and send it over the serial port.

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  22 December 2010
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = A5;


char dataFilename[13] = "undone";

// ** Searches for the next free number/filename for this boot & collect run
void findNextDataFilename() {
  int currFileNum = 0;
  bool done = false;
  while(!done and currFileNum < 100000) {
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



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  // Search for next unused filename
  
  

  findNextDataFilename();
  Serial.print("Chosen filename: ");
  Serial.println(dataFilename);

  File dataFile = SD.open(dataFilename, FILE_WRITE);
  dataFile.println(dataFilename);
  dataFile.println(millis());
  dataFile.close();

}


void garbage() {
  
  File dataFile = SD.open("file.dat");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop() {
}
