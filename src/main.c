#include "main.h"

#include "display/ssd1306.h"
#include "display/gfx.h"
#include "task/blinky.h"
#include "task/runtime_stats.h"
#include "bsp/board.h"

#define SSD_ADDR 0x78

LedBlinky t1 = {500, LL_GPIO_PIN_5};
LedBlinky t2 = {10, LL_GPIO_PIN_5};

int main(void) {
  board_init();

  // settle for a moment before writing to screen
  LL_mDelay(20);

  gfx_init();

  //                   123456789ABCDEF12345
  gfx_write_str("\n\n\n     Starting..\n");
  gfx_display();

  blinky_create_task(&t1);
  blinky_create_task(&t2);
  //runtime_stats_create_task();

  gfx_clear();

  vTaskStartScheduler();

  for (;;) {
    // should never get here.
  }
}


void vApplicationMallocFailedHook( void ) {
  gfx_clear();
  gfx_write_str("Malloc Failed\n");
  gfx_display();
}

// Driver functions for OLED lib
void ssd1306_hard_reset() { }
void ssd1306_write(uint8_t *buffer, uint16_t length) {
  LL_I2C_HandleTransfer(I2C1, SSD_ADDR, LL_I2C_ADDRSLAVE_7BIT, length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

  while(!LL_I2C_IsActiveFlag_STOP(I2C1)) {
    if (LL_I2C_IsActiveFlag_TXIS(I2C1)) {
      LL_I2C_TransmitData8(I2C1, (*buffer++));
    }
  }

  LL_I2C_ClearFlag_STOP(I2C1);
}
