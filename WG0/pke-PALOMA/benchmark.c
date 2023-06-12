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
 
#include <stdio.h>
#include "gf.h"
#include "gfpoly.h"
#include "gf_table_gen.h"
#include "config.h"
#include <time.h> 
#include <stdlib.h>
#include "mat_mul.h"
#include "goppa_instance.h"
#include "keygen.h"
#include "encap.h"
#include "decap.h"

//gcc -Wl,-stack_size -Wl,0x20000000 -O2 gf_table_gen.c gf.c gfpoly.c goppa_instance.c keygen.c mat_mul.c common.c lsh.c lsh512.c encap.c decap.c paloma.c rng.c benchmark.c

int main(){
    int tset[3]={64,128,128};
    int nset[3]={3904,5568,6592};

    int flag = 0;
    int t = tset[flag];     
    int n = nset[flag];   
    int k = (n-(13*t));
    
    PARAM PALOMAParam;

    GF_init(&PALOMAParam, n, k, t);     // 필수임.

	srand(time(NULL));

    int count = 100;
    clock_t start1, end1;
    float ttkeygen=0, ttencap=0, ttdecap=0;
    
    
    for(int i =0;i<count;i++){
        //=============keygen===========
        u64 sk[(MAXN*13/64) + (MAXT*13/64) + (13*MAXT*13*MAXT/64) + 4 ]={0,}; // L + g(X) + S + P 순서
        u64 pk[13*MAXT*(MAXN-(13*MAXT))/64]={0,};       // 서브행렬

        start1 = clock();
        GenKeyPair(pk, sk, &PALOMAParam);
        end1 = clock();

        ttkeygen += (float)(end1 - start1)/CLOCKS_PER_SEC;          //*********
        //==============encap===========

        u64 key[4]={0,};
        u64 r[4]={0,};
        u64 shat[13*MAXT/64]={0,};

        start1 = clock();
        Encap(key,r,shat,pk,n,k,t);
        end1 = clock();

        ttencap += (float)(end1 - start1)/CLOCKS_PER_SEC;          //*********
        //==============decap=============
        u64 recoverkey[4]={0,};
        
        start1 = clock();
        Decap(recoverkey, sk, r, shat, &PALOMAParam);
        end1 = clock();
        
        ttdecap += (float)(end1 - start1)/CLOCKS_PER_SEC;       

        for(int i=0;i<4;i++){
            if(key[i]!=recoverkey[i]){
                printf("\ndecerror!\n");
            }
        }   

    }
    printf("\ndecdone!\n");

	printf("\n(n,t) -(%d, %d), keygen in %d = %.6f \n", n,t,count, ttkeygen);
	printf("\n(n,t) -(%d, %d), encap in %d = %.6f \n", n,t,count, ttencap);
	printf("\n(n,t) -(%d, %d), decap in %d = %.6f \n", n,t,count, ttdecap);





    return 0;
}
