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

#include "common.h"
#include <stdio.h>
#include "include/lsh.h"
#include "lsh512.h"
#include "lsh_local.h"
#include "rng.h"
#include "include/lsh_def.h"
#include "gf.h"

/**
* @brief 난수 r 이용 하여 원소가 n개인 집합 셔플
*/
void shuffle(OUT gf* shuffled, IN int n, IN u64* r){  //원소 n개 짜리 집합을 난수 r 이용 하여 셔플
    /* 원소 n개 집합 생성 */
    for(int i=0;i<n;i++){
        shuffled[i]=i;
    }

    /* shuffle 집합 생성 */ 
    int count =0;
    for(int i= n-1;i>0;i--){
        int seed = (r[count/4]>>(16*(count%4)))&0xffff;    // 입력 받은 난수중 16비트만 이용
        count = (count+1)&0xf;
        int j = seed%(i+1);   
        
        /* swap */          
        int tmp = shuffled[j];
        shuffled[j] = shuffled[i];
        shuffled[i] = tmp;
    }
}

/**
* @brief 길이가 n 비트인 난수열 생성
*/
void randbitgen(OUT u64* randnum,IN int n){
    unsigned char seed[1];
    for(int i=0;i<(n/64);i++){
        randnum[i]=0;
        for(int j=0;j<8;j++){
            randombytes(seed, 1);     
            u64 k = seed[0]&0xff;
            randnum[i]|=(k<<(8*(7-j)));
        }
    }
}

/**
* @brief encap, decap 과정에서 사용하는 랜덤오라클
* @param msg: 오라클 입력 데이터
* @param msglen: 오라클 입력 데이터의 비트 길이.
* @param oraclenum: 1 인경우 oracle G, 2인 경우 oracle H 사용
* @param seed: oracle 결과 (해시값)
*/
int RandOracle(OUT u64* seed, IN u64* msg, IN int msglen, IN int oraclenum){ 		  
	// LSH512을 사용하고, 출력 해시 길이를 512비트로 설정 */
	lsh_type algtype = LSH_MAKE_TYPE(1, 512);  

	// 입력 데이터앞으로 8byte 문자열이 추가되므로 8을 추가한 만큼 길이로 배열 생성
	lsh_u8 src[8+(msglen/8)];

    /* oracle 입력 데이터 생성 */
    /* PALOMAGG or PALOMAHH ASCII 값*/
    src[0]= 0x50;
    src[1]= 0x41;
    src[2]= 0x4c;
    src[3]= 0x4f;
    src[4]= 0x4d;
    src[5]= 0x41;

    if (oraclenum ==1){
        src[6]= 0x47;
        src[7]= 0x47;
    }
    else if (oraclenum ==2){
        src[6]= 0x48;
        src[7]= 0x48;
    }
    else{
        return -1;
    }
    /* 입력받은 데이터 */
    for(int i=0;i<(msglen/8);i++) {
        src[8+i] = (msg[i/8]>>((8*i)%64))&0xff;
    }
	
    // 출력 해시값을 512비트(512/8=64바이트) 변수 result에 저장
	lsh_u8 result[64] = {0, };

    /* 해시값 생성 */
	lsh_digest(algtype, src, 64+msglen, result);

    /* 해시값 출력 */
    for(int i=0;i<32;i++){
        seed[i/8] |= ((u64)result[i]<<((i*8)%64));
    }

    return 0;
}
