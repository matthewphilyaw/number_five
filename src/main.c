#include "main.h"

#include "display/ssd1306.h"
#include "display/gfx.h"
#include "task/blinky.h"

#define I2C_TIMING 0x0004060E
#define SSD_ADDR 0x78

static void WriteToScreen(void *argument);

void config_led(void);
void config_i2c(void);

LL_UTILS_PLLInitTypeDef pll_init_struct = {LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3};
LL_UTILS_ClkInitTypeDef clk_init_struct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1, LL_RCC_APB2_DIV_1};

int main(void) {
  LL_PLL_ConfigSystemClock_HSI(&pll_init_struct, &clk_init_struct);
  // For LL_mDelay - let FreeRtos handle SysTick_Handler
  LL_Init1msTick(SystemCoreClock);

  config_led();
  config_i2c();

  LL_mDelay(1000);

  gfx_init();

  //                  123456789ABCDEF12345
  gfx_writeStr("\n\n\n      Starting..    \n");
  gfx_display();

  LL_mDelay(1000);

  gfx_clear();

  LedBlinky t1 = {1000, LL_GPIO_PIN_5};

  blinky_create_task(&t1);
  xTaskCreate(WriteToScreen, "Write", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  xPortStartScheduler();

  for (;;) {
    // should never get here.
  }
}

/*
void SysTick_Handler() {

}
*/


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

void config_led(void) {
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
}

void config_i2c(void) {
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  // Configure I2C1 SCL
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

  // Configure I2C1 SDA
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

  // Enable clock for I2C1
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  // Set clock source for I2C1
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK);

  // Disable I2C1 while configuring
  LL_I2C_Disable(I2C1);

  LL_I2C_SetTiming(I2C1, I2C_TIMING);
  LL_SYSCFG_EnableFastModePlus(LL_SYSCFG_I2C_FASTMODEPLUS_I2C1);

  LL_I2C_Enable(I2C1);
}

void vApplicationMallocFailedHook( void ) {
  gfx_clear();
  gfx_writeStr("Malloc Failed\n");
  gfx_display();
}

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
