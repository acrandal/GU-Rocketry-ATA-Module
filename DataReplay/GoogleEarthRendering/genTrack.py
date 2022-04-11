#!/usr/bin/env python3

import sys
from pprint import pprint



def getGPSLines(inputFilename: str) -> list:
    gpsLines = []

    fileHandle = open(inputFilename, 'r')
    allLines = fileHandle.readlines()

    for line in allLines:
        if line.startswith("GPS"):
            gpsLines.append(line.strip())

    return gpsLines

def dms2dd(degrees, minutes, seconds, direction):
    dd = float(degrees) + float(minutes)/60 + float(seconds)/(60*60);
    if direction == 'W' or direction == 'S':
        dd *= -1
    return dd;


def formatLatitudeGPSGoogle(rawGPS: str) -> str:
    googleLat = ""
    #print(rawGPS)
    direction = rawGPS[-1:]
    rawGPS = rawGPS[:-1]
    degrees = rawGPS[:2]
    minutes = rawGPS[2:]
    seconds = 0
    googleLat = dms2dd(degrees, minutes, seconds, direction)
    return googleLat

def formatLongitudeGPSGoogle(rawGPS: str) -> str:
    googleLon = ""
    #print(rawGPS)
    direction = rawGPS[-1:]
    rawGPS = rawGPS[:-1]
    degrees = rawGPS[:3]
    minutes = rawGPS[3:]
    seconds = 0
    googleLon = dms2dd(degrees, minutes, seconds, direction)
    return googleLon



def retGPSCoordinates(gpsLines: list) -> list:
    gpsCoordinates = []

    for line in gpsLines:
        vals = line.split(',')
        #print(vals)

        lat = vals[2]
        lon = vals[3]
        alt_m = vals[4]

        googleLat = formatLatitudeGPSGoogle(lat)
        #print(googleLat)
        googleLon = formatLongitudeGPSGoogle(lon)
        #print(googleLon)

        gpsCoordinates.append((googleLat, googleLon, alt_m))
    return gpsCoordinates

def emitDataFile(forematterFilename, backmatterFilename, outputFilename, gpsCoordinates):
    outputFilehandle = open(outputFilename, 'w')

    forematterFilehandle = open(forematterFilename, 'r')
    backmatterFilehandle = open(backmatterFilename, 'r')

    for line in forematterFilehandle.readlines():
        outputFilehandle.write(line)
    
    for coordinate in gpsCoordinates:
        if int(coordinate[1]) == 0:
            continue
        line = f"          {coordinate[1]},{coordinate[0]}\n" #, {coordinate[2]}\n"
        outputFilehandle.write(line)

    for line in backmatterFilehandle.readlines():
        outputFilehandle.write(line)

if __name__ == "__main__":
    print("Starting")

    forematterFilename = "forematter.kml"
    backmatterFilename = "backmatter.kml"

    inputFilename = sys.argv[1]
    outputFilename = sys.argv[2]

    print(f"Reading in from: {inputFilename}")
    print(f"Writing out to: {outputFilename}")

    gpsLines = getGPSLines(inputFilename)
    gpsCoordinates = retGPSCoordinates(gpsLines)

    #pprint(gpsCoordinates)

    emitDataFile(forematterFilename, backmatterFilename, outputFilename, gpsCoordinates)


    print("Done")