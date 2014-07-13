#include "stmflash.h"
/*
//STM32F40x_41x Flash Memory Organization (User Manual PM0081):
//	Sector [0:3]  Block Size = 16 KBytes
//	Sector [4]    Block Size = 64 KBytes
//	Sector [5:11] Block Size = 128 KBytes
//      ***Total: 1024 KBytes
//Reference from ALIENTEK Warship Dev. Board IAP example
//
*/ 

//Read half-word from assigned address
//faddr: address to read
//return: data read out
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//If write is enabled   
	//Write data without checking
	//WriteAddr: address to write
	//pBuffer: data pointer
	//NumToWrite: data number(16it as unit)  
	void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
	{ 			 		 
		u16 i;
		for(i=0;i<NumToWrite;i++)
		{
			FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
			WriteAddr+=2;//pointer shift 2 bytes
		}  
	} 	 
	void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
	{
		u16 stm_sector_size = 16*1024; //16*1024 = 16kbyte		
		u32 secpos;    //sector number (0~11) for stm32f40x_41x
		u16 secoff;    //offset in sector (16bit)
		u16 secremain; //remaining size in sector (16bit)	   
	 	u16 i;    
		u32 offaddr;   //memory address offset, start from FLSH_BASE address
		if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;
		FLASH_Unlock();					//unlock flash
		offaddr=WriteAddr-STM32_FLASH_BASE;		//calculate the address offset from FLASH_BASE
		
		if((offaddr/(64*1024))==0)//Belong sector 0~3, 16KByte
		{	
			stm_sector_size = 16*1024;
			secpos=offaddr/stm_sector_size;			
		}
		else if ((offaddr/(128*1024))==0)//Belong sector 4, 64KByte
		{
			stm_sector_size = 64*1024;
			secpos = 4;
		}
		else//Belong to sector 5~11, 128KByte
		{
			stm_sector_size = 128*1024; 
			secpos=offaddr/stm_sector_size+4;
		}		
		secoff=(offaddr%stm_sector_size)/2;		//calculate address offset in sector
		secremain=stm_sector_size/2-secoff;		//calculate remain memory size in sector  
		if(NumToWrite<=secremain)secremain=NumToWrite;//it can be written all in this time
		while(1) 
		{	
			//Read out whole sector data				
			STMFLASH_Read(GetSecAddr(secpos)+STM32_FLASH_BASE,STMFLASH_BUF,stm_sector_size/2);
			for(i=0;i<secremain;i++)//check if need to be earased or not
			{
				if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//earase needed 	  
			}
			if(i<secremain)//if earase is needed
			{
				FLASH_ErasePage(GetSecAddr(secpos)+STM32_FLASH_BASE);//earase this sector
				for(i=0;i<secremain;i++)//copy data to buffer
				{
					STMFLASH_BUF[i+secoff]=pBuffer[i];	  
				}
				STMFLASH_Write_NoCheck(GetSecAddr(secpos)+STM32_FLASH_BASE,STMFLASH_BUF,stm_sector_size/2);//write into address
			}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//need no earase 				   
			if(NumToWrite==secremain)break;//finish writing
			else//not yet finish
			{
				secpos++;		//section position shift
				secoff=0;		//section offset set zero
			   	pBuffer+=secremain;  	//data pointer shift
				WriteAddr+=secremain;	//write address shift	   
			   	NumToWrite-=secremain;	//decrease data number to write
				if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=stm_sector_size/2;//writing cannot be finished next time
				else secremain=NumToWrite;//next time the writing is finished
			}	 
		};	
		FLASH_Lock();//unlock flash
	}
#endif

//Read data from address
//ReadAddr: data address to read
//pBuffer: pointer to store data
//NumToRead: number of half-word data to read
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//read out the data
		ReadAddr+=2;//address shift 2 Bytes
	}
}


//Get secotr start address
//SectorPos: number of sector (0~11 for stm32f40x_41x)
//return: start address
//note: see PM0081, table 1
u32 GetSecAddr(u32 SectorPos)
{
	switch(SectorPos)
	{
		case 0: return 0x08000000;
		case 1: return 0x08004000;
		case 2: return 0x08008000;
		case 3: return 0x0800C000;
		case 4: return 0x08010000;
		case 5: return 0x08020000;
		case 6: return 0x08040000;
		case 7: return 0x08060000;
		case 8: return 0x08080000;
		case 9: return 0x080A0000;		 	
		case 10: return 0x08C00000;
		case 11: return 0x08E00000;	
	}
}

//Get secotr size
//SectorPos: number of sector (0~11 for stm32f40x_41x)
//return: sector size (KB)
//note: see PM0081, table 1
u32 GetSecAddr(u32 SectorPos)
{
	switch(SectorPos)
	{
		case 0: return 16;
		case 1: return 16;
		case 2: return 16;
		case 3: return 16;
		case 4: return 64;
		case 5: return 128;
		case 6: return 128;
		case 7: return 128;
		case 8: return 128;
		case 9: return 128;		 	
		case 10: return 128;
		case 11: return 128;	
	}
}











