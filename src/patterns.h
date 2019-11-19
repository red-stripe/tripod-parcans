#ifndef PATTERNS_H
#define PATTERNS_H
#include "config.h"

unsigned short patternPosZ, patternPosX, patternPosDmx = 0;

int patternZ[][4] = { // speed, acceleration, position
                    {2, 60, 50, maxTravelZ}, {2, 50, 80, 10}, {2, 20, 50, maxTravelZ/2}, {2, 45, 80, 20}
                  };
int patternX[][4] = { // speed, acceleration, position
                    {1, 20, 50, maxTravelX}, {1, 13, 80, 10}, {1, 10, 50, maxTravelX/2}, {1, 30, 80, 20}
                  };

int patternDmx[][5] = { //R, G, B, UV, Dwell time in millis
                      {255, 255, 255, 0, 2600},
                      {255, 255, 255, 0, 3500},
                      {255, 255, 255, 0, 1000},
                      {255, 255, 255, 0, 4000},
                      {255, 255, 255, 0, 1500},
                      {255, 255, 255, 0, 500}
                      };

#endif
