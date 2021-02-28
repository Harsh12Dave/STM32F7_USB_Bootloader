#include "bootloader.h"
#include "stm32f7xx_hal.h"
#include "log.h"

void BootAppImage(void)
{
	BOOT_LOG("EXECUTING APPLICATION IMAGE\r\n");
	uint8_t i = 0;
	uint32_t msp_value = *(volatile uint32_t *)0x8100000;
	if(msp_value == 0xFFFFFFFF)
	{
		return;
	}
	uint32_t resest_address = *(volatile uint32_t *)0x8100004;
	__set_MSP(msp_value);
	AppEntry Jump = (void *)resest_address;
	/* Disable all interrupts */
	__disable_irq();
	/* Disable Systick timer */
	SysTick->CTRL = 0;

	/* Set the clock to the default state */
	HAL_RCC_DeInit();
	/* Clear Interrupt Enable Register & Interrupt Pending Register */
	for (i=0;i<5;i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}

	/* Re-enable all interrupts */
	__enable_irq();

	Jump();
}
