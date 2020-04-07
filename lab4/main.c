#include "Driver_SPI.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include <stdio.h>

ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;

void init(void);
void Touch_Read (uint8_t reg, uint8_t* val);
void readX(void);
void readY(void);
void readZ(void);

int main(void) {
	init();
	uint8_t val;// equal 0x3F
	Touch_Read(0x0F, &val);
	
	while(1){
		readX();
		readY();
		readZ();
	}
}


void init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_UNUSED | ARM_SPI_DATA_BITS(8), 2000000);
}

void Touch_Read (uint8_t reg, uint8_t* val) 
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	reg = reg | 0x80;
	while (SPIdrv->GetStatus().busy); 
  SPIdrv->Send(&reg, 1); 
	while (SPIdrv->GetStatus().busy);
	SPIdrv->Receive(val, 1);
	while (SPIdrv->GetStatus().busy);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
}


void readX(void){
	uint8_t buf[2];
	Touch_Read(0x28, &buf[0]);
	Touch_Read(0x29, &buf[1]);
	float X = (int16_t)((buf[1] << 8) + buf[0]);
	printf(" %f ", X);
}

void readY(void){
	uint8_t buf[2];
	Touch_Read(0x2A, &buf[0]);
	Touch_Read(0x2B, &buf[1]);
	float Y = (int16_t)((buf[1] << 8) + buf[0]);
	printf(" %f ", Y);
}	

void readZ(void){
	uint8_t buf[2];
	Touch_Read(0x2C, &buf[0]);
	Touch_Read(0x2D, &buf[1]);
	float Z = (int16_t)((buf[1] << 8) + buf[0]);
	printf(" %f ", Z);
}	
