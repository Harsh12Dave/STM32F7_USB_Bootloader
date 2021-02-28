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

#define BOOT_LOG(args) printf(KGRN args KWHT)
#define BOOT_ERR(args) printf(KRED args KWHT)
#define APP_LOG(args) printf(args)

#endif /* INC_LOG_H_ */
