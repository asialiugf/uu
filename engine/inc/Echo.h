#ifndef UBEE_ECHO_H
#define UBEE_ECHO_H

namespace uBEE
{
typedef struct {
  char who;
  int  what;
} Echo_t;

// who
#define _WEB =    'w'        // web server
#define _DATA =   'd'        // data server
#define _TRADE =  't'        // trade server

//what -- from web server
#define  STT_START           1
#define  STT_STOP            2
#define  STT_RESTART         3
#define  STT_PAUSE           4
#define  STT_CONTINUE        5

//what -- from data server
#define  ONTICK              1
#define  ONBARS              2

//what -- from trade server
#define  ON_RES_BUY_OPEN      1
#define  ON_RES_BUY_CLOSE     1

}

#endif // UBEE_ECHO_H
