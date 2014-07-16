#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"
#include "stmflash.h"	
#include "ymodem.h"


uint8_t FileName[FILE_NAME_LENGTH];//array to store filename of download *.bin


//Delay for a while
//time: delay time
static void delay(int32_t time)
{
	while(time--);
}

int main(void)
{	
	uint8_t FlashProtection = 0;	

	LED_Init();
	Usart2_Init(230400);
	Myprintf_Init(0x00,myputc);
	
	//Show Program Information
	my_printf("\r\n======================================================================");
	my_printf("\r\n=          STM32F40x_41x In-Application Programming (IAP)            =");	
	my_printf("\r\n=                        Author: RdMaxes                             =");
	my_printf("\r\n=                        Version: 1.01                               =");
	my_printf("\r\n======================================================================");
	my_printf("\r\n\r\n");	

	//Test if any sector of Flash memory where user application will be loaded is write protected
	if (STM_FLASH_GetWriteProtectionStatus() == 0)   FlashProtection = 1;
	else FlashProtection = 0;

	//Show Main Menu
	my_printf("\r\n======================================================================");
	my_printf("\r\n========================= Main Menu ==================================");
	my_printf("\r\nDownload Image To the STM32F4xx Internal Flash --------------------[1]");
	my_printf("\r\nUpload Image From the STM32F4xx Internal Flash --------------------[2]");
	my_printf("\r\nExecute The New Program -------------------------------------------[3]");

	if(FlashProtection != 0)
	{
	      my_printf("\r\nExecute The New Program -------------------------------------------[4]");
	}
	my_printf("\r\n======================================================================");	
	my_printf("\r\nPlease select the option...");


	while(1) 
	{
		delay(8000000);
		LED_GREEN_ON();
		delay(8000000);
		LED_GREEN_OFF();		
	}

	return 0;
}
