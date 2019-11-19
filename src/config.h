#ifndef CONFIG_H
#define CONFIG_H

  #include <Arduino.h>
  //Pin Setup
  const short endstopZ = 10;
  const short endstopX = 9;
  const short dmxPin = 8;
  const short stepDirX = 2;
  const short stepPinX = 3; //motorID 1
  const short stepTypeX = 1; //AccelStepper::DRIVER (1) means a stepper driver (with Step and Direction pins).
  const short stepDirZ = 4;
  const short stepPinZ = 5; //motorID 2
  const short stepTypeZ = 1;

  //Hardware specific config
  const int dmxChannels = 6;

  //Run mode
  const boolean calibration = false;

  //Asthetic Settings
  //Max steps to travel from home position
  const int maxTravelX = 2600; // A: 2600 B: 1500
  const int maxTravelZ = 800; // A: 800 B: 700

  //Max crossfade time in millis
  const unsigned int crossfade = 5000;

#endif
