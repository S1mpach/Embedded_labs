#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "main.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#define  STM32F40_41xxx

int i;
uint16_t isBoosted = 0;
double mul = 1.0;

void EXTI0_IRQHandler() 
{ 
    for (i = 0; i < 2500000; i++); 

    if(!isBoosted){
    	mul += 0.5;
    	if(mul == 2) isBoosted = 1;
    }else{
    	mul -= 0.5;
    	if(mul == 1) isBoosted = 0;
    }		
	EXTI_ClearITPendingBit(EXTI_Line0);
} 

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

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	SysTick_Config(SystemCoreClock/1000);
	
	__enable_irq();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	while (1) {
  		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		delay_ms(200*mul);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		delay_ms(200*mul);
    }	
}
