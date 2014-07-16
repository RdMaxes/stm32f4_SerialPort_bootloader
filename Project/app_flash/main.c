#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"

//Delay for a while
//time: delay time
static void delay(int32_t time)
{
	while(time--);
}

int main(void)
{	
	SCB->VTOR = FLASH_BASE|0x10000; //Interrupt Vector Table Relocation in internal FLASH
	
	LED_Init();
	Usart2_Init(230400);
	Myprintf_Init(0x00,myputc);

	while(1) 
	{
		
		LED_GREEN_ON(); LED_BLUE_ON();LED_RED_ON();LED_ORANGE_ON(); delay(8000000);
		
		LED_GREEN_OFF(); LED_BLUE_OFF(); LED_RED_OFF(); LED_ORANGE_OFF(); delay(8000000);		
	}

	return 0;
}
