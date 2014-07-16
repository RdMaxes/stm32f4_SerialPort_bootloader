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

	//Receive a byte from usart2
	key = Usart2_GetByte();
	if (key == 0x31)
	{
	}
	else if (key == 0x32)
	{
	}
	else if (key == 0x33) /* execute the new program */
	{
//		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
//		/* Jump to user application */
//		Jump_To_Application = (pFunction) JumpAddress;
//		/* Initialize user application's Stack Pointer */
//		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
//		Jump_To_Application();
	}
	else if ((key == 0x34) && (FlashProtection == 1))
	{
		/* Disable the write protection */
		switch (FLASH_If_DisableWriteProtection())
		{
			case 1:
			{
				my_printf("\r\nWrite Protection disabled...");
				FlashProtection = 0;
				break;
			}
			case 2:
			{
				my_printf("\r\nError: Flash write unprotection failed...");
				break;
			}
			default:{}
		}
	}
	else
	{
		if (FlashProtection == 0)
		{
			my_printf("\r\nInvalid Number ! ==> The number should be either 1, 2 or 3");
		}
		else
		{
			my_printf("\r\nInvalid Number ! ==> The number should be either 1, 2, 3 or 4");
		}
	}

	while(1) 
	{
		delay(8000000);
		LED_GREEN_ON();
		delay(8000000);
		LED_GREEN_OFF();		
	}

	return 0;
}
