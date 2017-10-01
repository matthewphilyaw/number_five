//
// Created by Tommy Philyaw on 9/29/17.
//

#ifndef N5_TASKCPP_H
#define N5_TASKCPP_H

#include "os.h"

template<uint32_t STACK_SIZE>
class StaticTask {
  private:
    StaticTask_t tcb;
    StackType_t thread_stack[STACK_SIZE];

  public:
    StaticTask(char const * name, UBaseType_t priority) {
      xTaskCreateStatic(&task_fun, name, STACK_SIZE, this, priority, this->thread_stack, &this->tcb);
    }

    virtual void task() = 0;

    static void task_fun(void *param) {
      static_cast<StaticTask<STACK_SIZE> *>(param)->task();

      for(;;) {
        // should never reach this code.
      }
    }
};

#endif //N5_TASKCPP_H
