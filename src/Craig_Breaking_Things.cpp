// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@open.com.au)
// Copyright (C) 2009 Mike McCauley
// $Id: HRFMessage.h,v 1.1 2009/08/15 05:32:58 mikem Exp mikem $

/*
 * Hi, This is some test code.
 * Limit switches are DISABLED!
 * Please start the lights in 'neutral position'
 *
 */
#include <Arduino.h>
#include <AccelStepper.h>
#include <DmxSimple.h>

//Pin Setup
const short endstopZ = 10;
const short endstopX = 9;
const short dmxPin = 8;
const short stepDirX = 2;
const short stepPinX = 3;
const short stepTypeX = 1; //AccelStepper::DRIVER (1) means a stepper driver (with Step and Direction pins).
const short stepDirZ = 4;
const short stepPinZ = 5;
const short stepTypeZ = 1;

AccelStepper stepperX(stepTypeX,stepPinX,stepDirX); //스테퍼 구동방식-1은 step/dir으로 움직이는 방식, step pin , direction pin
AccelStepper stepperZ(stepTypeZ,stepPinZ,stepDirZ);

const int maxTravelX = 4500;
const int maxTravelZ = 1000; //900 steps = 180 degrees

const boolean calibration = false;

unsigned int posX, posZ;
unsigned short patternPosZ, patternPosX = 0;

unsigned long time;
long TravelX;
int move_finished=1;
long inital_homing=-1;
int dur;
boolean homedX = false;

void setup() //endstop이 눌리면 그곳을 초기 home으로 설정하는 셋업
{
  Serial.begin(9600);
  DmxSimple.usePin(dmxPin);
  pinMode(endstopX,INPUT_PULLUP);
  delay(5);
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

    inital_homing = -1;
    pinMode(endstopZ,INPUT_PULLUP);
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
    delay(2000);
  /*
  while (!digitalRead(endstop1)){
    stepperX.moveTo(initial_homing);
    stepperX.run();
    initial_homing++;
    delay(5);
  }

  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);

  delay(1000);
  */

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

      }
}


}

int patternZ[][4] = { // speed, acceleration, position
                    {2,180, 50, maxTravelZ}, {2,120, 80, 0}, {2, 50, 50, maxTravelZ/2}, {2, 180, 80, 20}
                  };
int patternX[][4] = { // speed, acceleration, position
                    {1, 180, 50, maxTravelX}, {1, 120, 80, 0}, {1, 50, 50, maxTravelX/2}, {1, 180, 80, 20}
                  };

void loop()
{
 if(calibration == true){
   calibrateTravel();
 } else {
   delay(100);

   delay(100);


   moveTo(patternZ[patternPosZ][0], patternZ[patternPosZ][1], patternZ[patternPosZ][2], patternZ[patternPosZ][3]);
   moveTo(patternX[patternPosX][0], patternX[patternPosX][1], patternX[patternPosX][2], patternX[patternPosX][3]);
   if(stepperZ.distanceToGo() == 0){
     if(patternPosZ < 3){
       patternPosZ ++;
       DmxSimple.write(5, 255);
     } else {
       //TODO:add call to homing function here
       patternPosZ = 0;
       DmxSimple.write(5, 0);
     }
    }
    if(stepperX.distanceToGo() == 0){
      if(patternPosX < 3){ //length of pattern -1
        patternPosX ++;
        DmxSimple.write(6, 255);
      } else {
        //TODO:add call to homing function here
        patternPosX = 0;
        DmxSimple.write(6, 0);
      }
     }
     stepperX.run();
     stepperZ.run();
     /*
     stepperX.runToNewPosition(1000); //포지션 1000으로 이동 - 정확히 단위는 모르겠음
     stepperX.setAcceleration(0.1);  //아주 천천히 가속하도록 해서 멈추는 효과를 줌 소수점도 가능
     stepperX.runToNewPosition(1010);  //포지션 1000에서 1010까지 0.1의 가속으로 이동

   // stepperX.moveTo(0);
     stepperX.setMaxSpeed(200);
     stepperX.setAcceleration(50);
     stepperX.runToNewPosition(0);
     stepperX.setAcceleration(0.05);
     stepperX.runToNewPosition(-10);
     */
 }
}
