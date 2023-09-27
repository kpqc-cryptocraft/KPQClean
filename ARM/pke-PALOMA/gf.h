/*
 * Copyright (c) 2022 FDL(Future cryptography Design Lab.) Kookmin University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef GF_H
#define GF_H

#include "config.h"

#define IRR_POLY 0b10000011100001 // x^13 + x^7 + x^6 + x^5 + 1

/*
   a_13 z^13 + ... +a_0z^0  > 0 || 0 || a_13 || a_12 || a_11 || .... || a_0  : 2bytes
*/

typedef unsigned short gf;

typedef struct{
   gf mul6bx6bTab[64][64];   // 상위 6bit x 상위 6bit
   gf mul6bx7bTab[64][128];  // 상위 6bit x 하위 7bit
   gf mul7bx7bTab[128][128]; // 하위 7bit x 하위 7bit
} table4mul;         // 곱셈을 위한 테이블


typedef struct{
   gf tab4square[(1<<Param_M)];  // 제곱을 위한 테이블
   gf tab4sqrt[(1<<Param_M)];    // 제곱근을 위한 테이블
   gf tab4inv[(1<<Param_M)];     // 역원을 위한 테이블
   int n;                        // 코드 길이
   int t;                        // 다항식 차수
   int k;                        // 메시지 길이 
   table4mul tab4mul;            // 곱셈을 위한 테이블
} PARAM;         // 전체 테이블


void gf_print(IN gf in);
gf gf_add(IN gf in1, IN gf in2);

gf gf_mul(IN gf in1, IN gf in2);
gf gf_mul_o(IN gf in1, IN gf in2, IN table4mul* tab4mul); // table lookup, Table은 3개 테이블 structure.

gf gf_square(IN gf in);
gf gf_square_o(IN gf in, IN gf* tab4square);  //13비트 제곱에 대한 결과 가지고 있는 테이블 입력

gf gf_sqrt(IN gf in);
gf gf_sqrt_o(IN gf in, IN gf* tab4sqrt);    //13비트 제곱근에 대한 결과 가지고 있는 테이블 입력

gf gf_inv(IN gf in);
gf gf_inv_o(IN gf in, IN gf* tab4inv);     //13비트 역원에 대한 결과 가지고 있는 테이블 입력



#endif