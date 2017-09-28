//
// Created by Tommy Philyaw on 9/14/17.
//

#include "task/blinky.h"
#include "os.h"
#include "ll.h"

static StaticTask_t buffer;
static StackType_t stack[configMINIMAL_STACK_SIZE];

int blinky_led_factor = 1;

static void LED_Thread(void *argument);

void blinky_create_task(LedBlinky *config) {
  xTaskCreateStatic(LED_Thread, "BLNK", configMINIMAL_STACK_SIZE - 1, config, 2, stack, &buffer);
}

static void LED_Thread(void *argument) {
  LedBlinky *config =  (LedBlinky *)argument;

  uint32_t prevWakeUpTime = xTaskGetTickCount();
  for(;;) {
    LL_GPIO_TogglePin(GPIOA, config->pin);
    vTaskDelayUntil(&prevWakeUpTime, (config->rate + (blinky_led_factor * 10)));
  }
}
