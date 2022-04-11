#!/usr/bin/env python3

import serial
import time

class DataSample:
    def __init__(self, inputFileLine: str, priorFileLine: str):
        self.inputLine = inputFileLine
        self.priorLine = priorFileLine
        self.sampleDelay_ms = 0


class ATASerial(serial.Serial):
    def __init__(self, replay_input_file=None):
        print("ATA Fake serial port init")
        self.last_readline_epoch_ns = time.time_ns()

        self.dataSamples = []

        self.replay_input_file = replay_input_file
        if( self.replay_input_file != None ):
            self.load_replay_input_file(self.replay_input_file)


    def readline(self):
        ret = ""

    def load_replay_input_file(self, filename):
        print(f"Loading replay data file: {self.replay_input_file}")
        with open(filename) as fp:
            priorLine = ""
            for currentLine in fp:
                currentLine = currentLine.strip()
                print(currentLine)

                priorLine = currentLine


if __name__ == "__main__":
    print("Testing serial port")
    millisec = time.time_ns() // 1000000
    print(millisec)

    mockSerial = ATASerial("ATA00051.DAT")


