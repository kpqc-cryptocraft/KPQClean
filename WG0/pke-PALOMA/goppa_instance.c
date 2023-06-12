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

#include "goppa_instance.h"
#include <stdlib.h>
#include <stdio.h>
#include "rng.h"

/**
* @brief 주어진 다항식에 (X-alpha)를 곱하는 함수. 
* @param IN_POLY 기존 다항식
* @param polydeg 기존 다항식의 차수
* @param alpha 생성할 (X-alpha)에 해당하는 alpha
* @param POLY  IN_POLY (X-alpha)
*/
void Xalphamul(OUT gf* POLY, IN gf* IN_Poly, IN int polydeg, IN gf alpha, IN table4mul* tab4mul){

    gf tmp[polydeg+2];      //출력 다항식은 deg+1차 이므로 deg+2 배열 생성
    tmp[0]=0;               //첫 xor를 위해 상수항 0으로 초기화
    for(int i=0;i<=polydeg;i++){    
        tmp[i+1] = IN_Poly[i];      //X 곱하는 것은 차수 1증가.
        tmp[i]^=gf_mul_o(IN_Poly[i],alpha,tab4mul); //alpha를 곱해서 원래 위치에 입력
    }
    for(int i=0;i<=polydeg+1;i++){
        POLY[i] = tmp[i];
    }

}

/**
* @brief 유한체 t개를 입력 받아 t차 다항식 생성 (X-a1)(X-a2)...(X-at)
* @param gfset 유한체 원소 t개
* @param GoppaPoly  goppa 다항식
*/
void genGoppaPoly(OUT gf* GoppaPoly, IN gf* gfset, IN PARAM* PALOMAParam){ 
    int t = PALOMAParam->t;

    /* X-alpha0 생성 */
    GoppaPoly[0]=gfset[0];
    GoppaPoly[1]=1;
    
    /* 나머지 t-1개의 1차식 곱.*/
    for(int i=1;i<t;i++){
        Xalphamul(GoppaPoly,GoppaPoly,i,gfset[i],&PALOMAParam->tab4mul);  // 남은 X-alpha 곱하기.
    }
}

/**
* @brief 유한체 F213을 섞어 x개 만큼만 배열로 출력.
*/
void genRandElementGF(OUT gf* subSet, IN int x){
    
    gf set[(1<<13)];
    for(int i=0;i<(1<<13);i++){
        set[i]=i;
    }

    for(int i=(1<<13)-1;i>0;i--){
        unsigned char seed[2];
        unsigned int randnum;
        randombytes(seed, 2);   
        randnum = seed[0];
        randnum ^= (seed[1]<<8);   
        int j = randnum%(i+1);
        gf tmp = set[j];
        set[j] = set[i];
        set[i] = tmp;
    }

    for(int i=0;i<x;i++){
        subSet[i]=set[i];
    }

}
