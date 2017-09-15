//
// Created by Tommy Philyaw on 9/15/17.
//

#include "task/display.h"
#include "os.h"
#include "display/gfx.h"

static void WriteToScreen(void *argument);

void display_create_task() {
  xTaskCreate(WriteToScreen, "Write", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

static void WriteToScreen(void *argument) {
  (void) argument;

  uint32_t dt = 0;
  for(;;) {
    gfx_clear();

    uint32_t current_tick = xTaskGetTickCount();

    gfx_writeStr("CT: ");
    gfx_write_uint(current_tick);
    gfx_writeChar('\n');

    gfx_writeStr("DT: ");
    gfx_write_uint(dt);
    gfx_writeChar('\n');

    gfx_writeStr("HP: ");
    gfx_write_uint(xPortGetFreeHeapSize());
    gfx_writeStr("\n");

    gfx_display();

    dt = xTaskGetTickCount() - current_tick;
  }
}

