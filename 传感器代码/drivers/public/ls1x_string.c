#include "ls1x_string.h"
#include "ls1x_printf.h"
/***********************************************************************
function:	比较cs和ct中字节
@param	:	cs		要比较的数据
			ct  	要比较的数据
			count	个数
@other	:    该函数是按字节比较
@return	:	0	相同
			1	不同

@auther :	朱晓宇
@time	:	2020年4月9日

***********************************************************************/

int memcmp(const void *cs, const void *ct, int count)
{
    const unsigned char *su1, *su2;

    for( su1 = cs, su2 = ct; count > 0; ++su1, ++su2, --count)
    {
        if (*su1 != *su2)
        {
            return 1;
        }
    }
    return 0;
}
/***********************************************************************
function:	复制字符串函数
@param	:	s1		目标数据
			s2  	源数据
			n		个数
@other	:    该函数是按字节比较
@return	:	无

@auther :	朱晓宇
@time	:	2020年4月9日

***********************************************************************/

void *memcpy(void *s1, const void *s2, int n)
{
    const unsigned char *psrc = s2;
    volatile unsigned char *pdst = s1;

    while (n-- > 0)
        *pdst++ = *psrc++;
    return s1;
}
/***********************************************************************
function:	字符串设置函数
@param	:	s		要被设置的数据
			c  		设置值
			n		个数
@other	:    该函数是按字节比较
@return	:	无

@auther :	朱晓宇
@time	:	2020年4月9日

***********************************************************************/
void *memset(void *s, int c, int count)
{
    unsigned char *xs = (unsigned char *) s;

    while (count--)
        *xs++ = c;
    return s;
}
/***********************************************************************
function:	判断字符串sub_str是否是str的子串
@param	:	str			字符串
			sub_str		字符串
			num			个数
@other	:    该函数是按字节比较
@return	:	0	不是
			1	是

@auther :	朱晓宇
@time	:	2020年4月9日

***********************************************************************/
INT8U strstr(const INT8U *str, const INT8U *sub_str, INT8U num)
{
    unsigned char cont = 0;
    const unsigned char *p, *q;

    while(*str != '\0')
    {
        cont = 0;
        p    = str;
        q    = sub_str;
        if(*p == *q)
        {

            while(*p && *q && *p == *q)
            {
                p++;
                q++;
                cont += 1;
                if(cont == num)
                    return 1;
            }

        }
        str++;
    }
    return 0;
}
/***********************************************************************
function:	判断字符串needle 是否you haystack的字符串
@param	:	haystack			字符串
			needle		字符串
@other	:    该函数是按字节比较
@return	:	0	没有
			other 相应的字符

@auther :	朱晓宇
@time	:	2020年4月9日

***********************************************************************/
char *pstrstr(const char *haystack, const char *needle)
{
    char *cp = (char *)haystack;
    char *s1, *s2;

    if (!*needle)
        return((char *)haystack);

    while (*cp)
    {
        s1 = cp;
        s2 = (char *)needle;
        while(*s1 && *s2 && !(*s1 - *s2))
        {
            s1++;
            s2++;
        }
        if(!*s2)
            return(cp);
        cp++;
    }
    return NULL;
}


char *strchr(const char *s, char c)
{
    if (NULL == s) return NULL;
    while ('0' != *s && c != *s) s++;
    return ('0' == *s) ? NULL : (char *)s;
}

/***********************************************************************
函数功能:		将字符串转换为整形
@param:
			pstr		需要转换的字符串
@other:     无
@return:
			0	转换失败
			1	成功
***********************************************************************/
int atoi(char *pstr)
{
    int Ret_Integer = 0;
    int Integer_sign = 1;
    if(pstr == NULL)
    {
        return 0;
    }

    if(*pstr == '-')
    {
        Integer_sign = -1;
    }
    if(*pstr == '-' || *pstr == '+')
    {
        pstr++;
    }

    while(*pstr >= '0' && *pstr <= '9')
    {
        Ret_Integer = Ret_Integer * 10 + *pstr - '0';
        pstr++;
    }
    Ret_Integer = Integer_sign * Ret_Integer;

    return Ret_Integer;
}

/***********************************************************************
函数功能:		将整形转换为字符串
@param:
			chWord		保存转换后的数据
			pstr		需要转换的数数值
@other:     无
@return:	无
***********************************************************************/
void itoa( char chWord[], int Num)
{
    int i = 0, j;
    char chTemp;
    if(Num == 0)
    {
        chWord[i] = '0' + Num % 10;
        i++;
    }
    while(Num != 0)
    {
        chWord[i] = '0' + Num % 10;
        i++;
        Num = Num / 10;

    }
    chWord[i] = '\0';
    for(j = 0; j < i / 2; j++)
    {
        chTemp = chWord[j];
        chWord[j] = chWord[i - 1 - j];
        chWord[i - 1 - j] = chTemp;
    }
    return ;
}

