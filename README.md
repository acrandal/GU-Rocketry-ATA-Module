# GU Rocketry ATA Module

ATA: Alternative To Aluminium

Gonzaga University ASME Rocketry Club  
2021-2022 Annual Competition Rocket  
Project for data collection of telemetry onboard

-----

## Devices / Data Sources

The system has several on board sensors and data sources.
These parts are all read for telemetry and environmental data during rocket operations.

### Environmental sensor: Sparkfun Environmental Combo  
https://www.sparkfun.com/products/14348

CCS811 Sensor
- Carbon Dioxide (CO2)
- Total Voltaic Compounds (TVOC)

BME280
- Temperature (C)
- Atmospheric Pressue (Pa)
- Altitude Estimate (m)
- Relative Humidity (%)

-----

### Message formatting

Data is stored on the onboard SD card and transmitted over the packet radio.
Both use the same shorthand format for saving space and bandwidth.

Environmental data packing:  
ENV: millis,CO2,TVOC,C,Pa,Alt(m),Humidity%


Contributors:
- Aaron S. Crandall \<crandall@gonzaga.edu>

Copyright: 2021  
License: GPL v3.0  
