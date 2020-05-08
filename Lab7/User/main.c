#include "stm32f4xx.h"                  
#define STM32f4XX

void init_I2C3(void){
 
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;
     
    // enable APB1 peripheral clock for I2C3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
    // enable clock for SCL and SDA pins

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    /* setup SCL and SDA pins
     * 1. SCL on PA8 and SDA on PC9
    */
     
    // Configure the SCL pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8; // SCL is on PA8
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // set pins to alternate function
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // set GPIO speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // set output to open drain 
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // enable pull up resistors
    GPIO_Init(GPIOA, &GPIO_InitStruct); // initialize GPIOA
     
    // Configure the SDA pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // SDA is on PC9
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // set pins to alternate function
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // set GPIO speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // set output to open drain 
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // enable pull up resistors
    GPIO_Init(GPIOC, &GPIO_InitStruct); // init GPIOC
     
    // Connect I2C3 pins to AF
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3); // SCL
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3); // SDA
     

    // configure I2C3
    I2C_InitStruct.I2C_ClockSpeed = 100000; // 100kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; // I2C mode
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
    I2C_InitStruct.I2C_OwnAddress1 = 0x00; // own address, not relevant in master mode
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable; // disable acknowledge when reading (can be changed later on)
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
    I2C_Init(I2C3, &I2C_InitStruct); // init I2C3
     
    // enable I2C3
    I2C_Cmd(I2C3, ENABLE);
 
}
 

/* Function to Start I2C communications with specific device */
void I2C_start(uint8_t address, uint8_t direction){
    // wait until I2C3 is not busy anymore
    while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY));
    // Send I2C3 START condition
    I2C_GenerateSTART(I2C3, ENABLE);
    // wait for I2C3 --> Slave has acknowledged start condition
    while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));
    // Send slave Address for write
    I2C_Send7bitAddress(I2C3, address, direction);
    /* wait for I2C3 EV6, check if
    * either Slave has acknowledged Master transmitter or
    * Master receiver mode, depending on the transmission
    * direction
    */
    if(direction == I2C_Direction_Transmitter)
        while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    else if(direction == I2C_Direction_Receiver)
        while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

/* Function to write one byte to the I2C device */
void I2C_write(uint8_t data) {
    I2C_SendData(I2C3, data);
    // wait for I2C3 EV8_2 --> byte has been transmitted
    while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
 
/* Function to read the first byte from the I2C device */
uint8_t I2C_read_ack(void){
    // enable acknowledge of recieved data
    I2C_AcknowledgeConfig(I2C3, ENABLE);
    // wait until one byte has been received
    while( !I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2C3);
    return data;
}
 
/* Function to read a single byte from the I2C device */
uint8_t I2C_read_nack(void) {
    // disabe acknowledge of received data
    // nack also generates stop condition after last byte received
    I2C_AcknowledgeConfig(I2C3, DISABLE);
    I2C_GenerateSTOP(I2C3, ENABLE);
    // wait until one byte has been received
    while( !I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED) );
    // read data from I2C data register and return data byte
    uint8_t data = I2C_ReceiveData(I2C3);
    return data;
}
 
/* Function to stop communication with the I2C device */
void I2C_stop(void) {
    // Send I2C3 STOP Condition
    I2C_GenerateSTOP(I2C3, ENABLE);
}

int main() {
		init_I2C3();
		uint8_t data = 0xA6;
	
    I2C_start(0xff, I2C_Direction_Transmitter);
    I2C_write(data);
    I2C_stop();

    I2C_start(0xff, I2C_Direction_Receiver);
    data = I2C_read_nack();
    I2C_stop();
}
