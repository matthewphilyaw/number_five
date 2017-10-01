//
// Created by Tommy Philyaw on 9/30/17.
//

#ifndef N5_ENCODER_H
#define N5_ENCODER_H

#include "ll.h"


class Encoder {
  private:
    TIM_TypeDef *timer;

  public:
    explicit Encoder(TIM_TypeDef *timer) :
      timer(timer) {

      if (timer == TIM21) {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);
      }
      else if (timer == TIM22) {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM22);
      }

      LL_TIM_IC_SetActiveInput(timer, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
      LL_TIM_IC_SetActiveInput(timer, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
      LL_TIM_SetEncoderMode(timer, LL_TIM_ENCODERMODE_X4_TI12);

      LL_TIM_EnableCounter(timer);
    }

    inline uint32_t get_count() {
      return LL_TIM_GetCounter(this->timer);
    }

    inline void set_count(uint32_t count) {
      LL_TIM_SetCounter(this->timer, count);
    }

    inline bool get_direction() {
      return LL_TIM_GetDirection(this->timer) == LL_TIM_COUNTERDIRECTION_UP;
    }
};


#endif //N5_ENCODER_H
