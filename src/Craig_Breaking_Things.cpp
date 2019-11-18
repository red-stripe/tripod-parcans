/*
Made by:

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



AccelStepper stepperX(stepTypeX,stepPinX,stepDirX); //스테퍼 구동방식-1은 step/dir으로 움직이는 방식, step pin , direction pin
AccelStepper stepperZ(stepTypeZ,stepPinZ,stepDirZ);

//Config Settings
//Max steps to travel from home position
const int maxTravelX = 2800;
const int maxTravelZ = 800; //900 steps = 180 degrees

const boolean calibration = false;

unsigned int posX, posZ;
unsigned short patternPosZ, patternPosX = 0;
const int dmxChannels = 6;

unsigned long time;
long TravelX;
int move_finished=1;
long inital_homing=-1;
int dur;
boolean homedX = false;

void setup() //endstop이 눌리면 그곳을 초기 home으로 설정하는 셋업
{
  Serial.begin(9600);
//  DmxSimple.maxChannel(dmxChannels);
  DmxSimple.usePin(dmxPin);
  DmxSimple.write(1, 255);
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
  DmxSimple.write(1, 0);
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

short dmxQue[][3] = { //[channel][current, target]
                      {1,0,0}, //Red
                      {2,0,0}, //Green
                      {3,0,0}, //Blue
                      {4,0,0}, // Yellow
                      {5,0,0}, // Spooky Orange
                      {6,0,0} // UV
};

void dmxTarget(short channel, short value){

  switch(channel) {
    case 1:
      dmxQue[0][2] = value;
      break;
    case 2:
      dmxQue[1][2] = value;
      break;
    case 3:
      dmxQue[2][2] = value;
      break;
    case 4:
      dmxQue[3][2] = value;
      break;
    case 5:
      dmxQue[4][2] = value;
      break;
    case 6:
      dmxQue[5][2] = value;
      break;
    default:
      // somethings gone wrong
      break;
  }

}

void dmxUpdate(){
  for(short channelRefresh = 0; channelRefresh < dmxChannels; channelRefresh++){
    if(dmxQue[channelRefresh][1] != dmxQue[channelRefresh][2]){
      if(dmxQue[channelRefresh][1] > dmxQue[channelRefresh][2]){
        dmxQue[channelRefresh][1] --;
      } else {
        dmxQue[channelRefresh][1] ++;
      }
      DmxSimple.write(dmxQue[channelRefresh][0], dmxQue[channelRefresh][1]);
  //    Serial.print("\t dmxUPdate - C:");
  //    Serial.print(dmxQue[channelRefresh][0]);
  //    Serial.print(" \t V:");
  //    Serial.println(dmxQue[channelRefresh][1]);
    }
  }


}

int patternZ[][4] = { // speed, acceleration, position
                    {2,120, 50, maxTravelZ}, {2,80, 80, 0}, {2, 50, 50, maxTravelZ/2}, {2, 100, 80, 20}
                  };
int patternX[][4] = { // speed, acceleration, position
                    {1, 60, 50, maxTravelX}, {1, 40, 80, 0}, {1, 50, 50, maxTravelX/2}, {1, 60, 80, 20}
                  };

// TODO: fade with acceleration
// TODO: Dwell at end of travel
// TODO: check limit switch

void checkLimit(){
  if(digitalRead(endstopZ) == false){
    stepperZ.setCurrentPosition(0);
    posZ = 0;
  }
  if(digitalRead(endstopX) == false){
    stepperX.setCurrentPosition(0);
    posX = 0;
  }
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
   if(stepperZ.distanceToGo() == 0){
     if(patternPosZ < 3){
       patternPosZ ++;
     } else {
       //TODO:add call to homing function here
       patternPosZ = 0;
     }
    }
    if(stepperX.distanceToGo() == 0){
      if(patternPosX < 3){ //length of pattern -1
        patternPosX ++;
      } else {
        //TODO:add call to homing function here
        patternPosX = 0;
      }
     }
     stepperX.run();
     stepperZ.run();
     //Serial.print("Z ToGO: ");
     unsigned int travelLeftZ = abs(stepperZ.distanceToGo());
//     Serial.println(travelLeftZ);
     if(travelLeftZ < 200){
       dmxUpdate();
       dmxTarget(1,255);
       dmxTarget(2,255);
       dmxTarget(3,255);
       dmxTarget(6,0);
     } else {
       dmxUpdate();
       dmxTarget(1,0);
       dmxTarget(2,0);
       dmxTarget(3,0);
       dmxTarget(6,255);
     }
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
