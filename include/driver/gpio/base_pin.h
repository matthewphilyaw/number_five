//
// Created by Tommy Philyaw on 9/30/17.
//

#ifndef N5_PIN_BASE_H
#define N5_PIN_BASE_H

#include <ll.h>

class BasePin {
  protected:
    GPIO_TypeDef * const gpio;
    uint32_t const pin;

  public:
    BasePin(
      GPIO_TypeDef * const gpio,
      uint32_t const pin,
      uint32_t const mode,
      uint32_t const speed,
      uint32_t const output_type,
      uint32_t const pull,
      uint32_t const alternate)
      :
      gpio(gpio),
      pin(pin) {

      uint32_t clock_periph = 0;
      if (gpio == GPIOA) {
        clock_periph = LL_IOP_GRP1_PERIPH_GPIOA;
      }
      else if(gpio == GPIOB) {
        clock_periph = LL_IOP_GRP1_PERIPH_GPIOB;
      }
      else if(gpio == GPIOC) {
        clock_periph = LL_IOP_GRP1_PERIPH_GPIOC;
      }
      else if(gpio == GPIOD) {
        clock_periph = LL_IOP_GRP1_PERIPH_GPIOD;
      }

      // All GPIO are on the same call
      LL_IOP_GRP1_EnableClock(clock_periph);

      LL_GPIO_SetPinMode(gpio, pin, mode);
      LL_GPIO_SetPinPull(gpio, pin, pull);

      if (mode == LL_GPIO_MODE_OUTPUT || mode == LL_GPIO_MODE_ALTERNATE) {
        LL_GPIO_SetPinSpeed(gpio, pin, speed);
      }

      if (mode == LL_GPIO_MODE_ALTERNATE) {
        if (pin >= LL_GPIO_PIN_8) {
          LL_GPIO_SetAFPin_8_15(gpio, pin, alternate);
        } else {
          LL_GPIO_SetAFPin_0_7(gpio, pin, alternate);
        }
      }

      if (mode == LL_GPIO_MODE_OUTPUT || LL_GPIO_MODE_ALTERNATE) {
        LL_GPIO_SetPinOutputType(gpio, pin, output_type);
      }
    }
};


#endif //N5_PIN_BASE_H
