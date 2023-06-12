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

#include "keygen.h"
#include "goppa_instance.h"
#include "gfpoly.h"
#include <stdio.h>
#include "mat_mul.h"
#include "common.h"
#include <stdlib.h>

/**
* @brief 패리티 검사행렬 생성 함수
* @param GPPoly goppa polynomial
* @param SupSet support set
* @param H 패리티 검사행렬 H
*/
void makeParityCheckMat(OUT u64* H, IN gf* GPPoly, IN gf* SupSet, IN PARAM* PALOMAParam){

    int n = PALOMAParam->n;
    int t = PALOMAParam->t;

    gf gfPCMat[MAXT][MAXN]={0,};
    gf gfPCMatresult[MAXT][MAXN]={0,};

    u64 PCmat[13*MAXT*MAXN/64]={0,};

    /* C행렬 생성 */
    for(int i=0;i<n;i++){
        gf galpha=GF_eval(GPPoly, SupSet[i], t, PALOMAParam);        
        gfPCMat[0][i]=gf_inv_o(galpha, PALOMAParam->tab4inv);          // C행렬 (g(a)^-1) 미리 곱해두기
    }
    
    /* BC행렬 생성 */
    for(int i=1;i<t;i++){
        for(int j=0;j<n;j++){
            gfPCMat[i][j] = gf_mul_o(gfPCMat[i-1][j],SupSet[j],&PALOMAParam->tab4mul);  // BC행렬
        }
    }

    /* ABC행렬 생성 */
    gf tmp;
    for(int i=0;i<t;i++){    // A행렬 행  
        for(int j=0;j<n;j++){      //BC 행렬 열
            tmp =0;
            for(int k=0;k<t-i;k++){      //A행렬 열
                tmp^= gf_mul_o(GPPoly[k+i+1],gfPCMat[k][j],&PALOMAParam->tab4mul);        //ABC 행렬
            }
            gfPCMatresult[i][j] = tmp;    // gfPCMat 중복 확인 //확인
        }
    }

    /* ABC행렬을 64비트 원소에 빈공간없이 저장 */
    int row;
    int tmp2=0;
    for(int j=0;j<n;j++){
        row =0;
        for(int i=0;i<t;i++){
            if(tmp2<51){
                PCmat[row + j*(13*t/64)] |= (((u64)gfPCMatresult[i][j])<<(tmp2));
                tmp2+=13;
            }
            else
            {
                u64 temp = gfPCMatresult[i][j]&((1<<(64 - tmp2))-1);
                PCmat[row + j*(13*t/64)] |= (temp<<(tmp2));
                PCmat[row + j*(13*t/64) + 1] |= (gfPCMatresult[i][j] >> (64 - tmp2)) ;
                row ++;
                tmp2 -= 51;
            }

        }
    }

     for(int i=0;i<13*t*n/64;i++){
        H[i]=PCmat[i];
     }
}

/**
* @brief goppacode 생성함수
* @param SupportSet support set
* @param GoppaPoly goppa polynomial
* @param H 패리티 검사행렬 H
*/
void GenRandGoppaCode(OUT gf* SupportSet, OUT gf* GoppaPoly, OUT u64* H, IN PARAM* PALOMAParam){

    int t = PALOMAParam->t;
    int n = PALOMAParam->n;

    gf gfset[MAXN+MAXT]={0,};
    gf gf4goppapoly[MAXN]={0,};

    /* Goppa Code 생성 */ 
    /* 유한체 gf 원소 n+t개 선택 */   
    genRandElementGF(gfset, n+t);  

    /* support set 생성 */ 
    for(int i=0;i<n;i++){
         SupportSet[i] = gfset[i]; // 상위 n개의 원소는 supportset 원소로 사용
    }

    /* goppa polynomial 생성 */
    for(int i=0;i<t;i++){
        gf4goppapoly[i] = gfset[n+i]; // 하위 t개의 원소는 goppa poly용 원소로 사용
    }

    genGoppaPoly(GoppaPoly, gf4goppapoly, PALOMAParam);  // goppa poly 생성
    
    /* Parity Check Matrix 생성 */
    makeParityCheckMat(H, GoppaPoly, SupportSet, PALOMAParam);

}

