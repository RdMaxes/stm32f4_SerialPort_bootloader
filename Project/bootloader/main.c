#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"
#include "stmflash.h"	
#include "ymodem.h"

int32_t test_var = 343422;

static void delay(int32_t time)
{
	while(time--);
}

int main(void)
{	
	LED_Init();
	Usart2_Init(9600);
	Myprintf_Init(0x00,myputc);
	
	while(1) 
	{
		my_printf("Hello, variable value is %d\r\n",test_var);
		delay(8000000);
		LED_GREEN_ON();
		delay(8000000);
		LED_GREEN_OFF();		
	}

	return 0;
}
