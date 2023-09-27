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

#include "decap.h"
#include "gfpoly.h"
#include "mat_mul.h"
#include "common.h"
#include <stdio.h>
#include "keygen.h"
#include "encap.h"

/**
* @brief 비트열로 되어있는 신드롬은 유한체 원소 단위로 나누는 함수
* @param synd: 비트형태의 신드롬
* @param syndpoly: 유한체 원소 신드롬 다항식.
* @param t: 신드롬 다항식의 차수
*/
void Topoly(OUT gf* syndpoly, IN u64* synd, IN int t){
    gf spoly[MAXT]={0,};
    int row =0;

    /* 13비트씩 나누어 유한체 원소 생성 */
    for(int i=0;i<(13*t);i++){
        spoly[(i/13)] |= (((synd[i/64]>>(i%64))&1)<<(i%13));    
    }
    for(int i=0;i<t;i++){
        syndpoly[i] = spoly[i];
    }
}

/**
* @brief 확장 Patterson 디코딩에서 사용하는 키방정식 생성 함수
*/
void ConstructKeyEqn(OUT gf* shat, OUT gf* g1, OUT gf* g2, OUT gf* g12, IN gf* syndpoly, IN gf* gx, IN PARAM* PALOMAParam){
    
    gf sp[MAXT]={0,};     
    int t=PALOMAParam->t;

    /* 1+ Xs(X) */
    sp[0]=1;  
    for(int i=0;i<t;i++){
        sp[i+1]=syndpoly[i];      
    }

    // printf("\nsp(x)\n");
    // GF_print(sp,t);
    // printf("\ng(x)\n");
    // GF_print(gx,t);
    
    /* gcd(g(X),s(X)), gcd(g(X),sp(X))*/
    gf tmp[MAXT]={0,},g1g2[MAXT]={0,}; //  최대 t차
    GF_GCD(g1, gx, syndpoly, PALOMAParam);
    GF_GCD(g2, gx, sp, PALOMAParam);

    /* g(X) / g1(X)g2(X) 생성 */
    GF_mul_o(g1g2, gx, g1, g2,  PALOMAParam);
    GF_remind(g12, tmp, gx, g1g2, PALOMAParam);

    /* sp(X)/g2(X), s(X)/g(X) 생성 */
    gf s2p[MAXT]={0,},s1[MAXT]={0,}; //  최대 t차

    GF_remind(s2p, tmp, sp, g2, PALOMAParam);
    GF_remind(s1, tmp, syndpoly, g1, PALOMAParam);

    /* g1(X)sp2(X) (g2(X)s1(X))^{-1} mod g12(X) */
    gf tmp2[MAXT]={0,}, tmp3[MAXT]={0,}, u[MAXT]={0,}, check[MAXT]={0,},tmp4[MAXT]={0,},sqrtcheck[MAXT]={0,};
    GF_mul_o(tmp2, g12, g2, s1, PALOMAParam);
    GF_inv_o(tmp3, tmp2, g12, PALOMAParam);
    GF_mul_o(check, g12, tmp2, tmp3, PALOMAParam);
    GF_mul_o(tmp4, g12, g1, s2p, PALOMAParam);
    GF_mul_o(u, g12, tmp4, tmp3, PALOMAParam);

    /* sqrt(u(X)) mod g12(X) */
    GF_sqrt_o(shat, g12, u, PALOMAParam);
    //GF_square_o(sqrtcheck, g12, shat, PALOMAParam);

}

/**
* @brief 확장 Patterson 디코딩에서 사용하는 키방정식 해찾기 함수
*/
void SolveKeyEqn(OUT gf* a1, OUT gf* b2 , IN gf* shat, IN gf* g12, IN int dega, IN int degb, IN PARAM* PALOMAParam){
    gf a0[MAXT]={0,},b0[MAXT]={0,},b1[MAXT]={0,};
    int t=PALOMAParam->t;

    for(int i=0;i<t+1;i++){
        a0[i] = shat[i];
        a1[i] = g12[i];
    }
    b0[0]=1;

    while(Deg_poly(a1,t)>=0){
        gf qq[MAXT]={0,} ,r[MAXT]={0,},tmp[MAXT]={0,}  ;
        /* a0(X) = qq(X)a1(X) + r(X) */
		GF_remind(qq,r,a0,a1,PALOMAParam);
        for(int i=0;i<t+1;i++){
            a0[i]=a1[i];
            a1[i]=r[i];
        }

        /* b2(X) = b0(X) − q(X)b1(X) */
        POLY_mul_o(tmp, qq,b1,PALOMAParam);
        GF_add(b2,b0,tmp,t);  

        for(int i=0;i<t+1;i++){
            b0[i]=b1[i];
            b1[i]=b2[i];
        }

        if((Deg_poly(a0,t)<=dega)&&(Deg_poly(b0,t)<=degb))
            break;
    }

    for(int i=0;i<t+1;i++){
        a1[i] = a0[i];
        b2[i] = b0[i];
    }

}

