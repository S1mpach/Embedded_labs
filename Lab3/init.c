#include "init.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#include "osObjects.h"
#include "cmsis_os.h"

// func init
void EXTI0_IRQHandler(void);
void GPIO_Init(void);
void UserButton_Init(void);

void labInit() {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_Init();
	
}

void GPIO_Init() {
	// GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}
