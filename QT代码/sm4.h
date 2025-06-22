#ifndef SM4_H
#define SM4_H

#include <stddef.h>

#define SHL(x,n) (((x) & 0xFFFFFFFF) << n)
#define ROTL(x,n) (SHL((x),n) | ((x) >> (32 - n)))
#define SWAP(a,b) { unsigned long t = a; a = b; b = t; t = 0; }

#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i) \
{\
        (n) = ((unsigned long)(b)[(i)]     << 24) \
          | ((unsigned long)(b)[(i) + 1] << 16) \
          | ((unsigned long)(b)[(i) + 2] <<  8) \
          | ((unsigned long)(b)[(i) + 3]      );\
}
#endif

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i) \
{\
        (b)[(i)    ] = (unsigned char) ( (n) >> 24 );\
        (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );\
        (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );\
        (b)[(i) + 3] = (unsigned char) ( (n)       );\
}
#endif

extern const unsigned char SboxTable[16][16];
extern const unsigned long FK[4];
extern const unsigned long CK[32];

void SM4_Enc(const unsigned char* src, unsigned char* dst, size_t* len, unsigned char iv[16], const unsigned char key[16]);
void SM4_Dec(const unsigned char* src, unsigned char* dst, size_t* len, unsigned char iv[16], const unsigned char key[16]);

#endif // SM4_H
