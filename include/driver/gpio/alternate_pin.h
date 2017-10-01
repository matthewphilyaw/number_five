//
// Created by Tommy Philyaw on 9/30/17.
//

#ifndef N5_ALTERNATE_PIN_H
#define N5_ALTERNATE_PIN_H

#include "driver/gpio/base_pin.h"

class AlternatePin : public BasePin {
  public:
    AlternatePin(
      GPIO_TypeDef * const gpio,
      uint32_t const pin,
      uint32_t const alternate_function,
      uint32_t const speed = LL_GPIO_SPEED_LOW,
      uint32_t const pull = LL_GPIO_PULL_NO,
      uint32_t const output_type = LL_GPIO_OUTPUT_PUSHPULL) :
      BasePin(gpio, pin, LL_GPIO_MODE_ALTERNATE, speed, output_type, pull, alternate_function) {
        // EMPTY
    }
};


#endif //N5_ALTERNATE_PIN_H
