#include "stm32f4xx.h"                
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "main.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#define  STM32F40_41xxx

uint16_t delay_c = 0; 

void SysTick_Handler(void){
	if(delay_c > 0)
		delay_c--;
}

void delay_ms(uint16_t delay_t)
{
	delay_c = delay_t;
	while(delay_c){};
}

int main(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	SysTick_Config(SystemCoreClock/1000);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/**************************************************
	 *12 - green | 13 - Orange | 14 - Red | 15 - Blue *
	 **************************************************/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  while(1) {
		/*RED*/
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay_ms(1000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		
		/*BLUE*/
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay_ms(1000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);

		/*YELLOW and GREEN*/
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		delay_ms(1000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	}
}
