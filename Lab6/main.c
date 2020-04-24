#define COMPILE_UNICODE_GRAPHIC
#include "stm32f4xx.h"                  // Device header
#include "unicode.h"

#define  ADDRESS				 0x08000400

extern ICON_PARAMS_T  Icon_params;

extern const unsigned char UNICODE_ID[16];
const unsigned char FIRMWARE_ID[16]  __attribute__((at(ADDRESS))) = {'L','A','B','7',' ','M','E','M','O','R','Y',' ','M','A','P',0x00}; 

int main(void)
{
	unsigned char my_buff[16] = {0};
	
	my_buff[0] = UNICODE_ID[0];
	while (1);
}
