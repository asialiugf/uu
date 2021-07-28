#ifndef UBEE_ERRLOG_H
#define UBEE_ERRLOG_H

#include <string>

namespace uBEE
{
extern char ca_errmsg[];
int ErrLog(int iErrCode,const char *message,char cDumpDev,const char *buf,long length);
int CheckDate(int year, int month, int day);
std::string GetCallback();

#define ERR_MSG_LEN         2048

#define  RPT_TO_LOG	    	1          /* 00000001 */
#define  RPT_TO_TTY	    	2          /* 00000010 */
#define  RPT_TO_CON   		4          /* 00000100 */
#define  RPT_TO_TTY_DUMP	8          /* 00001000 */

//#define  TTY_DEV        ttyname(1)
#define  CONSOLE        "/dev/console"
#define  LOG_MSG_SEPERATE	"\n"
#define  LOG_SEPERATE_LEN	1
//#define  LOG_NO_OF_ITEM	6

//#//define  MAX_PRT_LEN	256
//#define  MAX_LEVEL	40
//#define  MAX_PARA	30	/* max. number of parameters */
#define  MAX_OPEN_RETRY	256	/* max retry open log file */

#define  NO_ERR		0
//#define  RCVR_ERR	10000
//#define  NORCVR_CHK_ERR	40000
//#define  LOG_CHGCODE_ERROR	1
//#define  LOG_CHGMODE_ERROR	2
//#define  LOG_CHGLOG_ERROR	3

} // namespace

#endif // UBEE_ERRLOG_H
