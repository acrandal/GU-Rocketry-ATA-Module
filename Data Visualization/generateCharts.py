#!/usr/bin/env python3

import plotly.express as px

IMAGES_DIR="images/"
DATA_DIR="datasets/"


# *************************************************************************** #
def generateFlightAltitudes():
    print("Generating flight altitudes")
    FILENAME=DATA_DIR + "2022.05.01-Airplane-FeltsField-PriestLake.DAT"
    CHARTTITLE = "ATA Measured Altitude - GPS Sensor Source"

    CHARTLABELS={
                "x": "Sample time (hh:mm:sec) (UTC)",
                "y": "Altitude (m)",
                }

    IMAGEFILE=IMAGES_DIR + "flightAltitudesGPS.png"

    dataXaxis = []
    dataYaxis = []

    with open(FILENAME, 'r', encoding='utf-8') as infile:
        for line in infile:
            if line.startswith("GPS: "):
                vals = line.split(",")
                if(vals[5] != "0.00"):
                    altitude = float(vals[5])
                    (date, timeval) = vals[2].split("-")

                    dataXaxis.append(timeval)
                    dataYaxis.append(altitude)

    fig = px.line(x=dataXaxis, y=dataYaxis, title=CHARTTITLE, labels=CHARTLABELS)
    fig.write_image(IMAGEFILE)


# ***************************************************************************** #
def generateFlightSpeeds():
    print("Generating flight speeds")
    FILENAME=DATA_DIR + "2022.05.01-Airplane-FeltsField-PriestLake.DAT"
    CHARTTITLE = "ATA Measured Speeds - GPS Sensor Source"

    CHARTLABELS={
                "x": "Sample time (hh:mm:sec) (UTC)",
                "y": "Estimated Speed (m/s)",
                }

    IMAGEFILE=IMAGES_DIR + "flightSpeedsGPS.png"

    dataXaxis = []
    dataYaxis = []

    with open(FILENAME, 'r', encoding='utf-8') as infile:
        for line in infile:
            if line.startswith("GPS: "):
                vals = line.split(",")
                if(vals[5] != "0.00"):  # Filter samples with no GPS lock
                    altitude = float(vals[5])
                    speed = float(vals[6])
                    (date, timeval) = vals[2].split("-")

                    dataXaxis.append(timeval)
                    dataYaxis.append(speed)

    fig = px.line(x=dataXaxis, y=dataYaxis, title=CHARTTITLE, labels=CHARTLABELS)
    fig.write_image(IMAGEFILE)


def generateFlightCharts():
    generateFlightAltitudes()
    generateFlightSpeeds()



# *************************************************************************** #
if __name__ == "__main__":
    print("Starting visualization generation")

    generateFlightCharts()


    print("Done")