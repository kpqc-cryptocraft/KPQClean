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

#ifndef GF_TABLE_GEN_H
#define GF_TABLE_GEN_H

#include "config.h"
#include "gf.h"

void print_all_tab();
void GF_init(OUT PARAM* PALOMAParam, IN int n, IN int k, IN int t);

void gen_gfmul_tab(OUT table4mul* tab4mul);     //곱셈용 테이블 3개 생성
void gen_gfsquare_tab(OUT gf* tab4square);      //제곱용 테이블 생성
void gen_gfsqrt_tab(OUT gf* tab4sqrt);          //제곱근용 테이블 생성
void gen_gfinv_tab(OUT gf* tab4inv);            //역원용 테이블 생성

void print_gfmul_tab(IN table4mul* tab4mul);    //곱셈용 테이블 출력
void print_gfsquare_tab(IN gf* tab4square);     //제곱용 테이블 출력
void print_gfsqrt_tab(IN gf* tab4sqrt);         //제곱근용 테이블 출력
void print_gfinv_tab(IN gf* tab4inv);           //역원용 테이블 출력


void measure_performance(IN PARAM* PALOMAParam);  // 모든 함수 시간 측정 결과 알수있도록. 
void tab_verify_check(IN PARAM* PALOMAParam);     // 테이블 연산 == 일반 연산 확인


#endif