;
;  Copyright © 2016 Odzhan, Peter Ferrie. All Rights Reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;
;  1. Redistributions of source code must retain the above copyright
;  notice, this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright
;  notice, this list of conditions and the following disclaimer in the
;  documentation and/or other materials provided with the distribution.
;
;  3. The name of the author may not be used to endorse or promote products
;  derived from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
;  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
;  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
;  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
;  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
;  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;  POSSIBILITY OF SUCH DAMAGE.
;
; -----------------------------------------------
; HC-256 stream cipher in x86 assembly
;
; size: 272 bytes
;
; global calls use cdecl convention
;
; -----------------------------------------------
    bits   32
   
struc pushad_t
  _edi resd 1
  _esi resd 1
  _ebp resd 1
  _esp resd 1
  _ebx resd 1
  _edx resd 1
  _ecx resd 1
  _eax resd 1
  .size:
endstruc
    
    %ifndef BIN
      global hc256_setkeyx
      global _hc256_setkeyx
      
      global hc256_cryptx
      global _hc256_cryptx
    %endif
    
; expects ctx in edi
hc256_generatex:
_hc256_generatex:
    pushad
    xor    edx, edx
    mov    dh, 8               ; edx = 2048
    mov    esi, edi            ; esi = c
    lodsd                      ; eax = c->ctr
    push   eax                 ; save  c->ctr
    inc    eax                 ; c->ctr++
    dec    edx                 ; edx = 2047
    and    eax, edx            ; c->ctr &= 2047
    stosd                      ; save new c->ctr
    pop    eax                 ; restore old c->ctr
    lea    edi, [esi+edx*2+2]  ; x0  = c->Q
    shr    edx, 1              ; edx = 1023
    push   edx                 ; save 1023
    cmp    eax, edx            ; c->ctr > 1023
    jbe    gen_l0
    xchg   esi, edi            ; swap Q and P ptrs
gen_l0:
    and    eax, edx            ; i &= 1023
    
    lea    ebx, [eax-3]        ; i3 = (i - 3) & 1023;
    and    ebx, edx
    
    lea    ecx, [eax-10]       ; i10 = (i - 10) & 1023;
    and    ecx, edx
    
    lea    ebp, [eax-12]       ; i12 = (i - 12) & 1023;
    and    ebp, edx
    mov    ebp, [esi+ebp*4]
    push   ebp                 ; save i12
    
    mov    ebp, eax            ; i1023 = (i - 1023) & 1023;
    sub    ebp, edx
    and    ebp, edx
    
    push   eax                 ; save i
    mov    eax, [esi+eax*4]    ; eax  = x0[i]
    add    eax, [esi+ecx*4]    ; eax += x0[i10]
    
    mov    ebp, [esi+ebp*4]    ; ebp  = x0[i1023]
    mov    ebx, [esi+ebx*4]    ; ebx  = x0[i3]
    mov    ecx, ebx            ; ecx  = x0[i3]
    xor    ecx, ebp            ; ecx ^= x0[i1023]
    and    ecx, edx            ; ecx &= 0x3ff
    add    eax, [edi+ecx*4]    ; ecx  = x1[(x0[i3] ^ x0[i1023]) & 1023]
    ror    ebx, 10             ; ebx  = ROTR32(x0[i3], 10)
    rol    ebp, 9              ; ebp  = ROTL32(x0[i1023], 9)
    xor    ebx, ebp            ;
    add    eax, ebx            ; 
    pop    ebx                 ; ebx = i
    mov    [esi+ebx*4], eax    ; eax = (x0[i] += eax)
    pop    edx                 ; edx = i12
    
    pop    ebp                 ; ebp=1023
    inc    ebp                 ; ebp=1024
    xchg   eax, ecx
    xor    eax, eax            ; r=0
gen_l1:
    movzx  ebx, dl
    add    eax, [edi+ebx*4]
    add    edi, ebp            ; x1 += 1024/4
    shr    edx, 8              ; w1 >>= 8
    jnz    gen_l1
    
    xor    eax, ecx            ; r ^= w0;
    mov    [esp+_eax], eax     ; return r;
    popad
    ret
    
hc256_setkeyx:
_hc256_setkeyx:
    pushad
    mov    edi, [esp+32+4]   ; edi=c
    mov    esi, [esp+32+8]   ; esi=kiv
    ;
    xor    ecx, ecx          ; ecx=0
    mul    ecx               ; eax=0, edx=0
    
    mov    cl, 5             ; ecx=5
    mov    dh, 16            ; edx=4096
    ; allocate stack memory in 4096 byte blocks
    ; 4 x 4096 = 16384 bytes, 
    ; additional 4096 bytes just in case (not needed?)
xalloca:
    sub    esp, edx          ; subtract page size
    test   [esp], esp        ; page probe
                             ; causes pages of memory to be 
                             ; allocated via the guard page 
                             ; scheme (if possible)
    loop   xalloca           ; raises exception if 
                             ; unable to allocate
    mov    ebx, esp          ; ebx=W
    
    push   edx               ; save 4096    
    push   edi               ; save ptr to c
    stosd                    ; c->ctr=0
    push   edx               ; save 4096
    push   edi               ; save ptr to c->T
    push   edx               ; save 4096
    
    ; 2. copy 512-bits of key/iv to workspace
    mov    cl, 64
    mov    edi, ebx          ; edi=W
    rep    movsb
    
    mov    esi, ebx          ; esi=W
    mov    cl, 16
expand_key:
    ; eax = SIG0(W[i-15])
    mov    eax, [edi - 15*4]
    mov    edx, eax
    mov    ebp, eax
    ror    eax, 7
    ror    edx, 18
    shr    ebp, 3
    xor    eax, edx
    xor    eax, ebp
    ; ebx = SIG1(W[i-2])
    mov    ebx, [edi - 2*4]
    mov    edx, ebx
    mov    ebp, ebx
    ror    ebx, 17
    ror    edx, 19
    shr    ebp, 10
    xor    ebx, edx
    xor    ebx, ebp
    ; W[i] = ebx + W[i-16] + eax + w[i-7] + i
    add    eax, [edi - 16*4]
    add    ebx, [edi -  7*4]
    add    eax, ebx
    add    eax, ecx
    stosd
    inc    ecx
    cmp    ecx, [esp]        ; 4096 words
    jnz    expand_key
    
    pop    ecx               ; ecx=4096
    pop    edi               ; edi=c->T
    shr    ecx, 1            ; /=2 for 2048 words
    add    esi, ecx          ; add 512*4
    rep    movsd
    
    pop    ecx               ; ecx=4096
    pop    edi               ; edi=ctx
sk_l3:
    call   hc256_generatex
    loop   sk_l3
    
    pop    eax              ; eax=4096
    lea    esp, [esp+eax*4] ; free stack
    add    esp, eax
    popad
    ret
    
hc256_cryptx:
_hc256_cryptx:
    pushad
    lea    esi, [esp+32+4]
    lodsd
    xchg   edi, eax          ; edi=ctx
    lodsd
    xchg   edx, eax          ; edx=in
    lodsd
    xchg   ecx, eax          ; ecx=len
hc_l0:                       ; .repeat
    jecxz  hc_l2             ; .break .if ecx == 0
    call   hc256_generatex
hc_l1:
    xor    [edx], al         ; *in ^= (w0 & 0xFF)
    inc    edx               ; in++
    shr    eax, 8            ; w0 >>= 8
    loopnz hc_l1             ; .while ecx != 0 && eax != 0
    jmp    hc_l0
hc_l2:
    popad
    ret
    