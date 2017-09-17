//
// Created by Tommy Philyaw on 9/14/17.
//

#include "task/blinky.h"
#include "os.h"
#include "ll.h"

static void LED_Thread(void *argument);

void blinky_create_task(LedBlinky *config) {
  xTaskCreate(LED_Thread, "BLNK", configMINIMAL_STACK_SIZE - 1, config, 2, NULL);
}

static void LED_Thread(void *argument) {
  LedBlinky *config =  (LedBlinky *)argument;

  uint32_t prevWakeUpTime = xTaskGetTickCount();
  for(;;) {
    LL_GPIO_TogglePin(GPIOA, config->pin);
    vTaskDelayUntil(&prevWakeUpTime, config->rate);
  }
}
