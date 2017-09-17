//
// Created by Tommy Philyaw on 9/15/17.
//

#include "task/runtime_stats.h"
#include "os.h"
#include "display/gfx.h"

char buffer[100];

static void WriteToScreen(void *argument);

void runtime_stats_create_task() {
  xTaskCreate(WriteToScreen, "STAT", 200, NULL, 1, NULL);
}

/*
 * This will display the stats after total_ticks overflows
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

  uint32_t lastCount = main_get_count();
  for(;;) {
    gfx_clear();
    TaskStatus_t task_array[array_size];
    uint32_t total_ticks = 0;

    uint32_t tCount = uxTaskGetSystemState(task_array, array_size, &total_ticks);

    if (lastCount < total_ticks) {
      lastCount = total_ticks;
      vTaskDelay(10);
      continue;
    }

    for (int i = 0; i < tCount; i++) {
      const TaskStatus_t *task = &task_array[i];
      gfx_write_str(task->pcTaskName);
      gfx_write_str("  ");
      gfx_write_uint(task->ulRunTimeCounter);
      gfx_write_char('\n');
    }

    gfx_display();

    // We have the stats, now just stop running this task.
    vTaskSuspend(NULL);
  }
}
