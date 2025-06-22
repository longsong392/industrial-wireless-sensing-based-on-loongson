#ifndef _LS1X_PRINTF_H_
#define _LS1X_PRINTF_H_

#include "test.h"


#define max(a,b) (((a)>(b))?(a):(b))

int sprintf(char *str, const char *format, ...) ;
int 		myprintf(char *fmt,...);

void 		myputchar(unsigned char chr);

INT8U       mygetchar(void);

#endif

