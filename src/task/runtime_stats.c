//
// Created by Tommy Philyaw on 9/15/17.
//

#include "os.h"
#include "ll.h"

#include "display/gfx.h"
#include "task/runtime_stats.h"


static void WriteToScreen(void *argument);

char buffer[100];
TaskHandle_t run_time_stat_handle;

void runtime_stats_create_task() {
  xTaskCreate(WriteToScreen, "STAT", 200, NULL, 3, &run_time_stat_handle);
}

/*
 * This will display once counter overflows
 * it's driven by a 16 bit timer at the moment.
 *
 * Need to also ensure the stack is big enough to handle
 * processing the states of the task.
 *
 * Note that the screen attached can only display about 7 lines
 * and the screen will scroll vertically once it passes the 7th
 * line by clearing the screen and writing the 8th line at the top,
 * all of which is handled by the gfx lib
 */
static void WriteToScreen(void *argument) {
  (void) argument;

  // No tasks are freed are created/feed once scheduler starts
  const uint32_t array_size = uxTaskGetNumberOfTasks();

  for(;;) {
    // Wait till counter overflows
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    gfx_clear();
    TaskStatus_t task_array[array_size];
    uint32_t total_ticks = 0;

    uint32_t tCount = uxTaskGetSystemState(task_array, array_size, &total_ticks);

    for (int i = 0; i < tCount; i++) {
      const TaskStatus_t *task = &task_array[i];
      gfx_write_str(task->pcTaskName);
      gfx_write_str("  ");
      gfx_write_uint(task->ulRunTimeCounter);
      gfx_write_char('\n');
    }

    gfx_display();

    vTaskSuspend(NULL);
  }
}

void TIM21_IRQHandler() {
  static uint32_t isr_count = 0;

  isr_count++;

  if (LL_TIM_IsActiveFlag_UPDATE(TIM21) == 1) {
    LL_TIM_ClearFlag_UPDATE(TIM21);
  }

  if (isr_count < 2) {
    return;
  }

  LL_TIM_DisableIT_UPDATE(TIM21);

  BaseType_t higherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(run_time_stat_handle, &higherPriorityTaskWoken);
  portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void runtime_stats_config_timer(void) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

  LL_TIM_SetPrescaler(TIM21, __LL_TIM_CALC_PSC(SystemCoreClock, 100000));
  LL_TIM_SetAutoReload(TIM21, UINT16_MAX -1);

  LL_TIM_EnableIT_UPDATE(TIM21);

  NVIC_SetPriority(TIM21_IRQn, 1);
  NVIC_EnableIRQ(TIM21_IRQn);

  LL_TIM_EnableCounter(TIM21);
  LL_TIM_GenerateEvent_UPDATE(TIM21);
}

uint32_t runtime_stats_get_count(void) {
  return LL_TIM_GetCounter(TIM21);
}
