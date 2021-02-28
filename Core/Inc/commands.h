/*
 * commands.h
 *
 *  Created on: Feb 7, 2021
 *      Author: daveh
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

/* Defie to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMAND_H
#define _COMMAND_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "fatfs.h"

#define BUFFER_SIZE (512*64)

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void COMMAND_UPLOAD(void);
void COMMAND_DOWNLOAD(void);
void COMMAND_JUMP(void);
void COMMAND_ProgramFlashMemory(void);
void dummy(void);

#ifdef __cplusplus
}
#endif

#endif  /* _COMMAND_H */


#endif /* INC_COMMANDS_H_ */
