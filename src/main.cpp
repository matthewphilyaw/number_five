#include "ll.h"
#include "os.h"

extern "C" {

#include "display/ssd1306.h"
#include "display/gfx.h"

}

#include "task/blinky_task.h"
#include "task/menu_task.h"
#include "driver/gpio/output_pin.h"
#include "driver/gpio/alternate_pin.h"
#include "driver/timer/encoder.h"
#include "driver/i2c.h"

#define IDLE_STACK_SIZE configMINIMAL_STACK_SIZE
static StaticTask_t idle_task_block;
static StackType_t  idle_task_stack[IDLE_STACK_SIZE];

constexpr uint32_t I2C_TIMING = 0x0004060E;
constexpr uint32_t SSD_ADDR = 0x78;

int main() {
  LL_UTILS_PLLInitTypeDef pll_init_struct = {LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3};
  LL_UTILS_ClkInitTypeDef clk_init_struct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1, LL_RCC_APB2_DIV_1};
  LL_PLL_ConfigSystemClock_HSI(&pll_init_struct, &clk_init_struct);

  // For LL_mDelay - let FreeRtos handle SysTick_Handler
  LL_Init1msTick(SystemCoreClock);

  // Green LED on board
  static OutputPin led(GPIOA, LL_GPIO_PIN_5);

  // Initializer I2C1
  static AlternatePin i2c_1_scl(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_UP, LL_GPIO_OUTPUT_OPENDRAIN);
  static AlternatePin i2c_1_sda(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4, LL_GPIO_SPEED_FREQ_VERY_HIGH, LL_GPIO_PULL_UP, LL_GPIO_OUTPUT_OPENDRAIN);
  static I2C i2c(I2C1, LL_RCC_I2C1_CLKSOURCE_SYSCLK, I2C_TIMING, true);

  // Initialize Encoder
  static AlternatePin q_enc_ch1(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_6);
  static AlternatePin q_enc_ch2(GPIOB, LL_GPIO_PIN_14, LL_GPIO_AF_6);
  static Encoder q_enc(TIM21);

  gfx_init();

  static BlinkyTask<configMINIMAL_STACK_SIZE> btask1("b1", 3, &led, 1000);
  static MenuTask<configMINIMAL_STACK_SIZE> mtask1("m1", 1, &q_enc);

  vTaskStartScheduler();

  for (;;) {
    // never get here
  }
}

extern "C" {

  void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                     StackType_t **ppxIdleTaskStackBuffer,
                                     uint32_t *pulIdleTaskStackSize ) {
    *ppxIdleTaskTCBBuffer = &idle_task_block;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = IDLE_STACK_SIZE;
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
