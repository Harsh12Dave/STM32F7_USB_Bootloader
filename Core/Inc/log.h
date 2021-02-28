#ifndef INC_LOG_H_
#define INC_LOG_H_

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BOOT_LOG(args) BOOTLOADER_LOG(KGRN args KWHT)
#define BOOT_ERR(args) BOOTLOADER_LOG(KRED args KWHT)

#define BOOTLOADER_LOG(f_, ...) printf((f_), ##__VA_ARGS__)
#define APP_LOG(f_, ...) printf((f_), ##__VA_ARGS__)

#endif /* INC_LOG_H_ */
