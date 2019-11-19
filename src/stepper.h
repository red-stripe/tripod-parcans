#include "config.h"
#include <AccelStepper.h>

#ifndef STEPPER_H
#define STEPPER_H

//static PubSubClient mqtt_client(espClient);

//PubSubClient client(espClient);
static AccelStepper stepperX;
static AccelStepper stepperZ;

void homeAxis();
void moveTo(short, int, int, int);
void calibrateTravel();
void checkLimit();

#endif
