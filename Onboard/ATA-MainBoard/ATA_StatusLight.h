/**
 * 
 */

#ifndef __ATA_STATUS_LIGHT_H
#define __ATA_STATUS_LIGHT_H

#include <Adafruit_NeoPixel.h>

#include "PinDefinitions.h"

#define OFF Adafruit_NeoPixel::Color(0,0,0)
#define RED Adafruit_NeoPixel::Color(255,0,0)
#define AMBER Adafruit_NeoPixel::Color(255,191,0)
#define GREEN Adafruit_NeoPixel::Color(0,255,0)
#define PURPLE Adafruit_NeoPixel::Color(204, 88, 99)
#define CYAN Adafruit_NeoPixel::Color(0, 255, 255)


class ATA_StatusLight
{
private:
    Adafruit_NeoPixel * strip;

public:
    void begin() {
        strip = new Adafruit_NeoPixel(STATUS_STRIP_LENGTH,
                                      STATUS_LIGHT_PIN,
                                      NEO_RGB + NEO_KHZ800);

        // Setup the strip and turn it off
        strip->begin();
        strip->show();
        strip->setBrightness(50); 
    }

    void setBooting() { setColor(AMBER); }
    void setOff() { setColor(OFF); }
    void setRed() { setColor(RED); }
    void setNominal() { setColor(GREEN); }
    void setErrorSD() { setColor(PURPLE); }
    void setChecking() { setColor(CYAN); }

    void setColor(uint32_t newColor) {
        strip->setPixelColor(0, newColor);
        strip->show();
    }

};


#endif
