#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"
#include "stmflash.h"	
#include "ymodem.h"

//Global Variables
uint8_t FileName[FILE_NAME_LENGTH];//array to store filename of download *.bin
uint8_t buf_1k[1024] ={0};

//Convert an interger to a string
//str: converted string
//intnum: integer waiting for converting
static void Int2Str(uint8_t* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}

//Download User Application into flash
static void Download2Flash(void)
{
	uint8_t Number[10] = "          ";
	int32_t Size = 0;

	my_printf("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	Size = Ymodem_Receive(&buf_1k[0],APPLICATION_ADDRESS);
	if (Size > 0)
	{
		my_printf("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
		my_printf((char*)FileName);
		Int2Str(Number, Size);
		my_printf("\n\r Size: ");
		my_printf((char*)Number);
		my_printf(" Bytes\r\n");
		my_printf("-------------------\n");
	}
	else if (Size == -1)
	{
		my_printf("\n\n\rThe image size is higher than the allowed space memory!\n\r");
	}
	else if (Size == -2)
	{
		my_printf("\n\n\rVerification failed!\n\r");
	}
	else if (Size == -3)
	{
		my_printf("\r\n\nAborted by user.\n\r");
	}
	else
	{
		my_printf("\n\rFailed to receive the file!\n\r");
	}
}
//Delay for a while
//time: delay time
static void delay(int32_t time)
{
	while(time--);
}

int main(void)
{	
	uint8_t FlashProtection = 0;	
	uint8_t cmd = 0;

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

	while(1) 
	{
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
		cmd = Usart2_GetByte();
		if (cmd == '1')
		{
			//Download User Application file into flash			
			Download2Flash();
		}
		else if (cmd == '2')
		{
			my_printf("\r\nCurrently not support Flash Upload!");
		}
		else if (cmd == '3') /* execute the new program */
		{
	//		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
	//		/* Jump to user application */
	//		Jump_To_Application = (pFunction) JumpAddress;
	//		/* Initialize user application's Stack Pointer */
	//		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
	//		Jump_To_Application();
		}
		else if ((cmd == '4') && (FlashProtection == 1))
		{
			/* Disable the write protection */
			switch (STM_FLASH_DisableWriteProtection())
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

		delay(8000000);
		LED_GREEN_ON();
		delay(8000000);
		LED_GREEN_OFF();		
	}

	return 0;
}
