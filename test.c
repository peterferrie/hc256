
// test unit for hc-256
// odzhan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hc.h"

// 1. If every byte of the key and IV are with value 0, 
//    then the first 32 bytes of the keystream are given as:
    
uint8_t ct1[]=
{ 0x5b, 0x07, 0x89, 0x85, 0xd8, 0xf6, 0xf3, 0x0d,    
  0x42, 0xc5, 0xc0, 0x2f, 0xa6, 0xb6, 0x79, 0x51,    
  0x53, 0xf0, 0x65, 0x34, 0x80, 0x1f, 0x89, 0xf2,    
  0x4e, 0x74, 0x24, 0x8b, 0x72, 0x0b, 0x48, 0x18 };

// 2. If every byte of the key and IV are with value 0, 
//    except that IV[0] = 1, then the first 32 bytes of the 
//    keystream are given as:
   
uint8_t ct2[]=
{ 0xaf, 0xe2, 0xa2, 0xbf, 0x4f, 0x17, 0xce, 0xe9,    
  0xfe, 0xc2, 0x05, 0x8b, 0xd1, 0xb1, 0x8b, 0xb1,    
  0x5f, 0xc0, 0x42, 0xee, 0x71, 0x2b, 0x31, 0x01,    
  0xdd, 0x50, 0x1f, 0xc6, 0x0b, 0x08, 0x2a, 0x50 };

// 3. If every byte of the key and IV are with value 0, 
//    except that key[0] = 0x55, then the first 32 bytes of the 
//    keystream are given as:
   
uint8_t ct3[]=
{ 0x1c, 0x40, 0x4a, 0xfe, 0x4f, 0xe2, 0x5f, 0xed,    
  0x95, 0x8f, 0x9a, 0xd1, 0xae, 0x36, 0xc0, 0x6f,    
  0x88, 0xa6, 0x5a, 0x3c, 0xc0, 0xab, 0xe2, 0x23,    
  0xae, 0xb3, 0x90, 0x2f, 0x42, 0x0e, 0xd3, 0xa8 };

uint8_t *ct_tbl[3]={ct1, ct2, ct3};

int equ(uint8_t x[], uint8_t y[], int len) {
    return memcmp(x, y, len)==0;
}

void bin2hex(void *in, int len) {
    int i;
    uint8_t *p=(uint8_t*)in;
    
    for (i=0; i<len; i++) {
      if ((i & 7)==0) putchar('\n');
      printf ("%02x, ", p[i]);
    }
    putchar('\n');
}

int main(void)
{
    uint8_t strm[32];
    hc_ctx  c;
    int i;
    
    struct {
      uint8_t key[32];
      uint8_t iv[32];
    } key;
    
    for (i=0; i<3; i++) {
      // zero init stream buffer, iv and key
      memset(key.iv,   0, sizeof(key.iv));
      memset(key.key,  0, sizeof(key.key));
      memset(strm, 0, sizeof(strm));
      
      if (i==1) key.iv[0]  = 1;
      if (i==2) key.key[0] = 0x55;
      
      hc256_setkeyx(&c, &key);
      hc256_cryptx(&c, strm, 32);
    
      printf ("\nHC256 test #%i - %s", (i+1), 
        equ(strm, ct_tbl[i], 32) ? "OK" : "failed");
      bin2hex(strm, 32);
    }
    return 0;
}
