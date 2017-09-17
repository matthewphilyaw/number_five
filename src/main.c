#include "main.h"

#include "display/ssd1306.h"
#include "display/gfx.h"
#include "task/blinky.h"
#include "task/runtime_stats.h"

#define I2C_TIMING 0x0004060E
#define SSD_ADDR 0x78

volatile uint32_t idle_time_delta = 0;

void config_led(void);
void config_i2c(void);

LL_UTILS_PLLInitTypeDef pll_init_struct = {LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3};
LL_UTILS_ClkInitTypeDef clk_init_struct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1, LL_RCC_APB2_DIV_1};

LedBlinky t1 = {500, LL_GPIO_PIN_5};
LedBlinky t2 = {10, LL_GPIO_PIN_5};

int main(void) {
  LL_PLL_ConfigSystemClock_HSI(&pll_init_struct, &clk_init_struct);
  // For LL_mDelay - let FreeRtos handle SysTick_Handler
  LL_Init1msTick(SystemCoreClock);

  config_led();
  config_i2c();

  LL_mDelay(1000);

  gfx_init();

  //                  123456789ABCDEF12345
  gfx_write_str("\n\n\n      Starting..    \n");
  gfx_display();

  LL_mDelay(1000);

  gfx_clear();

  blinky_create_task(&t1);
  blinky_create_task(&t2);
  runtime_stats_create_task();

  vTaskStartScheduler();

  for (;;) {
    // should never get here.
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

void main_config_timer(void) {
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

  LL_TIM_SetPrescaler(TIM21, __LL_TIM_CALC_PSC(SystemCoreClock, 100000));
  LL_TIM_SetAutoReload(TIM21, UINT16_MAX -1);

  LL_TIM_EnableCounter(TIM21);
  LL_TIM_GenerateEvent_UPDATE(TIM21);
}

uint32_t main_get_count(void) {
  return LL_TIM_GetCounter(TIM21);
}


void vApplicationMallocFailedHook( void ) {
  gfx_clear();
  gfx_write_str("Malloc Failed\n");
  gfx_display();
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName ) {
  gfx_clear();
  gfx_write_str("StackOverFlow\n");
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

