#include "stm32f4xx.h" 
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define WIDTH 84
#define HEIGHT 48

uint16_t delay_c = 0; 

void LCD5110_init(void);
void LCD5110_display_text(void);
void LCD5110_Write_Byte(unsigned char data, unsigned char isData);
void LCD5110_SCLK(unsigned char isOn);
void LCD5110_CS(unsigned char isOn);
void LCD5110_RESET(unsigned char isOn);
void LCD5110_DC(unsigned char isOn);
void LCD5110_MOSI(unsigned char isOn);
void LCD5110_GPIO_init(void);
void SysTick_Handler(void);
void delay_ms(uint16_t delay_t);
	
int main(void) {
	LCD5110_init();
	LCD5110_display_text();
}

void LCD5110_display_text(void) {
	const unsigned char text[5] = "Hello";
	LCD5110_Write_Byte(0x80, 1);
	const int size = WIDTH * HEIGHT / 8 ;
	const int center = size / 2 - sizeof(text)/2;
	const int end_position = center + sizeof(text);
	
	for (int i = 0, j = 0; i < size; i++) {
		(i >= center && i<= end_position) ? LCD5110_Write_Byte(text[j], 1), j++ : LCD5110_Write_Byte((unsigned char)'0x00', 1) ; 
	}
}

void SysTick_Handler(void){
	if(delay_c > 0)
		delay_c--;
}

void delay_ms(uint16_t delay_t)
{
	delay_c = delay_t;
	while(delay_c){};
}

void LCD5110_init(void) {
	LCD5110_GPIO_init();
	
	LCD5110_DC(1);
	LCD5110_MOSI(1);
	LCD5110_SCLK(1);
	LCD5110_CS(1);
	
	LCD5110_RESET(0);
	delay_ms(10);
	LCD5110_RESET(1);
	
	LCD5110_Write_Byte(0x23, 0);
	LCD5110_Write_Byte(0xC6, 0);
	LCD5110_Write_Byte(0x07, 0);
	LCD5110_Write_Byte(0x13, 0);
	LCD5110_Write_Byte(0x22, 0);

	LCD5110_Write_Byte(0x0C, 0);
}


void LCD5110_GPIO_init(void) {
	GPIO_InitTypeDef GPIOA_Init;
	GPIO_InitTypeDef GPIOC_Init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIOA_Init.GPIO_Pin = GPIO_Pin_8;
	GPIOA_Init.GPIO_Speed = GPIO_Speed_25MHz;
	GPIOA_Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIOA_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOA_Init.GPIO_OType = GPIO_OType_PP;

	GPIOC_Init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIOC_Init.GPIO_Speed = GPIO_Speed_25MHz;
	GPIOC_Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIOC_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOC_Init.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOA, &GPIOA_Init);
	GPIO_Init(GPIOC, &GPIOC_Init);
}

void setBitsGPIO(unsigned char isOn, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	if (isOn) {
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
	else {
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
}

void LCD5110_Write_Byte(unsigned char data, unsigned char isData) {
	LCD5110_CS(0);
	if (isData)	LCD5110_DC(1);
	else LCD5110_DC(0);
	
	for (unsigned char i = 0; i < 8; i++) {
		LCD5110_MOSI(data & 0x80);
		data <<= 1;
		LCD5110_SCLK(0);		
		LCD5110_SCLK(1);
	}
	LCD5110_CS(1);
}

void LCD5110_CS(unsigned char isOn) {
	setBitsGPIO(isOn, GPIOC, GPIO_Pin_9);
}

void LCD5110_RESET(unsigned char isOn) {
	setBitsGPIO(isOn, GPIOC, GPIO_Pin_8);
}

void LCD5110_DC(unsigned char isOn) {
	setBitsGPIO(isOn, GPIOC, GPIO_Pin_7);
}

void LCD5110_MOSI(unsigned char isOn) {
	setBitsGPIO(isOn, GPIOC, GPIO_Pin_6);
}

void LCD5110_SCLK(unsigned char isOn) {
	setBitsGPIO(isOn, GPIOA, GPIO_Pin_8);
}

