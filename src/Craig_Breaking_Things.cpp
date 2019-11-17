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
//Pin Setup
const short endstopZ = 10;
const short endstopX = 9;

const short stepDirX = 2;
const short stepPinX = 3;
const short stepTypeX = 1; //AccelStepper::DRIVER (1) means a stepper driver (with Step and Direction pins).
const short stepDirZ = 4;
const short stepPinZ = 5;
const short stepTypeZ = 1;

AccelStepper stepperX(stepTypeX,stepPinX,stepDirX); //스테퍼 구동방식-1은 step/dir으로 움직이는 방식, step pin , direction pin
AccelStepper stepperZ(stepTypeZ,stepPinZ,stepDirZ);

const int maxTravelX = 4500;
const int maxTravelZ = 50;

unsigned int posX, posY;

unsigned long time;
long TravelX;
int move_finished=1;
long inital_homing=-1;
int dur;

void setup() //endstop이 눌리면 그곳을 초기 home으로 설정하는 셋업
{
  Serial.begin(9600);
  pinMode(endstopX,INPUT_PULLUP);
  delay(5);
   stepperX.setAcceleration(100);
   stepperX.setMaxSpeed(100);

   while (digitalRead(endstopX)){
    stepperX.moveTo(inital_homing);
    inital_homing--;
    stepperX.run();
    Serial.print("Homing... ");
    Serial.println(inital_homing);
    delay(5);
   }

  stepperX.setCurrentPosition(0);
  posX = 0;
  stepperX.setMaxSpeed(400);
  stepperX.setAcceleration(150);

  inital_homing = 1;
  Serial.println("Homing... Complete!");
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

void loop()
{

stepperX.run();

if(stepperX.distanceToGo() == 0){
  stepperX.moveTo(posX + 500);
  posX += 500;
}

Serial.println(posX);

if(digitalRead(endstopX) == 0){
  Serial.println("HALTING!");
  Serial.println(posX);
  while(true){

  }
}

/*
//  stepperX.moveTo(1600); // moveTo로 움직이면 나머지 컨트롤이 잘 안돼서 안씀
  stepperX.setMaxSpeed(200); //천천히 움직이려면 200보다 느려도 됨, max2000
  stepperX.setAcceleration(50); //가본 움직임 가속 (천천히)
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
