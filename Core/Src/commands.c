#include "commands.h"
#include "fatfs.h"
#include "log.h"
#include "main.h"

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

static char rwtext[100];

unsigned char COMMAND_Test_Write(void);
unsigned char COMMAND_Test_Read(void);

extern unsigned short overflow;
static unsigned char detetcted_flag;
FATFS myUsbFatFS;
FIL myFile;
FIL fileR;
FRESULT res;
UINT byteswritten, bytesread;
extern FATFS fatfs;
extern FIL file;
extern FIL fileR;
extern DIR dir;
extern FILINFO fno;

unsigned char no_usb_detected(void)
{
	if((overflow >= 10000) && (detetcted_flag == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char COMMAND_PREPARE_USB(void)
{
	if(f_mount(&myUsbFatFS, (TCHAR const*)USBH_Path, 0) == FR_OK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
void COMMAND_DOWNLOAD(void)
{
	FILINFO file_info;
	if (f_open(&fileR, DOWNLOAD_FILENAME, FA_READ) == FR_OK)
	{
		detetcted_flag = 1;
		memset(&file_info, 0 , sizeof(file_info));
		if(f_stat(DOWNLOAD_FILENAME, &file_info) == FR_OK)
		{
			if (file_info.fsize > (0x81FFFFF - 0x8100000))
			{
				;
			}
			else
			{
				Erase_Flash();
				COMMAND_ProgramFlashMemory();
				BootAppImage();
				f_close (&fileR);
				f_mount(NULL, (TCHAR const*)USBH_Path, 0);
			}
		}
		else
		{
			;
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


unsigned char COMMAND_Test_Write(void)
{
	if(f_open(&myFile, "TEST.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		return 0;
	}
	else
	{
		sprintf(rwtext, "Hello world from Harsh Dave and Sachin Rajput from Grey Matter Technology .... !!!");
		res = f_write(&myFile, (uint8_t *)rwtext, strlen(rwtext), &byteswritten);
		if((byteswritten == 0) || (res != FR_OK))
		{
			return 0;
		}
	}

	f_close(&myFile);
	return 1;
}


unsigned char COMMAND_Test_Read(void)
{
	if(f_open(&myFile, "TEST.TXT", FA_READ) != FR_OK)
	{
		return 0; //error
	}
	else
	{
		for(uint8_t i=0; i<100; i++)
		{
			res = f_read(&myFile, (uint8_t*)&rwtext[i], 1, &bytesread);
			if(rwtext[i] == 0x00)
			{
				bytesread = i;
				break;
			}
		}
		if(bytesread==0) return 0;

	}

	f_close(&myFile);
	return 1; //success
}
