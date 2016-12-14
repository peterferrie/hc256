/**
  Copyright © 2016 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include "hc.h"

// key stream generation function
uint32_t hc256_generate(hc_ctx* c)
{
    uint32_t r, i, i3, i10, i12, i1023;
    uint32_t *x0, *x1;
    uint32_t w0, w1, w2, w3, t;
    
    t=c->ctr;
    
    c->ctr = (c->ctr+1) & 0x7ff;
    
    x0=c->P;
    x1=c->Q;
    
    if (t > 0x3ff) {
      x0=c->Q;
      x1=c->P;
    }

    i     = t          & 0x3ff;
    i3    = (i - 3)    & 0x3ff;
    i10   = (i - 10)   & 0x3ff;
    i1023 = (i - 1023) & 0x3ff;

    x0[i] += x0[i10] + 
            (ROTR32(x0[i3], 10) ^ ROTL32(x0[i1023], 9)) + 
            x1[(x0[i3] ^ x0[i1023]) & 0x3ff];
    
    i12 = (i - 12) & 0x3ff;
    
    w0=x0[i];
    w1=x0[i12];

    for (r=0, t=0; t<4; t++) {
      r += x1[w1 & 255];
      w1 >>= 8;
      x1 += 1024/4;
    }
    r ^= w0;
    
    return r;
}

#define SIG0(x)(ROTR32((x),  7) ^ ROTR32((x), 18) ^ ((x) >>  3))
#define SIG1(x)(ROTR32((x), 17) ^ ROTR32((x), 19) ^ ((x) >> 10))

// both key and iv must be 32 bytes / 256-bits!
void hc256_setkey(hc_ctx *c, void *kiv)
{
    uint32_t W[4096], i;
    uint32_t *pq, *wp;
    
    // 1. set counter
    c->ctr = 0;
    
    // 2. copy 512-bit key and iv to local workspace
    memcpy (W, kiv, 64);

    // 3. expand buffer using SHA-256 macros
    for (i=16; i<4096; i++) {
      W[i] = SIG1(W[i-2])  + W[i-7]  + 
             SIG0(W[i-15]) + W[i-16] + i; 
    }
    
    pq = c->T;
    wp = &W[512];
    
    // 6. set the P and Q tables
    for (i=0; i<2048; i++) { 
      *pq++ = *wp++;
    }
    
    // 7. run cipher 4096 iterations before generating output
    for (i=0; i<4096; i++) {
      hc256_generate(c);
    }
}

// encrypt/decrypt data in place
void hc256_crypt(hc_ctx *c, void *in, uint32_t inlen)
{
  uint32_t i, j, w;
  uint8_t *p=(uint8_t*)in;
  
  // encrypt all bytes
  for (i=0; i<inlen;) {
    w = hc256_generate(c);
    // encrypt 4 bytes or until i equals inlen
    for (j=0; j<4 && i < inlen; j++) {
      p[i++] ^= (w & 255);
      w >>= 8;
    }
  }
}

