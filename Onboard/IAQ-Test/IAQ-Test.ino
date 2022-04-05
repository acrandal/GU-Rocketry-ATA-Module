


#include <TaskScheduler.h>


#include "ATA_IAQ.h"
#include "ATA_BME280.h"

Scheduler taskScheduler;

ATA_IAQ iaq = ATA_IAQ();
ATA_BME280 bme = ATA_BME280();

char buf[120];

void ataBME280_TaskCallback();
Task taskATA_BME280(100, TASK_FOREVER, &ataBME280_TaskCallback, &taskScheduler, true);

void ataIAQ_TaskCallback();
Task taskATA_IAQ(2000, TASK_FOREVER, &ataIAQ_TaskCallback, &taskScheduler, true);


void setup() {
  Serial.begin(115200);

  iaq.begin();
  //iaq.enableVerbose();

  bme.begin();
  //bme.enableVerbose();

  taskScheduler.startNow();   // Set task scheduling system to start from now (millis())

  Serial.println("Ready?");

}


void loop() {
  taskScheduler.execute();
}


void ataBME280_TaskCallback() {
  bme.getValues(buf);
  Serial.println(buf);
}

void ataIAQ_TaskCallback() {
  iaq.getValues(buf);
  Serial.println(buf);
}