/**
* @brief 오류위치 다항식으로 부터 오류벡터 생성하는 함수
* @param sigma : 오류위치 다항식
* @param supportSet : L
* @param errorvec : 생성한 오류벡터
*/
void FindErrVec(OUT u64* errorvec, IN gf* sigma, IN u64* supportSet, IN PARAM* PALOMAParam){

    u64 One =1;
    int n = PALOMAParam->n;
    int t = PALOMAParam->t;

    /* supportset 유한체 형태로 변형*/
    gf spset[MAXN]={0,};
    for(int i=0;i<13*n;i++)
    {
        spset[i/13]|=(((supportSet[(i/64)]>>(i%64))&1)<<(i%13));
    } 

    /* 전수조사로 해인 경우 해당 인덱스 오류벡터 1로 설정. */
    u64 errvec[MAXN/64]={0,};
    for(int i=0;i<n;i++){
        if(GF_eval(sigma , spset[i], t, PALOMAParam)==0){
            errvec[i/64]|=(One<<(i%64));
        }
    }
    for(int i=0;i<n/64;i++){
        errorvec[i]=errvec[i];
    }
}

/**
* @brief Decrypt 함수
* @param sk : 비밀키
* @param synd_hat : 신드롬 벡터
* @param e_hat : 생성한 오류벡터
*/
void Decrypt(OUT u64* e_hat, IN u64* sk,  IN u64* synd_hat, IN PARAM* PALOMAParam){
    u64 synd[13*MAXT/64]={0,}; 
    u64 One=1;
    int n = PALOMAParam->n;
    int t = PALOMAParam->t;

    /* 개인키 S^{-1} 행렬 추출*/
    u64 Sinv[(13*MAXT/64)*13*MAXT]={0,};
    for(int i=0;i<(13*t/64)*13*t;i++){
        Sinv[i]=sk[(n*13/64) + (t*13/64)+i];
    }

    /* S^{-1} X shat */
    matXvec(synd, Sinv, synd_hat, 13*t, 13*t);

    u64 e[MAXN/64]={0,}; 

    /* 에러벡터 복원 */
    RecErrVec(e, sk, synd, PALOMAParam);

    /* 개인키 P 생성 시드 추출 */
    gf P[MAXN]={0,};
    u64 r[4]={0,};
    for(int i=0;i<4;i++){
        r[i] = sk[(n*13/64) + (t*13/64) + (13*t*13*t/64)+i];    //r 저장.
    }

    /* 시드로 P 생성 */
    GenRandPermMat(P, n, r);

    for(int i=0;i<n/64;i++) e_hat[i] = e[i];

    /* ehat = P^{−1}e */
    for(int i=0;i<n;i++){        //P의 역행렬과 곱
        if(((e_hat[i/64] >> (i%64))&1) != ((e_hat[P[i]/64]>>(P[i]%64))&1)){       // 두 비트가 다른 경우에만 ^1 해서 바꾸기. 
            e_hat[i/64] ^= (One<<(i%64));
            e_hat[P[i]/64] ^= (One<<(P[i]%64));        
        }
    }
}

