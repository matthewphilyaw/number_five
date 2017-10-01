//
// Created by Tommy Philyaw on 9/30/17.
//

#ifndef N5_I2C_H
#define N5_I2C_H

#include "ll.h"

class I2C {
  private:
    I2C_TypeDef * const i2c;

  public:
    I2C(
      I2C_TypeDef * const i2c,
      uint32_t const clock_source,
      uint32_t const timing,
      bool fast_mode_plus ) :
      i2c(i2c) {

      if (i2c == I2C1) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
      }
      else if (i2c == I2C2) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
      }

      LL_RCC_SetI2CClockSource(clock_source);
      LL_I2C_Disable(i2c);

      LL_I2C_SetTiming(i2c, timing);

      if (fast_mode_plus) {
        if (i2c == I2C1) {
          LL_SYSCFG_EnableFastModePlus(LL_SYSCFG_I2C_FASTMODEPLUS_I2C1);
        }
        else if (i2c == I2C2) {
          LL_SYSCFG_EnableFastModePlus(LL_SYSCFG_I2C_FASTMODEPLUS_I2C2);
        }
      }

      LL_I2C_Enable(i2c);
  }

  void transfer(uint32_t addr, uint8_t *buffer, uint16_t length) {
    LL_I2C_HandleTransfer(
      this->i2c,
      addr,
      LL_I2C_ADDRSLAVE_7BIT,
      length,
      LL_I2C_MODE_AUTOEND,
      LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_STOP(this->i2c)) {
      if (LL_I2C_IsActiveFlag_TXIS(this->i2c)) {
        LL_I2C_TransmitData8(this->i2c, (*buffer++));
      }
    }

    LL_I2C_ClearFlag_STOP(this->i2c);
  }
};


#endif //N5_I2C_H
