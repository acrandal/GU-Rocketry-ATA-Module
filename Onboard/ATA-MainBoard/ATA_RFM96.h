/**
 * ATA's packet radio wrapper
 */

#ifndef __ATA_RFM96_H
#define __ATA_RFM96_H

#include <SPI.h>
#include <RH_RF95.h>

// Pin settings for Feather M0 RFM9x LoRa board
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// #define RF95_FREQ 915.0  // Working value for 900 MHz boards
#define RF95_FREQ 434.0     // currently set for some 400 MHz M0 boards


// Packet radio class wrapper for ATA main board
class ATA_RFM96 {
private:
    bool verbose;
    RH_RF95 * rf95;

    void configureResetPin() {
        pinMode(RFM95_RST, OUTPUT);
        digitalWrite(RFM95_RST, HIGH);
        delay(10); 
    }

    void resetRadio() {
        digitalWrite(RFM95_RST, LOW);
        delay(10);
        digitalWrite(RFM95_RST, HIGH);
        delay(10);
    }

public:
    void begin() {
        Serial.print("Initializing RF95x radio -- ");

        configureResetPin();
        resetRadio();   // manual reset

        rf95 = new RH_RF95(RFM95_CS, RFM95_INT);

        rf95->init();       // Initialize radio hardware
        Serial.print(" init -");
        rf95->setFrequency(RF95_FREQ);
        Serial.print(" freq - ");
        rf95->setTxPower(23, false);
        Serial.print(" Tx power high - ");

        Serial.println("done.");
    }

    void sendAsync(char * msg) {
        send(msg, false);
    }

    void send(char * msg, bool blocking=true) {
        rf95->send((uint8_t *)msg, strlen(msg));
        if(blocking) {
            rf95->waitPacketSent();
        }
    }
};


#endif
