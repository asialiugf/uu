
#include <see_com_common.h>

static char LOG_FILE_NAME[100]={"./see_errlog"} ;
static int LOG_MODE = 1 ;
static int MAX_BAK_LOG_SIZE = 20*1024*1024 ;

char ca_errmsg[1024] ;

int see_chglog(char cChgCode,char *pcChgStr);
int see_errlog(int iErrCode,char *pcaDispMsg,char cDumpDev,char *pcaDumpAddr,long llen);
void see_disp_errmsg(int iFileId,char *pcaMsgStr,char *pcaDumpAddr,long llen);
void see_mem_dump(int iFileId,char *pucaAddr,long llen);
void see_DumpHex(int ifile,unsigned char *buf, int len) ;
void see_DumpLine(int ifile,int addr, unsigned char *buf, int len) ;

int
see_chglog( char cChgCode, char *pcChgStr)
{
  int iRc = 0 ;
  char g_caMsg[512] ;

  switch(cChgCode){
    case LOG_CHG_LOG:
      iRc = strlen(pcChgStr);
      if((iRc < 1) || (iRc > 100)){
        sprintf(g_caMsg,"see_chglog:Change LOG file name %s error!",pcChgStr);
        see_errlog(1000,g_caMsg,RPT_TO_LOG,0,0);
      }
      strcpy(LOG_FILE_NAME,pcChgStr);
      break;
    case LOG_CHG_MODE:
      switch(pcChgStr[0]){
        case '0':
          LOG_MODE = 0;
          break;
        case '1':
          LOG_MODE = 1;
          break;
        default:
          see_errlog(1000,"see_chglog: Change Mode error!",RPT_TO_LOG,0,0);
      } 
      break;
    case LOG_CHG_SIZE:
      iRc = atoi( pcChgStr );
      MAX_BAK_LOG_SIZE = iRc * 1024;
      break;
    default:
      see_errlog(1000,"see_chglog: Change Code error!",RPT_TO_LOG,0,0);
  }
  return iRc ;
}

int
see_errlog( int iErrCode, char *pcaDispMsg, char cDumpDev, char *pcaDumpAddr, long llen )
{
  int iRc = 0 ;
  int iTtyDev,iUcpErLog,iConsole; 
  int iRetry;
  int iFileSize;
  char caLogBakFileName[80];

  if((LOG_MODE == 0) && ((iErrCode < 999) && (iErrCode > 0))){
    return(0);   
  }

  /* output to logfile */
  if(cDumpDev & RPT_TO_LOG ){
      iRetry=0;
      while((iUcpErLog=open(LOG_FILE_NAME,O_WRONLY | O_CREAT,0777)) == -1){
          sleep(1);
          if ( (iRetry ++) > MAX_OPEN_RETRY) {
              printf("see_errlog.c: fatal error occured !!! error_no is %d\n",errno); 
              printf("errlog: %s CANNOT OPEN FOR ERROR LOG !!!\n",LOG_FILE_NAME);
             /* exit(1);*/
          }
      }

      iRetry=0;
      while (lockf(iUcpErLog,F_LOCK,0) == -1) {
          sleep(1);
          if ( (iRetry ++) > 10) {
              printf("see_errlog.c: fatal error occured !!! error_no is %d\n",errno); 
              printf("errlog:CANNOT LOCK ERROR_LOG %s !!!\n",LOG_FILE_NAME);
              /* exit(1);*/
          }
      }

      if ((iFileSize = lseek(iUcpErLog,0,SEEK_END)) ==-1) {
          printf("see_errlog.c: fatal error occured !!! error_no is %d\n",errno); 
          printf("errlog: %s CANNOT JUMP TO END OF FILE !!!\n",LOG_FILE_NAME);
         /* exit(1);*/
      }

      if (iErrCode == NO_ERR) {
          write(iUcpErLog,"@MSG: ",6);
          write(iUcpErLog,pcaDispMsg,strlen(pcaDispMsg));
          write(iUcpErLog,LOG_MSG_SEPERATE,LOG_SEPERATE_LEN);
      }
      else{
          if ((LOG_MODE == 1) || (iErrCode > 999))
          see_disp_errmsg(iUcpErLog,pcaDispMsg,pcaDumpAddr,llen); 
      }

      if ( iFileSize >= MAX_BAK_LOG_SIZE ) {
          memset(caLogBakFileName, 0, 80);
          strcpy(caLogBakFileName, LOG_FILE_NAME);
          strcat(caLogBakFileName,(char *) ".bak");

          if ( ( iRc = rename(LOG_FILE_NAME, caLogBakFileName) ) < 0 ) {
              printf("see_errlog.c: fatal error occured !!! iRc= %d error_no is %d\n"
                    ,iRc,errno); 
              printf("see_errlog: %s CANNOT BE RENAMED !!!\n",LOG_FILE_NAME);
          }
      }
      close(iUcpErLog); 
  } /* for if (cDumpDev & RPT_TO_LOG )  */

  if (cDumpDev & RPT_TO_TTY ) {    /* output to tty */
    iTtyDev = 1;
      if (iErrCode == NO_ERR) {
        write(iTtyDev,"@MSG: ",6);
        write(iTtyDev,pcaDispMsg,strlen(pcaDispMsg));
        write(iTtyDev,LOG_MSG_SEPERATE,LOG_SEPERATE_LEN);
      }
      else{
        if ((LOG_MODE == 1) || (iErrCode > 999))
          see_disp_errmsg(iTtyDev,pcaDispMsg,pcaDumpAddr,llen);
      }
  }

  if (cDumpDev & RPT_TO_CON ) {    /* output to console */
    if ( (iConsole=open(CONSOLE,O_WRONLY)) == -1 ) {
      printf("see_errlog.c: fatal error occured !!! \n");
      printf("CONSOLE %s CANNOT OPEN FOR ERROR LOG !!!\n",CONSOLE);
         /* exit(1);*/
    }
    else {
      if (iErrCode == NO_ERR) {
        write(iConsole,"@MSG: ",6);
        write(iConsole,pcaDispMsg,strlen(pcaDispMsg));
        write(iConsole,LOG_MSG_SEPERATE,LOG_SEPERATE_LEN);
      }
      else{
        if ((LOG_MODE == 1) || (iErrCode > 999))
          see_disp_errmsg(iConsole,pcaDispMsg,pcaDumpAddr,llen);
      }
      close(iConsole);
    }
  }
  return iRc ;
} 


