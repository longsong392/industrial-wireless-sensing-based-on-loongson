#ifndef _LS1X_STRING_H_
#define _LS1X_STRING_H_

#include "test.h"

int atoi(char *s);
int atohex(char *str);
int strlen(const char *p);
int strcmp(const char *s1, const char *s2);
int memcmp(const void *cs, const void *ct, int count);

void itoa(char chWord[], int Num);
void *memcpy(void *s1, const void *s2, int n);
void *memset(void *s, int c, int count);

char *strchr(const char *s, char c);
char *strcat(char *dst, const char *src);
char *strcpy(char *dest, const char *src);
char *pstrstr(const char *haystack, const char *needle);
int strncmp(const char *s1, const char *s2, size_t n);
INT8U strstr(const INT8U *str, const INT8U *sub_str, INT8U num);

#endif
