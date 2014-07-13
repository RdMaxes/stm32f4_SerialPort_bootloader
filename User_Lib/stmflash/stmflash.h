#ifndef __STMFLASH_H__
#define __STMFLASH_H__

/*
//STM32F40x_41x Flash Memory Organization (User Manual PM0081):
//	Sector [0:3]  Block Size = 16 KBytes
//	Sector [4]    Block Size = 64 KBytes
//	Sector [5:11] Block Size = 128 KBytes
//      ***Total: 1024 KBytes
//Reference from ALIENTEK Warship Dev. Board IAP example
//
*/ 

//User Defined Area
#define STM32_FLASH_SIZE 1024 	//STM32 Flash Size (KB) 		
#define STM32_FLASH_WREN 1      //Enable Write       

//FLASH Start Address
#define STM32_FLASH_BASE 0x08000000 	//stm32 flash start address

//Function Prototype 			
extern void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		 									   
#endif












