/*
 * ATA Payload
 * 
 * Alternative To Aluminum Telemetry & Environment Payload
 * 
 * Gonzaga University ASME Rocketry Club
 *   2021 competition rocket payload
 * 
 * @author Aaron S. Crandall <crandall@gonzaga.edu>
 * @copyright 2021
 * @license GPL v3
 * 
 * Hardware:
 *  Adafruit Feather 32u4 LoRa Radio (RFM9x) -- adafruit.com/product/3078
 *  900MHz spring antenna
 *  Adafruit Ultimate GPS FeatherWing -- adafruit.com/product/3133
 */


#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <RH_RF95.h>
#include <Adafruit_GPS.h>

// ** RFM9x LoRa Radio/Radiohead library setup
/* for feather32u4 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

// Transmission frequency - matches RX bridge setting
#define RF95_FREQ 915.0

// Radio driver -- Radiohead library
RH_RF95 rf95(RFM95_CS, RFM95_INT);


// ** BME280 Temp/Humidity/Pressure sensor
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;  // I2C interface

// ** GPS setup
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);

// **
char msg[50];  // General message buffer for string work
int16_t packetnum = 0;  // packet counter, we increment per xmission


// ** *******************************************************************
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  delay(100);

  Serial.println(F("ATA Payload Module Booting"));

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println(F("LoRa radio init failed"));
    while (1);
  }
  Serial.println(F("LoRa radio init OK!"));

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println(F("setFrequency failed"));
    while (1);
  }
  Serial.print(F("Set Freq to: ")); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
  Serial.println(F("Transmitter booted - beginning normal operations"));

    // ************** BME208
      unsigned status;
    
    // default settings
    status = bme.begin(0x76);  // Aliexpress knockoff is at I2C 0x76
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    // ************** GPS
    Serial.println("Setting up GPS");
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);
    Serial.println("GPS setup");
}


char radiopacket[20]; 

// ** **********************************************************************
void loop()
{

  char c = GPS.read();
  //if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {
    Serial.print(GPS.lastNMEA());
  }

  Serial.println((int)GPS.fix);

  //sprintf(radiopacket, "Packet #%d", packetnum++);
  //rf95.send((uint8_t *)radiopacket, strlen(radiopacket));
  //rf95.waitPacketSent();

/*

    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    */

    float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    //sprintf(msg, "{\"alt\": \"%s\"}", String(alt, 2).c_str());
    //Serial.println(msg);
    *msg = "hi";

  rf95.send((uint8_t *)msg, strlen(msg));
  rf95.waitPacketSent();
    

    //Serial.println();


  delay(20); // Wait 1 second between transmits, could also 'sleep' here!
}
