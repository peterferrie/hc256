
HC-256 is a 256-bit symmetric key stream cipher.

Designers         Wu Hongjun
Related to        Blowfish, SHA2-256
Certification     eSTREAM portfolio
Key sizes         256 bits
State size        65536 bits

[ license

  Copyright © 2016 Odzhan, Peter Ferrie. All Rights Reserved.

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
  POSSIBILITY OF SUCH DAMAGE.
  
HC-256 has a 256 bit key and an initialization vector (nonce) of 256 
bits.

Internally, it consists of two secret tables (P and Q). Each table 
contains 1024 32-bit words. For each state update one 32-bit word in 
each table is updated using a non-linear update function. After 2048 
steps all elements of the tables have been updated. 

It generates one 32-bit word for each update step using a 32-bit to 
32-bit mapping function similar to the output function of the Blowfish 
cipher. Finally a linear bit-masking function is applied to generate an 
output word. It uses the two message schedule functions in the hash 
function SHA-256 internally, but with the tables P and Q as S-boxes. 

HC-128 is similar in function, and reduces each of key length, nonce, 
number of words in the tables P and Q, and number of table updating 
steps by half.

HC-128 and HC-256 are two software-efficient stream ciphers. In 2008, 
HC-128 was selected for the final portfolio of eSTREAM, the stream 
cipher project of the European Network of Excellence for Cryptology 
(ECRYPT, 2004-2008). HC-256 is the 256-bit companion version of HC-128. 
From a 128-bit key and a 128-bit initialization vector, HC-128 generates 
keystream with length up to $2^{64}$ bits. From a 256-bit key and a 
256-bit initialization vector, HC-256 generates keystream with length up 
to $2^{128}$ bits. 

HC-256 and HC-128 were designed to demonstrate that strong stream cipher 
can be built from nonlinear feedback function and nonlinear output 
function. The large secret states of the two ciphers are updated in a 
nonlinear way, and table lookup (with changing tables) is used in the 
generation of keystream. 

HC-128 and HC-256 are very efficient on modern microprocessors. For long 
message, the encryption speed of HC-128 is about 2.1 cycles/byte on 
32-bit Intel Core 2 microprocessor, and the encryption speed of HC-256 
is about 3.3 cycles/byte on 32-bit Core 2 microprocessor. The encryption 
speed of HC-128 is the fastest among the secure stream ciphers being 
submitted to eSTREAM proejct. 

HC-256 and HC-128 are not covered by any patent and they are 
freely-available. 


[x] Stream Ciphers HC-128 and HC-256
  http://www3.ntu.edu.sg/home/wuhj/research/hc/index.html

[x] Wu Hongjun
  http://www3.ntu.edu.sg/home/wuhj/