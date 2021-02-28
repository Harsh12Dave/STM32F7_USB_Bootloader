/*
 * bootloader.h
 *
 *  Created on: 12-Feb-2021
 *      Author: daveh
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_


typedef void (*AppEntry)(void);
void BootAppImage(void);
#endif /* INC_BOOTLOADER_H_ */
