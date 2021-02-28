#include "commands.h"
#include "fatfs.h"
#include "log.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define DOWNLOAD_FILENAME          "0:image.bin"
/* Private macros ------------------------------------------------------------*/
extern char USBH_Path[4];  /* USBH Logical drive path */
/* Private variables ---------------------------------------------------------*/
static uint8_t RAM_Buf[BUFFER_SIZE] =
{
		0x00
};


extern FATFS fatfs;
extern FIL file;
extern FIL fileR;
extern DIR dir;
extern FILINFO fno;

void COMMAND_DOWNLOAD(void)
{
	FILINFO file_info;
	/* Open the binary file to be downloaded */
	if (f_open(&fileR, DOWNLOAD_FILENAME, FA_READ) == FR_OK)
	{
		memset(&file_info, 0 , sizeof(file_info));
		if(f_stat(DOWNLOAD_FILENAME, &file_info) == FR_OK)
		{
			if (file_info.fsize > (0x81FFFFF - 0x8100000))
			{
				//Fail_Handler();
			}
			else
			{
				Erase_Flash();
				COMMAND_ProgramFlashMemory();
				dummy();
				printf("Booting image\r\n");
			//	SetupBootAddress(0);
				BootAppImage();
				f_close (&fileR);
				f_mount(NULL, (TCHAR const*)USBH_Path, 0);
			}
		}
		else
		{

			//Fail_Handler();
		}
	}

	return;

}


void COMMAND_JUMP(void)
{
  /* Software reset */
  NVIC_SystemReset();
}

/**
  * @brief  Programs the internal Flash memory
  * @param  None
  * @retval None
  */
void COMMAND_ProgramFlashMemory(void)
{
	uint16_t BytesRead;

	struct flash_write_config wconfig;
    f_read (&fileR, RAM_Buf, BUFFER_SIZE, (void *)&BytesRead);
    /* The read data < "BUFFER_SIZE" Kbyte */
    if (BytesRead < BUFFER_SIZE)
    {
    		BOOT_LOG("TRANSFERING APPLICATION IMAGE\r\n");
			memset(&wconfig, 0, sizeof(wconfig));
			wconfig.address = 0x08100000;
			wconfig.data = RAM_Buf;
			wconfig.data_length = BytesRead;
			Write_To_Flash(&wconfig);
    }
}

void dummy(void)
{
		struct flash_write_config wconfig;
		uint32_t magic_number = 0x12341234;
				memset(&wconfig, 0, sizeof(wconfig));
				wconfig.address = 0x081C0000;
				wconfig.data = (uint8_t *)&magic_number;
				wconfig.data_length = 4;
				Write_To_Flash(&wconfig);
}
