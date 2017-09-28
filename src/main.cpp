#include "ll.h"
#include "os.h"

extern "C" {

#include "display/ssd1306.h"
#include "display/gfx.h"
#include "task/blinky.h"
#include "task/menu.h"
#include "bsp/board.h"

}

static StaticTask_t idle_task_block;
static StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];

#define N5_USER_RUNTIME_STATS 0
#define SSD_ADDR 0x78

LedBlinky t1 = {10, LL_GPIO_PIN_5};

int main(void) {
  board_init();

  // settle for a moment before writing to screen
  LL_mDelay(20);

  gfx_init();

  //                   123456789ABCDEF12345
  gfx_write_str("\n\n\n     Starting..\n");
  gfx_display();

  blinky_create_task(&t1);
  menu_create_task();

  gfx_clear();

  vTaskStartScheduler();

  for (;;) {
    // should never get here.
  }
}


extern "C" {

  void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                     StackType_t **ppxIdleTaskStackBuffer,
                                     uint32_t *pulIdleTaskStackSize ) {
    *ppxIdleTaskTCBBuffer = &idle_task_block;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  }

  void vApplicationMallocFailedHook(void) {
    gfx_clear();
    gfx_write_str("Malloc Failed\n");
    gfx_display();
  }

  // Driver functions for OLED lib
  void ssd1306_hard_reset() {}
  void ssd1306_write(uint8_t *buffer, uint16_t length) {
    LL_I2C_HandleTransfer(I2C1, SSD_ADDR, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_AUTOEND,
                          LL_I2C_GENERATE_START_WRITE);

    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {
      if (LL_I2C_IsActiveFlag_TXIS(I2C1)) {
        LL_I2C_TransmitData8(I2C1, (*buffer++));
      }
    }

    LL_I2C_ClearFlag_STOP(I2C1);
  }
}
