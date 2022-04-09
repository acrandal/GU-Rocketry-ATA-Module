/*
 * ATA Receiver Bridge
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
 *  Adafruit Feather 32u4 LoRa Radio (RFM9x) -- https://www.adafruit.com/product/3078
 *  900MHz spring antenna
 */

#include <SPI.h>
#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>


// Pin settings for Feather 32u4 board
/*
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
*/

// Pin settings for Feather M0 RFM9x board
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3


// #define RF95_FREQ 915.0
#define RF95_FREQ 434.0     // currently set for some 400 MHz M0 boards


// Create RFM API object
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Setup encryption
// See RadioHead rf95_encrypted_client.pde
//  Uses https://github.com/rweather/arduinolibs/tree/master/libraries/CryptoLW
Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

//unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key

// Key is ASCII: "GUASMEROCKETRY!!"
unsigned char encryptkey[16] = {71, 85, 65, 83, 77, 69, 82, 79, 67, 75, 69, 84, 82, 89, 33, 33};

char msg[255];


// ** *************************************************************************
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);

  Serial.println("DAT: {\"MSG\":\"ATA Receiver Bridge Booting\"}");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("DAT: {\"MSG\":\"ATA Receiver Bridge Booting\"}");

    Serial.println("ERR: {\"MSG\":\"LoRa radio init failed\"}");
    //Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("DAT: {\"MSG\":\"LoRa radio init OK!\"}");

  // Setting cipher's key
  Serial.println("DAT: {\"MSG\":\"Setting crypto key\"}");
  myCipher.setKey(encryptkey, sizeof(encryptkey));


  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("ERR: {\"MSG\":\"setFrequency failed\"}");
    while (1);
  }

  sprintf(msg, "DAT: {\"MSG\":\"Set Freq to: %s\"}", String(RF95_FREQ, 2).c_str());
  Serial.println(msg);
  //Serial.print("DAT: {\"MSG\":\"Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);

  Serial.println(F("DAT: {\"MSG\":\"ATA Receiver Bridge Booted- Beginning operations\"}"));
}


// ** **********************************************************************************
void loop()
{
  if( rf95.available() )
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];  // Max is 250 bytes
    uint8_t len = sizeof(buf);

    //if( rf95.recv(buf, &len) )
    if( myDriver.recv(buf, &len) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      
      buf[len] = '\0';  // Null terminate the string with # bytes copied

      // Emit string without altering, just put a DAT: lead for parsing/errors
      // Serial.print("DAT: ");
      Serial.print("RCV: ");
      Serial.println((char*)buf);

      // Also emit the RSSI of the received packet as JSON
      //Serial.print("DAT: {RSSI: ");
      //Serial.print("RSSI: ");
      //Serial.print(rf95.lastRssi(), DEC);
      //Serial.println("}");

      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      Serial.println("ERR: Receive failed");
    }
  }
}


// ** garbage dump
      //RH_RF95::printBuffer("Received: ", buf, len);  // Prints raw ASCII/byte values
      
      // Send a reply
      //uint8_t data[] = "And hello back to you";
      //rf95.send(data, sizeof(data));
      //rf95.waitPacketSent();
      //Serial.println("Sent a reply");
