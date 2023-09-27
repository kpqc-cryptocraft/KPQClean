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

#include "gf_table_gen.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
* @brief 생성한 테이블 출력함수 (확인용)
*/
void print_all_tab(){
    table4mul tabmul;
    gf tabsquare[(1<<Param_M)];  // 제곱을 위한 테이블
    gf tabsqrt[(1<<Param_M)];    // 제곱근을 위한 테이블
    gf tabinv[(1<<Param_M)];     // 역원을 위한 테이블

    gen_gfmul_tab(&tabmul); 
    gen_gfsquare_tab(tabsquare);
    gen_gfsqrt_tab(tabsqrt);
    gen_gfinv_tab(tabinv);

    print_gfmul_tab(&tabmul);
    print_gfsquare_tab(tabsquare);
    print_gfsqrt_tab(tabsqrt);
    print_gfinv_tab(tabinv);
}

/**
* @brief 사전계산 테이블 생성함수
*/
void GF_init(OUT PARAM* PALOMAParam, IN int n, IN int k, IN int t){
    gen_gfsquare_tab(PALOMAParam->tab4square);
    gen_gfsqrt_tab(PALOMAParam->tab4sqrt);
    gen_gfinv_tab(PALOMAParam->tab4inv);
    
    PALOMAParam->n = n;
    PALOMAParam->k = k;
    PALOMAParam->t = t;

    gen_gfmul_tab(&PALOMAParam->tab4mul); 
}

/**
* @brief 유한체 곱셈 사전계산 테이블 생성함수
*/
void gen_gfmul_tab(OUT table4mul* tab4mul){     // 상위 6 비트, 하위 7비트로 나누어 곱셈 테이블 생성

    gf ishift7;
    gf jshift7;

    for(gf i=0;i<(1<<7);i++){
        for(gf j=0;j<(1<<7);j++){
        
            tab4mul->mul7bx7bTab[i][j] = gf_mul(i,j);       // 하위 7bit간 연산
            
            if( (i<(1<<6)) && (j<(1<<6)) )
            {
                ishift7 = (i<<7);           // 곱하기 z^7
                jshift7 = (j<<7);           // 곱하기 z^7

                tab4mul->mul6bx6bTab[i][j] = gf_mul(ishift7,jshift7);          //상위 6bit간 연산
            }

            if( i<(1<<6) )
            {
                ishift7 = (i<<7);       // 곱하기 z^7
                
                tab4mul->mul6bx7bTab[i][j] = gf_mul(ishift7,j);         //상위 6bit x 하위 7bit
            }
        }
    }
}

/**
* @brief 유한체 제곱 사전계산 테이블 생성함수
*/
void gen_gfsquare_tab(OUT gf* tab4square){      
    for(gf i=0;i<(1<<13);i++){
        tab4square[i] = gf_square(i);           //모든 원소 제곱 테이블
    }
}

/**
* @brief 유한체 제곱근 사전계산 테이블 생성함수
*/
void gen_gfsqrt_tab(OUT gf* tab4sqrt){
    for(gf i=0;i<(1<<13);i++){
        tab4sqrt[i] = gf_sqrt(i);           //모든 원소 제곱근 테이블
    }
}

/**
* @brief 유한체 역원 사전계산 테이블 생성함수
*/
void gen_gfinv_tab(OUT gf* tab4inv){
    for(gf i=0;i<(1<<13);i++){
        tab4inv[i] = gf_inv(i);              //모든 원소 역원 테이블
    }
}


/**
* @brief 유한체 곱셈 사전계산 테이블 출력함수
*/
void print_gfmul_tab(IN table4mul* tab4mul){        //곱셈 테이블 출력
    printf("table4mul tab4mul ={ {");
    int count=0;
    for(gf i=0;i<(1<<6);i++){
        printf("{");
        for(gf j=0;j<(1<<6);j++){
            printf("0x%04x",tab4mul->mul6bx6bTab[i][j]);
            if(j!=0x3F)
                printf(", ");
            count++;
            if((count%20)==0)    
                printf("\n      ");
        }
        printf("}");
        if(i!=0x3F) printf(",");
    }
    printf("},\n");

    printf("   {");
    count=0;
    for(gf i=0;i<(1<<6);i++){
        printf("{");
        for(gf j=0;j<(1<<7);j++){
            printf("0x%04x",tab4mul->mul6bx7bTab[i][j]);
            if(j!=0x7F)
                printf(", ");
            count++;
            if((count%20)==0)    
                printf("\n      ");
        }
        printf("}");
        if(i!=0x3F) printf(",");
    }
    printf("},\n");

    printf("   {");

    count=0;
    for(gf i=0;i<(1<<7);i++){
        printf("{");
        for(gf j=0;j<(1<<7);j++){
            printf("0x%04x",tab4mul->mul7bx7bTab[i][j]);
            if(j!=0x7F)
                printf(", ");
            count++;
            if((count%20)==0)    
                printf("\n      ");

        }
        printf("}");
        if(i!=0x7F) printf(",");
    }
    printf("}};\n");
}

/**
* @brief 유한체 제곱 사전계산 테이블 출력함수
*/
void print_gfsquare_tab(IN gf* tab4square){         //제곱 테이블 출력
    printf("gf tab4square [(1<<Param_M)] = {");
    for(gf j=0;j<(1<<13);j++){
        printf("0x%04x",tab4square[j]);
        if(j!=0x1FFF)
            printf(", ");
        if((j+1)%20==0)    
            printf("\n      ");

    }
    printf("};\n");
}