/**
* @brief 패리티 검사행렬 scramble 함수
* @param H 패리티 검사행렬 H
* @param H_hat scrambled H
* @param S_inv 개인키 행렬 S
* @param rand4P P 생성용 시드 
*/
void GetScrambledCode(OUT u64* H_hat, OUT u64* S_inv, OUT u64* rand4P, IN u64* H, IN PARAM* PALOMAParam){

    int t = PALOMAParam->t;
    int n = PALOMAParam->n;

   while(1)
    {
        /* 치환행렬 P 생성 */
        gf P[MAXN]={0,};  // gf 로 잡아서 진행. 

        u64 r[4]={0,};
        randbitgen(r,n);

        for(int i=0;i<4;i++){
            rand4P[i]=r[i];
        }

        GenRandPermMat(P, n, r);

        /* HP 생성 */
        u64 HPmat[13*(MAXT/64)*MAXN]={0,};

        for(int i=0;i<n;i++){
            for(int j=0;j<13*(t/64);j++){  
                HPmat[j+(i*13*(t/64))] = H[j+(i*13*(t/64))]; 
            }
        }

        u64 tmp=0;
        for(int i=0;i<n;i++){
            for(int j=0;j<13*(t/64);j++){   //HP생성
                tmp = HPmat[j+(P[i]*13*(t/64))];  
                HPmat[j+(P[i]*13*(t/64))] = HPmat[j+(i*13*(t/64))]; 
                HPmat[j+(i*13*(t/64))] = tmp;      
            }
        }


        /* Gaussian elimination */
        u64 systematic[MAXN*13*(MAXT/64)]={0,};     //
        u64 HPmat_row[13*(MAXT/64)*MAXN]={0,};  // HP를 행벡터로 다시 표기한 행렬

        /* 원활한 Gaussian을 위해 열단위로 표현된 행렬을 행단위로 변경 */
        for(int j=0;j<n;j++){
            for(int i=0;i<(13*t);i++){
                HPmat_row[((n*i)+j)/64] |= (((HPmat[(i+(j*13*t))/64] >>(i%64))&1)<<(j%64));
            }
        }

        int check = Gaussian_row(systematic, HPmat_row, 13*t , n);

        // /* full rank가 될때까지 위의 과정 반복*/
        if(check==0)
        {
            for(int i=0;i<(13*t*n/64);i++){
                H_hat[i]=systematic[i];
            }

            for(int j=0;j<(13*t);j++){
                for(int i=0;i<(13*t)/64;i++){
                    S_inv[i+j*(13*t)/64] = HPmat[i+j*(13*t)/64];
                }
            }    
            break;
        }
    }    
}

/**
* @brief 키생성함수
* @param pk 공개키 M
* @param sk 개인키 L,g(X),S, P의seed
*/
void GenKeyPair(OUT u64* pk, OUT u64* sk, IN PARAM* PALOMAParam){

    int n = PALOMAParam->n;
    int t = PALOMAParam->t;

    gf SupportSet[MAXN]={0,};
    gf GoppaPoly[MAXT]={0,};
    
    u64 H[13*(MAXT/64)*MAXN]={0,};
    u64 H_hat[13*(MAXT/64)*MAXN]={0,};
    u64 S_inv[13*(MAXT/64)*13*MAXT]={0,};
    u64 rand4P[4]={0,};
    

    /* goppa 부호 생성 */
    GenRandGoppaCode(SupportSet, GoppaPoly, H, PALOMAParam);

    /* 패리티검사행렬 scramble */
    GetScrambledCode(H_hat, S_inv, rand4P, H, PALOMAParam);
    
    /* sk로 저장. 순서는 L,g,S,P 순서 */
    u64 srk[(MAXN*13/64) + (MAXT*13/64) + (13*MAXT*13*MAXT/64) + 4 ]={0,};
    int num=0;
    int tmp=0;
    for(int i=0;i<n;i++){
        if(tmp<51){
            srk[num] |= (((u64)SupportSet[i])<<(tmp));
            tmp+=13;
        }
        else
        {
            u64 temp = SupportSet[i]&((1<<(64 - tmp))-1);
            srk[num] |= temp<<(tmp);
            srk[num+1] |= (SupportSet[i] >> (64 - tmp)) ;
            num ++;
            tmp -= 51;
        }
    }

    for(int i=0;i<t;i++){
        if(tmp<51){
            srk[num] |= (((u64)GoppaPoly[i])<<(tmp));
            tmp+=13;
        }
        else
        {
            u64 temp = GoppaPoly[i]&((1<<(64 - tmp))-1);
            srk[num] |= temp<<(tmp);
            srk[num+1] |= (GoppaPoly[i] >> (64 - tmp)) ;
            num ++;
            tmp -= 51;
        }
    }

    tmp = (13*t/64)*13*t;

    for(int i=0;i<tmp;i++){
        srk[num+i]=S_inv[i];
    }
    num +=tmp;

    for(int i=0;i<4;i++){
        srk[num+i]=rand4P[i];
    }

    tmp = (13*t+13*n+13*t*13*t+256)/64;
    for(int i=0;i<tmp; i++){
        sk[i]=srk[i];
    }

    tmp =(13*t*(n-13*t)/64);
    for(int i=0;i<tmp;i++){
        pk[i]=H_hat[(13*t*13*t)/64+i];
    }

} 


/**
* @brief 치환함수 생성
* @param row 행의 개수
*/
void GenRandPermMat(OUT gf* permmatP, IN int row, IN u64* r){

    shuffle(permmatP, row, r);
}
