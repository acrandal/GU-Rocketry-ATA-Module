/**
 *  Inertial Measurement Unit Wrapper Library
 * 
 *  Based around a knockoff BNO055 board
 *  https://www.aliexpress.com/item/4000686420656.html?spm=a2g0o.order_list.0.0.21ef1802WCIPwt
 */

#ifndef __ATA_IMU_H
#define __ATA_IMU_H

#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>

// BNO055's are either 0x28 or 0x29
#define BNO055_I2C_ADDR 0x29    // Knockoff boards use 0x29 instead of the default
#define BNO_TYPE 55             // type of BNO sensor (there's 080, and others)

#define ACC_SCALE_4G 0x0D
#define ACC_SCALE_8G 0x0E
#define ACC_SCALE_16G 0x0F

#define RST             11    // BNO RST pin number
#define BNO_ADDR        0x28  // I2C address of first BNO
#define PAGE_ID         0x07  // BNO register: page select
#define ACC_DATA_X_LSB  0x08  // BNO page 0 register: Acceleration Data X LSB
#define GYR_DATA_X_LSB  0x14  // BNO page 0 register: Gyroscope Data X LSB
#define OPR_MODE        0x3D  // BNO page 0 register: Operation Mode <3:0>
#define ACC_CONFIG      0x08  // BNO page 1 register: Accelerometer Config
#define MODE_AMG        0x07  // non-fusion mode with accel/gyro/mag

struct AxisData {
    int16_t x, y, z;
};

// ATA's IMU sensor package
class ATA_IMU {
private:
    bool verbose;
    AxisData accel_data;
    AxisData gyro_data;

    void bno_write(uint8_t i2c_addr, uint8_t reg, uint8_t data)  // write one BNO register
    {
        Wire.beginTransmission(i2c_addr);
        Wire.write(reg);
        Wire.write(data);
        Wire.endTransmission(true);  // send stop
    }

    void bno_read_multiple(uint8_t i2c_addr, uint8_t reg, uint8_t *buf, uint8_t length)  // read multiple BNO registers into buffer
    {
        for (uint32_t n = 0; n < length; n++)
        {
            if ((n & 31) == 0)  // transfer up to 32 bytes at a time
            {
                Wire.beginTransmission(i2c_addr);
                Wire.write(reg+n);
                Wire.endTransmission(false);  // send restart
                Wire.requestFrom(i2c_addr, min(length-n, 32));
            }
            *buf++ = Wire.read();
        }
    }

public:
    void begin() {
        verbose = false;
        Serial.print(F("Initializing IMU / BNO055 Sensor -- "));

        Wire.begin();       // initialize I2C
        delay(800);         // Let BNO power on if you're too fast

        this->bno_write(BNO055_I2C_ADDR, PAGE_ID, 1);               // register page 1
        this->bno_write(BNO055_I2C_ADDR, ACC_CONFIG, ACC_SCALE_8G); // accel 2g range (default value 0x0D)
        this->bno_write(BNO055_I2C_ADDR, PAGE_ID, 0);               // register page 0
        this->bno_write(BNO055_I2C_ADDR, OPR_MODE, MODE_AMG);       // operating mode - set "raw" mode (no fusion)
        delay(10);                                                  // allow time for BNO to switch modes

        Serial.println(F("DONE"));
    }

    void enableVerbose() { verbose = true; }
    void disableVerbose() { verbose = false; }

    void getValues(char * res) {

        // Read in the current accel & gyro values
        bno_read_multiple(BNO055_I2C_ADDR, ACC_DATA_X_LSB, (uint8_t*) &accel_data, sizeof accel_data);  // read registers ACC_DATA_X_LSB through ACC_DATA_Z_MSB
        bno_read_multiple(BNO055_I2C_ADDR, GYR_DATA_X_LSB, (uint8_t*) &gyro_data, sizeof gyro_data);  // read registers GYR_DATA_X_LSB through GYR_DATA_Z_MSB

        if( verbose ) {
            Serial.print("Accelerometer (100 * m/s^2): ");
            Serial.print(accel_data.x);  Serial.print(" ");
            Serial.print(accel_data.y);  Serial.print(" ");
            Serial.print(accel_data.z);  Serial.println("");

            Serial.print("Gyroscope (rps): ");
            Serial.print(gyro_data.x);  Serial.print(" ");
            Serial.print(gyro_data.y);  Serial.print(" ");
            Serial.print(gyro_data.z);  Serial.println("");
        }

        sprintf(res, "IMU: %ld,%d,%d,%d,%d,%d,%d",
                millis(),
                accel_data.x,
                accel_data.y,
                accel_data.z,
                gyro_data.x,
                gyro_data.y,
                gyro_data.z
        );
    }
};


#endif