/**
* @brief 유한체 제곱근 사전계산 테이블 출력함수
*/
void print_gfsqrt_tab(IN gf* tab4sqrt){         //제곱근 테이블 출력
    printf("gf tab4sqrt [(1<<Param_M)] = {");
    
    for(gf j=0;j<(1<<13);j++){
        printf("0x%04x",tab4sqrt[j]);
        if(j!=0x1FFF)
            printf(", ");
        if((j+1)%20==0)    
            printf("\n      ");
    
    }

    printf("};\n");
}

/**
* @brief 유한체 역원 사전계산 테이블 출력함수
*/
void print_gfinv_tab(IN gf* tab4inv){           //역원 테이블 출력.
    printf("gf tab4inv [(1<<Param_M)] = {");
    
    for(gf j=0;j<(1<<13);j++){
        printf("0x%04x",tab4inv[j]);
        if(j!=0x1FFF)
            printf(", ");
        if((j+1)%20==0)    
            printf("\n      ");
    }       
    printf("};\n");
}

/**
* @brief 유한체 연산 테이블 성능 측정 함수
*/
void measure_performance(IN PARAM* PALOMAParam){         //시간 측정

	clock_t start, end;
	double res;
    int count = 10000000;

    /* 곱셈 속도 비교 */
    gf finite_a, finite_b , d;

    finite_a = rand()%0x1FFF;
    finite_b = rand()%0x1FFF;
    gf finite_c = finite_a;

    printf("a = \n");    gf_print(finite_a);
    printf("b = \n");    gf_print(finite_b);

    start = clock();
	for(int i=0; i<=count; i++ )
	{
        finite_c=gf_add(finite_a,finite_b);
        d=gf_add(finite_a,finite_b);
        finite_c=gf_add(finite_c,d);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("일반 덧셈 %d회 시간        : %lf\n",count, res);

	start = clock();
	for(int i=0; i<=count; i++ )
	{
        finite_a=gf_mul(finite_a,finite_b);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("일반 곱셈 %d회 시간        : %lf\n",count, res);

	start = clock();
	for(int i=0; i<=count; i++ )
	{    
        finite_a=gf_mul_o(finite_a,finite_b,&PALOMAParam->tab4mul);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("테이블 참조 곱셈 %d회 시간 : %lf\n",count, res);


    /* 제곱 속도 비교 */
    finite_a = rand()%0x1FFF;

	start = clock();
	for(int i=0; i<=count; i++ )
	{
        finite_b=gf_square(finite_a);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("일반 제곱 %d회 시간        : %lf\n",count, res);

	start = clock();
	for(int i=0; i<=count; i++ )
	{    
        finite_b=gf_square_o(finite_a,PALOMAParam->tab4square);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("테이블 참조 제곱 %d회 시간 : %lf\n",count, res);

    /* 제곱근 속도 비교 */
    finite_a = rand()%0x1FFF;

	start = clock();
	for(int i=0; i<=count; i++ )
	{
        finite_b=gf_sqrt(finite_a);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("일반 제곱근 %d회 시간       : %lf\n",count, res);

	start = clock();
	for(int i=0; i<=count; i++ )
	{    
        finite_b=gf_sqrt_o(finite_a,PALOMAParam->tab4sqrt);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("테이블 참조 제곱근 %d회 시간: %lf\n",count, res);

    /* 역원 속도 비교 */

   finite_a = rand()%0x1FFF;

	start = clock();
	for(int i=0; i<=count; i++ )
	{
        finite_b=gf_inv(finite_a);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("일반 역원 %d회 시간        : %lf\n",count, res);

	start = clock();
	for(int i=0; i<=count; i++ )
	{    
        finite_b=gf_inv_o(finite_a,PALOMAParam->tab4inv);
	}
	end = clock();
	res = (double)(end - start)/CLOCKS_PER_SEC;

    printf("테이블 참조 역원 %d회 시간 : %lf\n",count, res);

}


/**
* @brief 유한체 테이블 검증 함수
*/
void tab_verify_check(IN PARAM* PALOMAParam){

    int count = 10000;

    /* 곱셈 검증 */
    gf finite_a, finite_b;

    finite_a = rand()%0x1FFF;
    finite_b = rand()%0x1FFF;
    gf finite_c = finite_a;

    printf("a = \n");    gf_print(finite_a);
    printf("b = \n");    gf_print(finite_b);

	for(int i=0; i<=count; i++ )
	{
        finite_a=gf_mul(finite_a,finite_b);
        finite_c=gf_mul_o(finite_c,finite_b,&PALOMAParam->tab4mul);
        if(finite_a!=finite_c)
            printf("error!!\n");
	}
    printf("곱셈 체크 완료\n");

    /* 제곱 검증 */
    finite_a = rand()%0x1FFF;
    finite_c = finite_a;

	for(int i=0; i<=count; i++ )
	{
        finite_a=gf_square(finite_a);
        finite_c=gf_square_o(finite_c,PALOMAParam->tab4square);
        if(finite_a!=finite_c)
            printf("error!!\n");
	}
    printf("제곱 체크 완료\n");

    /* 제곱근 검증 */
    finite_a = rand()%0x1FFF;
    finite_c = finite_a;

	for(int i=0; i<=count; i++ )
	{
        finite_a=gf_sqrt(finite_a);
        finite_c=gf_sqrt_o(finite_c,PALOMAParam->tab4sqrt);
        if(finite_a!=finite_c)
            printf("error!!\n");
	}
    printf("제곱근 체크 완료\n");

    /* 역원 검증 */
    finite_a = rand()%0x1FFF;
    finite_c = finite_a;

	for(int i=0; i<=count; i++ )
	{
        finite_a=gf_inv(finite_a);
        finite_c=gf_inv_o(finite_c,PALOMAParam->tab4inv);
        if(finite_a!=finite_c)
            printf("error!!\n");
    }
    printf("역원 체크 완료\n");


}