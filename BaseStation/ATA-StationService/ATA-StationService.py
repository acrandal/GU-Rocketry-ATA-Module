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
from serial.tools import list_ports
import sys
from pprint import pprint

class ATASample:
    pass



if __name__ == "__main__":
    print("Starting ATA Station Service")

    serialPortDevName = "/dev/ttyACM0"
    serialPortBaudRate = 115200

    plist = list(list_ports.comports(include_links=True))
    for p in plist:
        print(p.device)

    pprint(plist)

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
