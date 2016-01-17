

////////Bilbiotecas
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

///////Definicoes e macros globais
#define LOCAL_ECHO

#ifdef LOCAL_ECHO
#define kOKResponseString "AT\r\r\nOK\r\n"
#else
#define kOKResponseString "\r\nOK\r\n"
#endif//eu que acrescentei esse end if

#define kATCommandString        "AT\r"
#define kMyErrReturn            -1

enum
{
    kNumRetries = 3
};

static struct termios gOriginalTTYAttrs;

/////////////////////////////////////////////////////////////////////////////////////////


