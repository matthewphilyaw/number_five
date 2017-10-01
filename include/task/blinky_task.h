//
// Created by Tommy Philyaw on 9/14/17.
//

#ifndef N5_BLINKY_H
#define N5_BLINKY_H

#include "os.h"
#include "task/static_task.h"
#include "driver/gpio/output_pin.h"

template<uint32_t STACK_SIZE>
class BlinkyTask : public StaticTask<STACK_SIZE> {
  private:
    OutputPin *led;
    uint16_t rate;

  public:
    BlinkyTask(const char * name, UBaseType_t priority, OutputPin *led, uint16_t rate) :
      StaticTask<STACK_SIZE>(name, priority),
      led(led),
      rate(rate) {

      led->reset();
    }

    void task() override {
      auto prevWakeUpTime = xTaskGetTickCount();
      for (;;) {
        this->led->toggle();
        vTaskDelayUntil(&prevWakeUpTime, this->rate);
      }
    }
};

#endif //N5_BLINKY_H