/***********************************************************************
函数功能:		将两个字符串连接
@param:
			dst		字符串
			src		字符串
@other:     无
@return:
			返回连接后的字符
***********************************************************************/
char *strcat(char *dst, const char *src)
{
    char *d;

    if (!dst || !src)
        return (dst);

    d = dst;
    for (; *d; d++);
    for (; *src; src++)
        *d++ = *src;
    *d = 0;
    return (dst);
}

/***********************************************************************
函数功能:		字符串比较函数
@param:
			dst		字符串
			src		字符串
@other:     无
@return:
			0	相同
			1	不同
***********************************************************************/
int strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(const unsigned char *)s1 - * (const unsigned char *)--s2);
}

/***********************************************************************
函数功能:		字符串复制
@param:
			dest	目标字符串
			src		源字符串
@other:     无
@return:
			0	转换失败
			1	成功
***********************************************************************/
char *strcpy(char *dest, const char *src)
{
    char *r = dest;
    while((*dest++ = *src++) != '\0');
    return r;
}

/***********************************************************************
函数功能:		计算字符串长度
@param:
			p		要计算的数据
@other:     该函数是按字节比较
@return:	无
***********************************************************************/
int strlen(const char *p)
{
    int len = 0;
    while(*p++)
        len++;
    return len;
}


int isContained(const char *str, char c)
{
    if (str == NULL)
    {
        return 0;
    }
    const char *p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            return 1;
        }
        p++;
    }
    return 0;
}
char *strtok_r(char *str, const char *sep, char **lasts)
{
    char *str_return = NULL;		 //返回的字符串
    const char *ctrl = sep; //分隔符


    if (sep == NULL)
    {
        return NULL;
    }


    if (str == NULL && *lasts == NULL)
    {
        return NULL;
    }


    //将分隔符放入map中
    char map[255] = {0};
    int len = 0;
    while (*ctrl != '\0')
    {
        if (isContained(map, *ctrl))
        {
            continue;
        }
        map[len] = *ctrl;
        len++;
        ctrl++;
    }

    if (str == NULL)
    {
        str_return = *lasts;
    }
    else
    {
        str_return = str;
    }

    //忽略掉字符串中起始部分的分隔符,找到第一个不是分隔符的字符指针
    while(*str_return != '\0')
    {
        if (isContained(map, *str_return))
        {
            str_return++;
            continue;
        }
        break;
    }

    str = str_return;

    //查找第一个分隔符
    while (*str_return)
    {
        if (isContained(map, *str_return))
        {
            *str_return++ = '\0'; //当找到时，把匹配字符填为0，并且把str指向下一位
            break;
        }
        str_return++;
    }
    *lasts = str_return; // 把剩余字符串的指针保存到静态变量last

    if (str == str_return)
    {
        *lasts = NULL;
        return NULL;
    }
    else
    {
        return str;
    }
}

/*
 * 16进制转字符串
 * @param
 * 	dest	转换后输出指针
 * 	src	待转换的输入字符指针
 * 	len	带转换的数据长度
 */
// void hex_to_str(INT8U *dest, INT8U *src, INT8S len)
// {
//     INT32S i;
//     for (i = 0; i < len; i++)
//     {
//         sprintf((INT8S *)dest + i * 2, "%02x", *(src + i));
//     }

// }

/*
 * 字符串转16进制
 * @param
 * 	hex	转换后16进制输出指针
 * 	dsp	待转换的输入字符指针
 * 	len	待转换的数据长度
 */
void str_to_hex(INT8U *hex, INT8U *dsp, INT8S len)
{
    int i, j;
    for (i = 0; i < len / 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            hex[i] |= dsp[i * 8 + j] << (8 - j);
        }
    }
}

/*
 * 16进制转ASCII整型数值
 * @param
 * 	str	待转换的输入字符指针
 * @return
 * 	int	输出ASCII整型数值
 */
int atohex(char *str)
{
    int singleResult = 0;
    int totalResult = 0;
    INT8U i, j;
    INT8U lenth = strlen(str);
    for(i = 0; i < lenth; i++)
    {
        singleResult = str[i];
        if ((singleResult >= '0') && (singleResult <= '9'))
        {
            singleResult = (str[i] - '0');

        }
        else
        {
            if ((singleResult >= 'a') && (singleResult <= 'f'))
            {
                singleResult = (str[i] - 'a') + 10;
            }
            else
            {
                if ((singleResult >= 'A') && (singleResult <= 'F'))
                {
                    singleResult = (str[i] - 'A') + 10;
                }
                else
                {
                    return 0;

                }

            }
        }

        for (j = 0; j < (lenth - 1 - i); j++)
        {
            singleResult = singleResult * 16;
        }
        totalResult += singleResult;
    }
    return totalResult;
}
int strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    // initial
    while (n-- != 0 && *s1 == *s2)
    {
        if (n == 0 || *s1 == '\0')
            break;
        s1++;
        s2++;
    }
    // initial

    // my
	//while (--n && *s1 && *s1 == *s2) // ??????????????\0????????????????? n = 0 ????
	//{
		//s1++;
		//s2++;
    //}
    // my

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}