void
see_disp_errmsg(int iFileId,char *pcaMsgStr,char *pcaDumpAddr,long llen)
{
  long lTLoc;
  long lCurTime;
  char caStrBuf[300];

  (lCurTime) = time(&lTLoc); 
  memset(caStrBuf,'0',300);
  sprintf(caStrBuf,"@@@@=%d|%d|%.24s\nMSG: %s\n",
          getpid(),1000,ctime(&lCurTime),pcaMsgStr);
  write(iFileId,caStrBuf,strlen(caStrBuf));

  /* dump memory to fd */
  if((pcaDumpAddr != 0)&&(llen > 0)){
    // see_mem_dump(iFileId,pcaDumpAddr,llen);
     see_DumpHex(iFileId,(unsigned char *)pcaDumpAddr,llen);
  }
  write(iFileId,"\n",1);
}
  

void
see_mem_dump(int iFileId,char *pucaAddr,long llen)
{
  write(iFileId,pucaAddr,llen);
} /* end of see_mem_dump   */

void see_DumpLine(int ifile,int addr, unsigned char *buf, int len)  
{  
  int i, pos;  
  char line[80+1];  
  
  // Address field  
  pos = sprintf(line, "%08X ", addr);  
  
  // Hex content  
  for (i = 0; i < 16; ++i) {  
    if (i % (16/2) == 0) {  
      line[pos++] = ' '; // Insert a space  
    }  
      
    if (i < len) {  
      pos += sprintf(&line[pos], "%02x ", buf[i]);  
    } else {  
      pos += sprintf(&line[pos], "   ");  
    }  
  }  
  pos += sprintf(&line[pos], "  |");  
  
  // Printable content  
  for (i = 0; i < len; ++i) {  
    line[pos++] = isprint(buf[i]) ? buf[i] : '.';  
  }  
  
  sprintf(&line[pos], "|\n");  
//  fprintf(stderr, "%s", line);  
  write(ifile,line,strlen(line)) ;
  
}  
  
void see_DumpHex(int ifile, unsigned char* buf, int len)  
{  
  int i;  
  for (i = 0; i < (len/16); ++i) {  
    see_DumpLine(ifile, 16*i, &buf[16*i], 16);  
  }  
  // Dump remaining which len is not 16  
  if (len % 16 != 0) {  
    see_DumpLine(ifile, 16*i, &buf[16*i], len % 16);  
  }  
}  

int
see_err_log( char *add, long len, const char *fmt, ... ) 
{
    va_list ap;
    memset(ca_errmsg,'\0',1024);
    va_start(ap, fmt);
    vsprintf(ca_errmsg, fmt, ap);
    va_end(ap);
    see_errlog(1000,ca_errmsg,RPT_TO_LOG,add,len);
    return 0;
}
