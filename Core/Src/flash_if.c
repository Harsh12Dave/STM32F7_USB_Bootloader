/*
 * flash_if.c
 *
 *  Created on: Feb 7, 2021
 *      Author: daveh
 */

#include "flash_if.h"

static FLASH_OBProgramInitTypeDef    OBInit;

HAL_StatusTypeDef Erase_Flash(void)
{
	uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t Address = 0, SECTORError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;
FLASH_OBProgramInitTypeDef    OBInit;
	/*Variable used for Erase procedure*/
	FLASH_EraseInitTypeDef EraseInitStruct;
	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();
	   /* Allow Access to option bytes sector */
	   HAL_FLASH_OB_Unlock();
	   /* Get the Dual bank configuration status */
	   HAL_FLASHEx_OBGetConfig(&OBInit);
	   /* Turn on LED3 if FLASH is configured in Dual Bank mode */
	   if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_SINGLE_BANK)
	   {
		   ;
	   }
	   /* Turn on LED3 if FLASH is configured in Single Bank mode */
	   if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_DUAL_BANK)
	   {
		   ;
	   }

	   /* Erase the user Flash area
	     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	   /* Get the 1st sector to erase */
	   FirstSector = 8;//GetSector(FLASH_USER_START_ADDR);
	   /* Get the number of sector to erase from 1st sector*/
	   NbOfSectors = 3;//GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	   EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	   EraseInitStruct.Sector        = FirstSector;
	   EraseInitStruct.NbSectors     = NbOfSectors;

	   /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	      you have to make sure that these data are rewritten before they are accessed during code
	      execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	      DCRST and ICRST bits in the FLASH_CR register. */
	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	   {
		   ;
	   }
	   /* Allow Access to option bytes sector */
	   HAL_FLASH_OB_Lock();
	   HAL_FLASH_Lock();
	return HAL_OK;
}


HAL_StatusTypeDef Write_To_Flash(struct flash_write_config * configuration)
{
	HAL_StatusTypeDef flashRetStatus = HAL_OK;
	uint32_t len = 0;
	uint32_t data = 0;
	uint8_t data_filler = 0;
	if(configuration)
	{

		if(Erase_Flash() == HAL_OK)
		{
			HAL_FLASH_Unlock();
			while(len < configuration->data_length)
			{
				memcpy(&data, configuration->data, 4);

				len += sizeof(uint32_t); //len raised by 4 // 16
				configuration->data = configuration->data + 4;
				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, configuration->address, data) == HAL_OK)
				{
					configuration->address += 4;
				}
				else
				{
					printf("flash write error\r\n");
					flashRetStatus = HAL_ERROR;
				}
			}
		}
		else
		{
			flashRetStatus = HAL_ERROR;
		}
	}
	else
	{
		flashRetStatus = HAL_ERROR;
	}

	HAL_FLASH_Lock();

	return flashRetStatus;
}


void SetupBootAddress(int addr)
{
    HAL_FLASH_Unlock();

    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();

    /* Switch Boot bank: the BootAddrx value "0x2040" corresponds to bank2 address: "0x08100000" */

    if(addr == 0)
    {
    	OBInit.OptionType = OPTIONBYTE_BOOTADDR_0 | OPTIONBYTE_BOOTADDR_1;
 	    OBInit.BootAddr0  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08000000);
    	OBInit.BootAddr1  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08000000);
    }
    else
    {
       	OBInit.OptionType = OPTIONBYTE_BOOTADDR_0 | OPTIONBYTE_BOOTADDR_1;
     	OBInit.BootAddr0  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08100000);
        OBInit.BootAddr1  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08100000);
    }

    if(HAL_FLASHEx_OBProgram(&OBInit) != HAL_OK)
    {
    	;
    }

    /* Start the Option Bytes programming process */
    if (HAL_FLASH_OB_Launch() != HAL_OK)
    {
    	;
    }

    /* Prevent Access to option bytes sector */
    HAL_FLASH_OB_Lock();

    /* Disable the Flash option control register access (recommended to protect
    the option Bytes against possible unwanted operations) */
    HAL_FLASH_Lock();
}

int ImageThereorNot(void)
{
	int ret = 0;
	if(*(uint32_t *)0x081C0000 == 0x12341234)
	{
		ret = 1;
	}

	return ret;
}

