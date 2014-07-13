#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供學習使用，未經作者許可，不得用於其它任何用途
//ALIENTEK戰艦STM32開發板
//STM32 FLASH 驅動代碼	   
//正點原子@ALIENTEK
//技術論壇:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版權所有，盜版必究。
//Copyright(C) 廣州市星翼電子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

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












