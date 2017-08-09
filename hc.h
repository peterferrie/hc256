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

#ifndef HC256_H
#define HC256_H

#include <stdint.h>
#include <string.h>

#include "macros.h"

#define SIG0(x)(ROTR32((x),  7) ^ ROTR32((x), 18) ^ ((x) >>  3))
#define SIG1(x)(ROTR32((x), 17) ^ ROTR32((x), 19) ^ ((x) >> 10))

typedef struct hc_ctx_t {
  uint32_t ctr;
  union {
    uint32_t T[2048];
    struct {
      uint32_t P[1024];
      uint32_t Q[1024];
    };
  };
} hc_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void hc256_setkeyx(hc_ctx*, void*);
void hc256_cryptx(hc_ctx*, void*, uint32_t);

void hc256_setkey(hc_ctx*, void*);
void hc256_crypt(hc_ctx*, void*, uint32_t);

#ifdef __cplusplus
}
#endif

#endif