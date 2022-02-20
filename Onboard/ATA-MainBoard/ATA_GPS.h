/**
 * GPS wrapper
 */

#ifndef __ATA_GPS_H
#define __ATA_GPS_H

#include <Adafruit_GPS.h>

#define GPSSerial Serial1
#define GPS_BAUD_RATE 9600



// ATA's GPS wrapper and interface
class ATA_GPS {

private:
    Adafruit_GPS * gps;
    bool verbose;

public:
    void begin() {
        verbose = false;
        gps = new Adafruit_GPS(&GPSSerial);

        gps->begin(GPS_BAUD_RATE);
        gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        gps->sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);     // or 5Hz
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    // read in GPS data to the provided buffer
    void getValues(char * res) {
        res[0] = 0;     // Null term default

        // Read out GPS serial buffer & parse out values
        while(gps->read()) {   // This is scary (potential infinite loop)
            if (gps->newNMEAreceived()) {
                // if(verbose) { Serial.println("Got a NMEA"); }
                gps->parse(gps->lastNMEA());
            }
        }

        if(verbose) { dumpReadings(); }
        buildResultString(res); // Generate data string

    }

    // Create output string for logging/transmission
    void buildResultString(char * res) {
        sprintf(res, "GPS: %d,%d,%s%s,%s%s,%s",
                millis(),
                gps->fix,
                String(gps->latitude, 6).c_str(),
                String(gps->lat).c_str(),
                String(gps->longitude, 6).c_str(),
                String(gps->lon).c_str(),
                String(gps->altitude,2).c_str()
            );
    }

    // Serial dump GPS values for testing/debug
    void dumpReadings() {
        Serial.print("Fix: ");
        Serial.print(gps->fix);
        Serial.print(" | Location: ");
        Serial.print(gps->latitude, 6); Serial.print(gps->lat);
        Serial.print(", ");
        Serial.print(gps->longitude, 6); Serial.print(gps->lon);
        //Serial.print("Speed (knots): "); Serial.println(GPS.speed);
        //Serial.print("Angle: "); Serial.println(GPS.angle);
        Serial.print("  | Altitude: "); Serial.println(gps->altitude);
        //Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }

};





#endif
