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
 
#include "encap.h"
#include "mat_mul.h"
#include "common.h"
#include "keygen.h"
#include <stdio.h>


/**
* @brief encrypt
* @param pk : 공개키
* @param e : 오류벡터
* @param synd : 신드롬 벡터
*/
void Encrypt(OUT u64* synd, IN u64* pk, IN u64* e, IN int n, IN int k){
    /* 항등행렬 부분 행렬곱 */
    for(int i=0;i<(n-k)/64;i++){
        synd[i]=e[i];                   // [I]e 계산.
    }   

    u64 err_k[MAXN/64]={0,}; 
    for(int i=0;i<k/64;i++) err_k[i] = e[(n-k)/64+i];   //e의 하위 k비트 선택

    u64 tmp[MAXN/64]={0,};
    /* [M] X e_j */
    matXvec(tmp, pk,err_k,(n-k),k);       // [M] e_j 계산.    

    /* [I|M] X e_j */
    for(int i=0;i<(n-k)/64;i++){
        synd[i]^=tmp[i];                   // [I|M]e 계산.
    }   
    
}

/**
* @brief weight가 t이고 길이가 n인 오류벡터 생성
* @param seed : 오류벡터 생성 시드
* @param errorvec : 생성한 오류벡터
*/
void GenRandErrVec(OUT u64* errorvec, IN u64* seed, IN int n, IN int t){
    gf err[MAXN]={0,};
    u64 One =1;

    /* seed이용 길이 n 배열 셔플 */
    shuffle(err, n, seed);

    for(int i=0;i<t;i++){
        gf in = err[i];
        errorvec[ (in/64) ] |= (One<< (in%64));
    }
}

/**
* @brief Encapsulation
* @param pk : 공개키
* @param key : 생성한 키
* @param r : 암호문중 256비트 비트열
* @param s_hat : 암호문중 신드롬
*/
void Encap(OUT u64* key, OUT u64* r, OUT u64* s_hat,  IN u64* pk, IN int n, IN int k, IN int t){

    /* 256비트 난수 생성 */
    u64 rp[4]={0,};    
    randbitgen(rp,256);

    /* 해밍무게가 t인 랜덤 벡터 생성 */
    u64 ep[MAXN/64]={0,};
    u64 e_hat[MAXN/64]={0,};

    GenRandErrVec(ep, rp, n, t);

    /* r = ROG(ep) */
    int check = RandOracle(r, ep, n, 1);

    /* 생성한 난수로 임의의 P 생성. */
    gf permmatP[MAXN] = {0,};

    GenRandPermMat(permmatP, n, r);

    /* ep 치환 */ 
    for(int i=0;i<n/64;i++) e_hat[i] = ep[i];

    u64 One = 1;
    for(int i=n-1;i>=0;i--){
        if(((e_hat[i/64] >> (i%64))&1) != ((e_hat[permmatP[i]/64]>>(permmatP[i]%64))&1)){       // 두 비트가 다른 경우에만 ^1 해서 바꾸기. 
            e_hat[i/64] ^= One<<(i%64);
            e_hat[permmatP[i]/64] ^= (One<<(permmatP[i]%64));         //check
        }
    }

    /* encrypt */
    Encrypt(s_hat, pk, e_hat,n,k);

    /* k = ROH(ep, rhat, shat) */
    u64 input[MAXN/64 + 4 + (13*MAXT)/64 ]={0,};

    for(int i=0;i<n/64;i++){
        input[i] = ep[i];
    }
    for(int i=0; i<4;i++){
        input[i+(n/64)] = r[i];
    }
    for(int i=0; i<(n-k)/64;i++){
        input[i+4+(n/64)] = s_hat[i];
    }

    int check2 = RandOracle(key, input, n + 256 + (n-k), 2);

}
