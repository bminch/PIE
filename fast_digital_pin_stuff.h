/*
** Copyright (c) 2021, Bradley A. Minch
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met: 
** 
**     1. Redistributions of source code must retain the above copyright 
**        notice, this list of conditions and the following disclaimer. 
**     2. Redistributions in binary form must reproduce the above copyright 
**        notice, this list of conditions and the following disclaimer in the 
**        documentation and/or other materials provided with the distribution. 
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
** POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FAST_DIGITAL_PIN_STUFF_H_
#define _FAST_DIGITAL_PIN_STUFF_H_

#define D0_PORT   PORTD
#define D1_PORT   PORTD
#define D2_PORT   PORTD
#define D3_PORT   PORTD
#define D4_PORT   PORTD
#define D5_PORT   PORTD
#define D6_PORT   PORTD
#define D7_PORT   PORTD
#define D8_PORT   PORTB
#define D9_PORT   PORTB
#define D10_PORT  PORTB
#define D11_PORT  PORTB
#define D12_PORT  PORTB
#define D13_PORT  PORTB

#define D0_BIT    0b00000001
#define D1_BIT    0b00000010
#define D2_BIT    0b00000100
#define D3_BIT    0b00001000
#define D4_BIT    0b00010000
#define D5_BIT    0b00100000
#define D6_BIT    0b01000000
#define D7_BIT    0b10000000
#define D8_BIT    0b00000001
#define D9_BIT    0b00000010
#define D10_BIT   0b00000100
#define D11_BIT   0b00001000
#define D12_BIT   0b00010000
#define D13_BIT   0b00100000

#define digitalPinSet(p, b)     p |= b
#define digitalPinClear(p, b)   p &= (~b)
#define digitalPinToggle(p, b)  p ^= b

#endif
