/*
//@brief :STM flash programming operation
//        (*)STM32F40x_41x IAP flash is excuted in Ymodem_Receive() function
//        (*)Modified from STM AN395 example
//	  (*)Reference: PM0081
//@Author:RdMaxes
//@Data  :2014/07/16
//@Usage :
//	  (*)In header file <stmflash.h> user can define App program location
//	  (*)IAP program location is start from 0x08000000 (sector 0~3, 64KB)
//        (*)APPLICATION_ADDRESS = App program location, default setting is 0x0801000 (sector 4~11, 960KB)
//@Note  :
*/

//Includes
#include "stmflash.h"

//Private Function
static uint32_t GetSector(uint32_t Address);

//unlock Flash for write acess
void STM_FLASH_Init(void)
{ 
  FLASH_Unlock(); 

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}

//This function does an erase of all user flash area
//StartSector: start of user flash area
//return: 0=user flash area successfully erased, 1=error occurred
uint32_t STM_FLASH_Erase(uint32_t StartSector)
{
  uint32_t UserStartSector = FLASH_Sector_1, i = 0;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(StartSector);

  for(i = UserStartSector; i <= FLASH_Sector_11; i += 8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  
  return (0);
}

//This function writes a data buffer in flash (data are 32-bit aligned).
//After writing data buffer, the flash content is checked.
//FlashAddress: start address for writing data buffer
//Data: pointer on data buffer
//DataLength: length of data buffer (unit is 32-bit word)   
//return:
//      0=Data successfully written to Flash memory
//      1=Error occurred while writing data in Flash memory
//      2=Written Data in flash memory is different from expected one
uint32_t STM_FLASH_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
  uint32_t i = 0;

  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
    {
     /* Check the written value */
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
      {
        /* Flash content doesn't match SRAM content */
        return(2);
      }
      /* Increment FLASH destination address */
      *FlashAddress += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (1);
    }
  }
  return (0);
}

//Returns the write protection status of user flash area.
//return: 
//       0=No write protected sectors inside the user flash area
//       1=Some sectors inside the user flash area are write protected
uint16_t STM_FLASH_GetWriteProtectionStatus(void)
{
  uint32_t UserStartSector = FLASH_Sector_1;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(APPLICATION_ADDRESS);

  /* Check if there are write protected sectors inside the user flash area */
  if ((FLASH_OB_GetWRP() >> (UserStartSector/8)) == (0xFFF >> (UserStartSector/8)))
  { /* No write protected sectors inside the user flash area */
    return 1;
  }
  else
  { /* Some sectors inside the user flash area are write protected */
    return 0;
  }
}


//Disables the write protection of user flash area.
//return: 
//       1: Write Protection successfully disabled
//       2: Error: Flash write unprotection failed
uint32_t STM_FLASH_DisableWriteProtection(void)
{
  __IO uint32_t UserStartSector = FLASH_Sector_1, UserWrpSectors = OB_WRP_Sector_1;

  /* Get the sector where start the user flash area */
  UserStartSector = GetSector(APPLICATION_ADDRESS);

  /* Mark all sectors inside the user flash area as non protected */  
  UserWrpSectors = 0xFFF-((1 << (UserStartSector/8))-1);
   
  /* Unlock the Option Bytes */
  FLASH_OB_Unlock();

  /* Disable the write protection for all sectors inside the user flash area */
  FLASH_OB_WRPConfig(UserWrpSectors, DISABLE);

  /* Start the Option Bytes programming process. */  
  if (FLASH_OB_Launch() != FLASH_COMPLETE)
  {
    /* Error: Flash write unprotection failed */
    return (2);
  }

  /* Write Protection successfully disabled */
  return (1);
}


//Gets the sector of a given address for STM32F40x_41x
//Address: Flash address
//return: The sector of a given address
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }
    return sector;
}







