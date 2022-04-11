# Simple wrapper for command line build & upload
#
# Author: Aaron S. Crandall <crandall@gonzaga.edu>
# Copyright: 2021
# License: GPL v3.0

PORT=/dev/ttyACM1
BAUD=115200
FQBN=adafruit:samd:adafruit_feather_m0
SKETCH=ATA-MainBoard

all: build

build:
	arduino-cli compile -v --fqbn $(FQBN) $(SKETCH)

upload:
	arduino-cli upload -v -p $(PORT) --fqbn $(FQBN) $(SKETCH)
	sleep 1		# To let the arduino HUP before next ops

reload: build upload

install: upload

view:
	screen $(PORT) $(BAUD)

review: build upload view

