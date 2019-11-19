#ifndef DMX_H
#define DMX_H

#include "config.h"
#include <DmxSimple.h>

void dmxTarget(short, short);
void dmxUpdate();
void dmxWrite(int, int);

#endif
