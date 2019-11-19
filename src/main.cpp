/*
Made by:
  Yongwoo S & Craig H - 2019
Built using:
  https://github.com/PaulStoffregen/DmxSimple
  https://github.com/waspinator/AccelStepper
  https://platformio.org
  https://atom.io/

Licenced under:
MIT
*/
#include <Arduino.h>
#include <AccelStepper.h>
#include <DmxSimple.h>
#include "config.h"
#include "patterns.h"
#include "stepper.h"
#include "dmx.h"

const char CompileDate[] = __DATE__ " " __TIME__;
const unsigned int fadeSpeed = crossfade/255;;
unsigned long time, dwellTarget;
boolean dwell = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(CompileDate);
  //output setup
  DmxSimple.usePin(dmxPin);
  pinMode(endstopX,INPUT_PULLUP);
  pinMode(endstopZ,INPUT_PULLUP);
  delay(5);

  homeAxis();

}

void loop()
{
 if(calibration == true){
   calibrateTravel();
 } else {
   checkLimit();
   //motorID,  speed, acceleration, position
   moveTo(patternZ[patternPosZ][0], patternZ[patternPosZ][1], patternZ[patternPosZ][2], patternZ[patternPosZ][3]);
   moveTo(patternX[patternPosX][0], patternX[patternPosX][1], patternX[patternPosX][2], patternX[patternPosX][3]);
   if(stepperX.distanceToGo() == 0 && stepperZ.distanceToGo() == 0){
     if(dwell == false){
       dwellTarget = millis();
       dwellTarget += patternDmx[patternPosDmx][4];
       dwell = true;
     }
     if(millis() > dwellTarget){
       dwell = false;
//       if(stepperZ.distanceToGo() == 0){
         if(patternPosZ < 3){ //numper of patterns in array TODO: make this value dynamic
           patternPosZ ++;
         } else {
           patternPosZ = 0;
         }
//        }
//        if(stepperX.distanceToGo() == 0){
          if(patternPosX < 3){ //length of pattern -1 TODO: make this value dynamic
            patternPosX ++;
          } else {
            patternPosX = 0;
          }
//         }
       }
   }
     stepperX.run();
     stepperZ.run();
     //Serial.print("Z ToGO: ");
     unsigned int travelLeftZ = abs(stepperZ.distanceToGo());
//     Serial.println(travelLeftZ);
     if(travelLeftZ < 200){ //number of steps remaining when crossfade starts
       //dmxUpdate();
       dmxTarget(1,255);
       dmxTarget(2,255);
       dmxTarget(3,255);
       dmxTarget(6,0);
     } else {
       //dmxUpdate();
       dmxTarget(1,0);
       dmxTarget(2,0);
       dmxTarget(3,0);
       dmxTarget(6,255);
     }


     if (millis() % fadeSpeed == 0) {
       dmxUpdate();
     }
     delay(1);
 }
}
