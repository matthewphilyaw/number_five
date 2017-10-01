//
// Created by Tommy Philyaw on 9/30/17.
//

#ifndef N5_OUTPUT_PIN_H
#define N5_OUTPUT_PIN_H

#include "ll.h"
#include "base_pin.h"

class OutputPin : public BasePin {
  private:
    bool is_set;
  public:
    OutputPin(
      GPIO_TypeDef * const gpio,
      uint32_t const pin,
      uint32_t const speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
      uint32_t const pull = LL_GPIO_PULL_NO,
      uint32_t const output_type = LL_GPIO_OUTPUT_PUSHPULL) :
      BasePin(gpio, pin, LL_GPIO_MODE_OUTPUT, speed, output_type, pull, LL_GPIO_AF_0),
      is_set(false) {

      this->reset();
    }

    inline void set() {
      LL_GPIO_SetOutputPin(this->gpio, this->pin);
      is_set = true;
    }

    inline void reset() {
      LL_GPIO_ResetOutputPin(this->gpio, this->pin);
      is_set = false;
    }

    inline void toggle() {
      if (is_set) {
        this->reset();
      }
      else {
        this->set();
      }
    }
};


#endif //N5_OUTPUT_PIN_H
