//
// Created by Tommy Philyaw on 9/24/17.
//

#include "task/menu.h"
#include "os.h"
#include "ll.h"
#include "display/gfx.h"

static StaticTask_t buffer;
static StackType_t stack[configMINIMAL_STACK_SIZE];

static void menu_thread(void *argument);

void menu_create_task(void) {
  xTaskCreateStatic(menu_thread, "MENU", configMINIMAL_STACK_SIZE, NULL, 2, stack, &buffer);
}

void menu_thread(void *argument) {
  // Set counter to the mid point
  LL_TIM_SetCounter(TIM21, UINT16_MAX >> 1);

  uint32_t prevWakeUpTime = xTaskGetTickCount();
  for(;;) {
    uint32_t count = LL_TIM_GetCounter(TIM21);
    uint32_t dir   = LL_TIM_GetDirection(TIM21);

    gfx_clear();
    if (dir == LL_TIM_COUNTERDIRECTION_UP) {
      gfx_write_str("u: ");
    }
    else if (dir == LL_TIM_COUNTERDIRECTION_DOWN){
      gfx_write_str("d: ");
    }
    else {
      gfx_write_str("x: ");
    }

    gfx_write_uint(count);

    gfx_display();

    vTaskDelayUntil(&prevWakeUpTime, 100);
  }
}

