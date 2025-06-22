#include "ls1x_printf.h"
#include "ls1x_uart.h"
#include "Config.h"
#include "ls1x_string.h"


void printBuf(uint8_t *buf,uint32_t len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%x ", buf[i]);
    }
    printf("\r\n");
}

void myputchar(unsigned char chr)
{
    UART_SendData(UART_PRINT, chr);
}

void myputchar2(unsigned char uarttype, unsigned char chr)
{
    if(uarttype == 0)
    {
        Uart0_send(chr);
    }
    else if(uarttype == 1)
    {
        Uart1_send(chr);
    }
    else if(uarttype == 2)
    {
        Uart2_send(chr);

    }
}

uint8_t mygetchar(void)
{
    uint8_t chr;
    chr = UART_ReceiveData(UART_PRINT);
    return chr;
}

int printbase(long v,int w,int base,int sign)
{
    int i,j;
    int c;
    char buf[64];
    unsigned long value;
    if(sign && v<0)
    {
        value = -v;
        myputchar('-');
    }
    else
        value=v;

    for(i=0; value; i++)
    {
        buf[i]=value%base;
        value=value/base;
    }
    for(j=max(w,i); j>0; j--)
    {
        c=j>i?0:buf[j-1];
        myputchar((c<=9)?c+'0':c-0xa+'a');
    }
    return 0;
}

int printbase2(unsigned char uarttype, long v, int w, int base, int sign)
{
    int i, j;
    int c;
    char buf[64];
    unsigned long value;
    if(sign && v < 0)
    {
        value = -v;
        myputchar2(uarttype, '-');
    }
    else
        value = v;

    for(i = 0; value; i++)
    {
        buf[i] = value % base;
        value = value / base;
    }
    for(j = max(w, i); j > 0; j--)
    {
        c = j > i ? 0 : buf[j - 1];
        myputchar2(uarttype, (c <= 9) ? c + '0' : c - 0xa + 'a');
    }
    return 0;
}

int puts(char *s)
{
    char c;
    while((c=*s))
    {
        if(c == '\n') myputchar('\r');
        myputchar(c);
        s++;
    }

    return 0;
}

int puts2(unsigned char uarttype, char *s)
{
    char c;
    while((c = *s))
    {
        if(c == '\n') myputchar2(uarttype, '\r');
        myputchar2(uarttype, c);
        s++;
    }

    return 0;
}

int myprintf(char *fmt,...)
{
    int i;
    char c;
    void **arg;
    void *ap;
    int w;

    __builtin_va_start(ap,fmt);
    arg=ap;
    for(i=0; fmt[i]; i++)
    {
        c=fmt[i];
        if(c=='%')
        {
            w=1;
again:
            switch(fmt[i+1])
            {
                case 's':
                    puts(*arg);
                    arg++;
                    i++;
                    break;
                case 'c':
                    myputchar((long)*arg);
                    arg++;
                    i++;
                    break;
                case 'u':
                    printbase((long)*arg,w,10,0);
                    arg++;
                    i++;
                    break;
                case 'd':
                    printbase((long)*arg,w,10,1);
                    arg++;
                    i++;
                    break;
                case 'o':
                    printbase((long)*arg,w,8,0);
                    arg++;
                    i++;
                    break;
                case 'b':
                    printbase((long)*arg,w,2,0);
                    arg++;
                    i++;
                    break;
                case 'p':
                case 'x':
                    printbase((long)*arg,w,16,0);
                    arg++;
                    i++;
                    break;
                case '%':
                    myputchar('%');
                    i++;
                    break;
                case '0':
                    i++;
                    for(w=0; fmt[i+1]>'0' && fmt[i+1]<='9'; i++)
                        w=w*10+(fmt[i+1]-'0');
                    goto again;
                    break;
                case '1' ... '9':
                    for(w=0; fmt[i+1]>'0' && fmt[i+1]<='9'; i++)
                        w=w*10+(fmt[i+1]-'0');
                    goto again;
                    break;

                default:
                    myputchar('%');
                    break;
            }
        }
        else
        {
            if(c=='\n') myputchar('\r');
            myputchar(c);
        }
    }
    return 0;

}

