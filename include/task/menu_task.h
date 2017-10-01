//
// Created by Tommy Philyaw on 9/24/17.
//

#ifndef N5_MENU_TASK_H
#define N5_MENU_TASK_H

#include "os.h"
#include "static_task.h"
#include "driver/timer/encoder.h"

template<uint32_t STACK_SIZE>
class MenuTask : public StaticTask<STACK_SIZE> {
  private:
    Encoder * const q_enc;

  public:
    MenuTask(char const * name, UBaseType_t priority, Encoder * const q_enc) :
      StaticTask<STACK_SIZE>(name, priority),
      q_enc(q_enc) {
      // empty
    }

    void task() override {
      constexpr uint32_t update_rate = 100;

      uint32_t prev_wake_up_time = xTaskGetTickCount();

      q_enc->set_count(UINT16_MAX >> 1);
      uint32_t prev_count = q_enc->get_count();

      for (;;) {

        uint32_t const count = q_enc->get_count();
        if (prev_count == count) {
          vTaskDelayUntil(&prev_wake_up_time, update_rate);
          continue;
        }

        prev_count = count;

        gfx_clear();
        if (q_enc->get_direction()) {
          gfx_write_str("u: ");
        }
        else {
          gfx_write_str("d: ");
        }

        gfx_write_uint(count);
        gfx_display();

        vTaskDelayUntil(&prev_wake_up_time, update_rate);
      }
    }
};

#endif //N5_MENU_TASK_H
