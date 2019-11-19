#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"
#include "dmx.h"

AccelStepper stepperX(stepTypeX,stepPinX,stepDirX); //스테퍼 구동방식-1은 step/dir으로 움직이는 방식, step pin , direction pin
AccelStepper stepperZ(stepTypeZ,stepPinZ,stepDirZ);

long inital_homing=-1;
unsigned int posX, posZ;
boolean homedX = false;

void homeAxis()
{
  //TODO: re-add lights during homing -- DONE
  //Home X
  dmxWrite(3, 255);
   stepperX.setAcceleration(100);
   stepperX.setMaxSpeed(100);

   while (digitalRead(endstopX)){
    stepperX.moveTo(inital_homing);
    inital_homing--;
    stepperX.run();
    Serial.print("HomingX... ");
    Serial.println(inital_homing);
    delay(5);
   }

  stepperX.setCurrentPosition(0);
  posX = 0;
  stepperX.setMaxSpeed(400);
  stepperX.setAcceleration(150);


  Serial.println("HomingX... Complete!");
  dmxWrite(3, 0);

  //Home Z
  dmxWrite(1, 255);
  inital_homing = -1;

  delay(5);
   stepperZ.setAcceleration(100);
   stepperZ.setMaxSpeed(100);

   while (digitalRead(endstopZ)){
    stepperZ.moveTo(inital_homing);
    inital_homing--;
    stepperZ.run();
    Serial.print("HomingZ... ");
    Serial.println(inital_homing);
    delay(5);
   }

  stepperZ.setCurrentPosition(0);
  posZ = 0;
  stepperZ.setMaxSpeed(100);
  stepperZ.setAcceleration(100);
  Serial.println("HomingZ... Complete!");
  dmxWrite(1, 0);
}

void moveTo(short motorID, int speed, int acceleration, int targetPosition){
  if(motorID == 1){
   stepperX.moveTo(targetPosition); // moveTo로 움직이면 나머지 컨트롤이 잘 안돼서 안씀
   stepperX.setMaxSpeed(speed); //천천히 움직이려면 200보다 느려도 됨, max2000
   stepperX.setAcceleration(acceleration);
  }
 if(motorID == 2){
  stepperZ.moveTo(targetPosition); // moveTo로 움직이면 나머지 컨트롤이 잘 안돼서 안씀
  stepperZ.setMaxSpeed(speed); //천천히 움직이려면 200보다 느려도 됨, max2000
  stepperZ.setAcceleration(acceleration);
  }
}

void calibrateTravel(){
  if(homedX == false){
    stepperX.run();
  }
  if(stepperX.distanceToGo() == 0 && homedX == false){
    stepperX.moveTo(posX + 500);
    posX += 500;
  }

  Serial.print("PosX = ");
  Serial.print(posX);
  Serial.print("  PosZ = ");
  Serial.println(posZ);

  if(digitalRead(endstopX) == 0 && homedX == false){
    homedX = true;
    Serial.print("HALTING! -- ");
    Serial.println(posX);
  }

  if(homedX == true){

    stepperZ.run();
  }
  if(stepperZ.distanceToGo() == 0 && homedX == true){
    stepperZ.moveTo(posZ + 100);
    posZ += 100;
  }
  if(digitalRead(endstopZ) == 0 && homedX == true){
    Serial.println("HALTING! Z --- END ");
    Serial.println(posZ);
    while(true){
      // Calibration finished, Halt forever
    }
  }
}

void checkLimit(){
  if(digitalRead(endstopZ) == false){
    stepperZ.setCurrentPosition(0);
  }
  if(digitalRead(endstopX) == false){
    stepperX.setCurrentPosition(0);
  }
}

void stepperUpdate()
{
  stepperX.run();
  stepperZ.run();
}

int stepsRemain(int id)
{
  if(id == 1){
    return stepperZ.distanceToGo();
  } else {
    return stepperZ.distanceToGo();
  }

}
