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
 
#include "api.h"
#include <stdio.h>
#include "keygen.h"
#include "encap.h"
#include "decap.h"
#include "config.h"

int crypto_PALOMA_keypair(unsigned char* pk, unsigned char* sk, PARAM* PALOMAParam){
    u64 sk64[(MAXN*13/64) + (MAXT*13/64) + (13*MAXT*13*MAXT/64) + 4 ]={0,}; // L + g(X) + S + P 순서
    u64 pk64[13*MAXT*(MAXN-(13*MAXT))/64]={0,};                             // 서브행렬
    int n = PALOMAParam->n;
    int t = PALOMAParam->t;
    GenKeyPair(pk64, sk64, PALOMAParam);
    
    int tmp = 13*t*(n-(13*t))/64;
    for(int i=0; i< tmp; i++){
        for(int j=0;j<8;j++){
            pk[8*i+j] = (pk64[i]>>(8*j))&0xff;
        }
    }
    
    tmp = (n*13/64) + (t*13/64) + (13*t*13*t/64) + 4;
    for(int i=0; i< tmp; i++){
        for(int j=0;j<8;j++){
            sk[8*i+j] = (sk64[i]>>(8*j))&0xff;
        }
    }

    return 0;

}

int crypto_PALOMA_enc(unsigned char* ct, unsigned char* key, const unsigned char* pk, PARAM* PALOMAParam){
    u64 key64[4]={0,};
    u64 r[4]={0,};
    u64 shat[13*MAXT/64]={0,};
    int n = PALOMAParam->n;
    int k = PALOMAParam->k;
    int t = PALOMAParam->t;
    
    u64 pk64[13*MAXT*(MAXN-(13*MAXT))/64]={0,};
    int tmp = 13*t*(n-(13*t))/64;
    
    for(int i=0; i<tmp; i++){
        for(int j=0;j<8;j++){
            pk64[i] |= (((u64)pk[8*i+j])<<(8*j));
        }
    }

    Encap(key64,r,shat,pk64,n,k,t);

    for(int i=0; i<4; i++){
        for(int j=0;j<8;j++){
            ct[8*i+j] = (r[i]>>(8*j))&0xff;
        }
    }

    tmp =13*t/64;
    for(int i=0; i<tmp; i++){
        for(int j=0;j<8;j++){
            ct[32+8*i+j] = (shat[i]>>(8*j))&0xff;
        }
    }

    for(int i=0; i<4; i++){
        for(int j=0;j<8;j++){
            key[8*i+j] = (key64[i]>>(8*j))&0xff;
            //printf("%d ",key[8*i+j]);
        }
    }
    //printf("\n");
    return 0;
}


int crypto_PALOMA_dec(unsigned char* key, const unsigned char* ct, const unsigned char* sk, PARAM* PALOMAParam){
    u64 recoverkey[4]={0,};
    u64 r[4]={0,};
    u64 shat[13*MAXT/64]={0,};
    u64 sk64[(MAXN*13/64) + (MAXT*13/64) + (13*MAXT*13*MAXT/64) + 4 ]={0,}; // L + g(X) + S + P 순서
    int n = PALOMAParam->n;
    int t = PALOMAParam->t;

    int tmp = (n*13/64) + (t*13/64) + (13*t*13*t/64) + 4 ;

    for(int i=0; i<tmp; i++){
        for(int j=0;j<8;j++){
            sk64[i] |= (((u64)sk[8*i+j])<<(8*j));
        }
    }

    tmp =13*t/64;
    for(int i=0; i<tmp; i++){
        for(int j=0;j<8;j++){
            shat[i] |= (((u64)ct[32+8*i+j]) <<(8*j));
        }
    }

    for(int i=0; i<4; i++){
        for(int j=0;j<8;j++){
            r[i]|= (((u64)ct[8*i+j] )<<(8*j));
        }
    }

    Decap(recoverkey, sk64, r, shat, PALOMAParam);

    for(int i=0; i<4; i++){
        for(int j=0;j<8;j++){
            key[8*i+j] = (recoverkey[i]>>(8*j))&0xff;
            //printf("%d ",key[8*i+j]);

        }
    }

    return 0;
}