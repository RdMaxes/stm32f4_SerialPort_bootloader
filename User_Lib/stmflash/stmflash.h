
#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
/*
//STM32F40x_41x Flash Memory Organization (User Manual PM0081):
//	Sector [0:3]  Block Size = 16 KBytes
//	Sector [4]    Block Size = 64 KBytes
//	Sector [5:11] Block Size = 128 KBytes
//      ***Total: 1024 KBytes
//Reference from ALIENTEK Warship Dev. Board IAP example
//
*/ 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用戶根據自己的需要設置
#define STM32_FLASH_SIZE 512 	 		//所選STM32的FLASH容量大小(單位為K)
#define STM32_FLASH_WREN 1              //使能FLASH寫入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解鎖鍵值
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //讀出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址開始寫入指定長度的數據
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址開始讀取指定長度數據
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//從指定地址開始寫入指定長度的數據
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//從指定地址開始讀出指定長度的數據

//測試寫入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif












