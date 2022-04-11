#!/usr/bin/env python3
#
#   ATA - Alternative To Aluminimum
#   Base Station Service
#    -- Serial to Storage Daemon
#
#   @author Aaron S. Crandall <crandall@gonzaga.edu>
#   @copyright 2022
#   @license GPL v3.0
#

import serial
import sys

class ATASample:
    pass



if __name__ == "__main__":
    print("Starting ATA Station Service")

    serialPortDevName = "/dev/ttyACM0"
    serialPortBaudRate = 115200

    with serial.Serial(serialPortDevName, serialPortBaudRate, timeout=1) as ser:
        try:
            while True:
                line = ser.readline()   # read a '\n' terminated line
                line = str(line, 'utf-8').strip()
                print(line)
        except KeyboardInterrupt:
            print("Received ^C - qutting")

    ser.close()

    print("Ending.")
