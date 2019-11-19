#include <Arduino.h>
#include "config.h"
#include <DmxSimple.h>

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
    }
  }
}

void dmxWrite(int id, int value)
{
  DmxSimple.write(id, value);
}
