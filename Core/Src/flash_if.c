#include "flash_if.h"

static FLASH_OBProgramInitTypeDef    OBInit;

HAL_StatusTypeDef Erase_Flash(void)
{
	uint32_t FirstSector = 0, NbOfSectors = 0;
	uint32_t SECTORError = 0;

	FLASH_OBProgramInitTypeDef    OBInit;
	FLASH_EraseInitTypeDef EraseInitStruct;
	HAL_FLASH_Unlock();
	HAL_FLASH_OB_Unlock();
	HAL_FLASHEx_OBGetConfig(&OBInit);
	if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_SINGLE_BANK)
	{
	   ;
	}
	if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_DUAL_BANK)
	{
	   ;
	}
	FirstSector = 8;
	NbOfSectors = 3;
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = FirstSector;
	EraseInitStruct.NbSectors     = NbOfSectors;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
	   ;
	}
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

				len += sizeof(uint32_t);
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
