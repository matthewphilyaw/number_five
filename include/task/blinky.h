//
// Created by Tommy Philyaw on 9/14/17.
//

#ifndef N5_BLINKY_H
#define N5_BLINKY_H

#include "stdint.h"

typedef struct {
  uint32_t rate;
  uint32_t pin;
} LedBlinky;

void blinky_create_task(LedBlinky *config);

#endif //N5_BLINKY_H