/**
* @brief 오류벡터 복원함수 (확장 Patterson)
* @param sk : 비밀키
* @param synd : 신드롬 벡터
* @param e : 생성한 오류벡터
*/
void RecErrVec(OUT u64* e, IN u64* sk, IN u64* synd, IN PARAM* PALOMAParam){
    gf spoly[MAXT]={0,}, s_hat[MAXT]={0,}, g1[MAXT]={0,}, g2[MAXT]={0,}, g12[MAXT]={0,}, gx[MAXT]={0,};
    gf a2[MAXT]={0,},b1[MAXT]={0,},sigma[MAXT]={0,},a[MAXT]={0,},b[MAXT]={0,};
    gf aa[MAXT]={0,},bb[MAXT]={0,};
    int t=PALOMAParam->t;
    int n=PALOMAParam->n;


    /* 신드롬 다항식 생성 */
    Topoly(spoly, synd, t);     

    /* 개인키로 부터 goppa poly 추출 */
    for(int i=0;i<13*t;i++)
    {
        gx[i/13]|=((sk[(13*n/64)+(i/64)]>>(i%64))&1)<<(i%13);
    }
    gx[t]=1;
    
    /* 오류 위치 다항식 생성 */
    ConstructKeyEqn(s_hat, g1, g2, g12, spoly, gx, PALOMAParam);
    
    SolveKeyEqn(a2, b1 , s_hat, g12, (t/2)-Deg_poly(g2,t),((t-1)/2)-Deg_poly(g1,t), PALOMAParam);
    
    
    /* a(X), b(X) = a2(X)g2(X), b1(X)g1(X) */
    POLY_mul_o(a,a2,g2,PALOMAParam);
    POLY_mul_o(b,b1,g1,PALOMAParam);


    /* sigma(X) = a2(X) + b2(X)X*/
    POLY_mul_o(aa,a,a,PALOMAParam);
    POLY_mul_o(bb,b,b,PALOMAParam);

    for(int i=t-1;i>=0;i--){
        sigma[i+1]^=bb[i];
        sigma[i]^=aa[i];
    }
    sigma[t]=aa[t];

    /* monic다항식으로 변경 */
    gf coefinv = gf_inv_o(sigma[t],PALOMAParam->tab4inv);

    for(int i=0;i<=t;i++){
        sigma[i] = gf_mul_o(sigma[i],coefinv, &PALOMAParam->tab4mul);
    }

    /* 개인키로 부터 support set 추출 */
    u64 supportSet[13*MAXN/64]={0,};
    
    for(int i=0;i<13*n/64;i++)
    {
        supportSet[i]=sk[i];
    }    

    FindErrVec(e, sigma, supportSet, PALOMAParam);
    


}


/**
* @brief decapsulation 함수
* @param sk : 비밀키
* @param r_hat : 암호문 중 256비트 r
* @param s_hat : 암호문 중 신드롬 
* @param key : 복원한 키 
*/
void Decap(OUT u64* key, IN u64* sk, IN u64* r_hat, IN u64* s_hat, IN PARAM* PALOMAParam){
    gf P[MAXN]={0,};  
    u64 e_hat[MAXN/64]={0,}, ep[MAXN/64]={0,}, e_tilde[MAXN/64]={0,}, r[4]={0,};

    int n= PALOMAParam->n;
    int k= PALOMAParam->k;
    int t= PALOMAParam->t;


    /* decrypt */
    Decrypt(e_hat,sk,  s_hat, PALOMAParam);
    
    /* randperm 복원 */
    GenRandPermMat( P, n, r_hat);   

    /* ep ← P^{−1} ehat */
    for(int i=0;i<n/64;i++) ep[i] = e_hat[i];

    u64 One = 1;
    for(int i=0;i<n;i++){        //P의 역행렬과 곱
        if(((ep[i/64] >> (i%64))&1) != ((ep[P[i]/64]>>(P[i]%64))&1)){       // 두 비트가 다른 경우에만 ^1 해서 바꾸기. 
            ep[i/64] ^= (One<<(i%64));
            ep[P[i]/64] ^= (One<<(P[i]%64));         
        }
    }

    /* rphat = ROG(ep) */
    u64 rphat[4]={0,};
    int check = RandOracle(rphat, ep, n, 1);
    
    for(int i=0;i<4;i++){
        r[i] = sk[(n*13/64) + (t*13/64) + (13*t*13*t/64)+i];    //r 저장.
    }

    /* e = GenRandErrVec(r) */
    GenRandErrVec(e_tilde, r, n, t);

    /* rphat = rhat 체크 */
    int compare =0;
    for(int i=0;i<4;i++){
        if(r_hat[i]!=rphat[i]){
            compare=1;
            break;
        }
    }

    /* 비교후 연접 오류 벡터 ep또는 etilde 사용 */
    u64 ROinput[MAXN/64 + 4 + (13*MAXT)/64]={0,};

    for(int i=0; i<4;i++){
        ROinput[i+(n/64)] = r_hat[i];
    }
    for(int i=0; i<(n-k)/64;i++){
        ROinput[i+4+(n/64)] = s_hat[i];
    }

    /* rphat = rhat인경우 ep 사용 */
    if(compare ==0 ){
        for(int i=0;i<n/64;i++)
            ROinput[i] = ep[i]; //입력으로 ep가 연접
        check = RandOracle(key,  ROinput,  n+256+(n-k), 2);
    }

    /* rphat != rhat인경우 etilde 사용 */
    if(compare ==1){
        //printf("\nr3\n");
        for(int i=0;i<n/64;i++)
            ROinput[i] = e_tilde[i]; //입력으로 etilde 가 연접
        check = RandOracle(key, ROinput , n+256+(n-k), 2);
    }

}