int myprintf2(unsigned char uarttype, char *fmt, ...)
{
    int i;
    char c;
    void **arg;
    void *ap;
    int w;

    __builtin_va_start(ap, fmt);
    arg = ap;
    for(i = 0; fmt[i]; i++)
    {
        c = fmt[i];
        if(c == '%')
        {
            w = 1;
again:
            switch(fmt[i + 1])
            {
                case 's':
                    puts2(uarttype, *arg);
                    arg++;
                    i++;
                    break;
                case 'c':
                    myputchar2(uarttype, (long)*arg);
                    arg++;
                    i++;
                    break;
                case 'u':
                    printbase2(uarttype, (long)*arg, w, 10, 0);
                    arg++;
                    i++;
                    break;
                case 'd':
                    printbase2(uarttype, (long)*arg, w, 10, 1);
                    arg++;
                    i++;
                    break;
                case 'o':
                    printbase2(uarttype, (long)*arg, w, 8, 0);
                    arg++;
                    i++;
                    break;
                case 'b':
                    printbase2(uarttype, (long)*arg, w, 2, 0);
                    arg++;
                    i++;
                    break;
                case 'p':
                case 'x':
                case 'X':
                    printbase2(uarttype, (long)*arg, w, 16, 0);
                    arg++;
                    i++;
                    break;
                case '%':
                    myputchar2(uarttype, '%');
                    i++;
                    break;
                case '0':
                    i++;
                    for(w = 0; fmt[i + 1] > '0' && fmt[i + 1] <= '9'; i++)
                        w = w * 10 + (fmt[i + 1] - '0');
                    goto again;
                    break;
                case '1' ... '9':
                    for(w = 0; fmt[i + 1] > '0' && fmt[i + 1] <= '9'; i++)
                        w = w * 10 + (fmt[i + 1] - '0');
                    goto again;
                    break;

                default:
                    myputchar2(uarttype, '%');
                    break;
            }

        }
        else
        {

            if(c == '\n') myputchar2(uarttype, '\r');
            myputchar2(uarttype, c);
        }
    }
    return 0;

}

char *vsputs(char *dst, char *s)
{
    uint32_t len = strlen(s);
    strcpy(dst, s);
    return dst + len;
}

char *vsputchar(char *dst, char chr)
{
    dst[0] = chr;
    return ++dst;
}

char *vsprintbase(char *dst, long v, int w, int base, int sign)
{
    int i, j;
    int c;
    char buf[64];
    unsigned long value;
    if(sign && v < 0)
    {
        value = -v;
        dst = vsputchar(dst, '-');
    }
    else
        value = v;

    for(i = 0; value; i++)
    {
        buf[i] = value % base;
        value = value / base;
    }
    for(j = max(w, i); j > 0; j--)
    {
        c = j > i ? 0 : buf[j - 1];
        dst = vsputchar(dst, (c <= 9) ? c + '0' : c - 0xa + 'a');
    }
    return dst;
}

int vsprintf(char *str, const char *format, __builtin_va_list ap)
{
    int i;
    char c;
    int w;
    char *str_old;
    str_old = str;
    for(i = 0; format[i]; i++)
    {
        c = format[i];
        if(c == '%')
        {
            w = 1;
again:
            switch(format[i + 1])
            {
                case 's':
                    str = vsputs(str, __builtin_va_arg(ap, char *));
                    i++;
                    break;
                case 'c':
                    str = vsputchar(str, (char)__builtin_va_arg(ap, int));
                    i++;
                    break;
                case 'u':
                    str = vsprintbase(str, __builtin_va_arg(ap, long), w, 10, 0);
                    i++;
                    break;
                case 'd':
                    str = vsprintbase(str, __builtin_va_arg(ap, long), w, 10, 1);
                    i++;
                    break;
                case 'o':
                    str = vsprintbase(str, __builtin_va_arg(ap, long), w, 8, 0);
                    i++;
                    break;
                case 'b':
                    str = vsprintbase(str, __builtin_va_arg(ap, long), w, 2, 0);
                    i++;
                    break;
                case 'p':
                case 'x':
                    str = vsprintbase(str, __builtin_va_arg(ap, long), w, 16, 0);
                    i++;
                    break;
                case '%':
                    str = vsputchar(str, '%');
                    i++;
                    break;
                case '0':
                    i++;
                    for(w = 0; format[i + 1] > '0' && format[i + 1] <= '9'; i++)
                        w = w * 10 + (format[i + 1] - '0');
                    goto again;
                    break;
                case '1' ... '9':
                    for(w = 0; format[i + 1] > '0' && format[i + 1] <= '9'; i++)
                        w = w * 10 + (format[i + 1] - '0');
                    goto again;
                    break;
                default:
                    str = vsputchar(str, '%');
                    break;
            }

        }
        else
        {
            if(c == '\n')
                str = vsputchar(str, '\r');
            str = vsputchar(str, c);
        }
    }
    str = vsputchar(str, '\0');
    return (int)(str - str_old);
}

int sprintf(char *str, const char *format, ...)
{
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    int result = vsprintf(str, format, ap);
    __builtin_va_end(ap);
    return result;
}

long _rand_next;
int rand()
{
    _rand_next = _rand_next * 1103515245 + 12345;
    return (int) (_rand_next >> 16);
}

