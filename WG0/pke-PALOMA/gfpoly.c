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
 
#include "gfpoly.h"
#include <stdio.h>
#include "gf.h"
#include <stdlib.h>
/*
    irreducible polynomial >  X^96 + X^10 + X^9 + X^6+1  //97개 항
*/

/**
* @brief F2m[X]상의 원소 출력 함수
*/
void GF_print(IN gf* in, IN int t){
    int flag =0;

    for(int i=t;i>=0;i--)
    {
        if(in[i] != 0)
        {
            if(flag)
                printf("+");
            flag =1;   
			printf("("); 
			gf_print(in[i]);
			printf(")*");
            printf("X^%d",i);
        }    
    }
    printf("\n");
}

/**
* @brief F2m[X]상의 덧셈
*/
void GF_add(OUT gf* out, IN gf* in1, IN gf* in2, IN int t){  //t차 덧셈. 
    for(int i=0;i<=t;i++){
        out[i] = gf_add(in1[i], in2[i]);
    }
}

/**
* @brief F2m[X]상의 원소 차수 출력.
* 출력이 -1인경우 0임을 의미 
*/
int Deg_poly(gf *in, IN int t){			//t차 이하 다항식의 차수 계산.
	for(int i = t;i>=0;i--)
	{
		if(in[i]!=0)
			return i;
	}
	return -1;	// 다항식이 0인경우.
}

/**
* @brief 다항식 테이블 이용 곱셈.
*/
void POLY_mul_o(OUT gf* out, IN gf* in1, IN gf* in2, IN PARAM* PALOMAParam){   // 다항식 in1 * in2 % gx
	gf result[ MAXT*2-1 ]={0,};	// t차 두개의 곱 저장 

	int t = PALOMAParam->t;

	for (int i = 0; i <= t; i++)
		for (int j = 0; j <= t; j++)
			result[i+j] ^= gf_mul_o(in1[i], in2[j], &PALOMAParam->tab4mul);

	for (int i = 0; i <= t; i++)
		out[i] = result[i];
}

/**
* @brief F2m[X]/<gx(X)>상의 원소 테이블 이용 곱셈.
*/
void GF_mul_o(OUT gf* out, IN gf *gx, IN gf* in1, IN gf* in2, IN PARAM* PALOMAParam){   // 다항식 in1 * in2 % gx
	gf result[ MAXT*2-1 ]={0,};	// 최대 t차 두개의 곱 저장 

	int t = PALOMAParam->t;

	for (int i = 0; i <= t; i++)
		for (int j = 0; j <= t; j++)
			result[i+j] ^= gf_mul_o(in1[i], in2[j], &PALOMAParam->tab4mul);

	int Deg = Deg_poly(gx,t);
	for (int i = t * 2; i >= Deg; i--)
	{
		for(int j = 0; j < Deg + 1; j++) //gx는 Deg차 다항식.
		{
			result[i - Deg + j] ^= gf_mul_o(result[i], gx[j], &PALOMAParam->tab4mul);
		}
	}

	for (int i = 0; i <= Deg; i++)
		out[i] = result[i];
}

/**
* @brief F2m[X]/<gx(X)>상의 원소 테이블 이용 제곱.
*/
void GF_square_o(OUT gf* out, IN gf* gx, IN gf* in, IN PARAM* PALOMAParam){
	gf result[ MAXT*2-1 ]={0,};	// t차 두개의 곱 저장 
	
	int t = PALOMAParam->t;

	for (int i = 0; i <= t; i++)
		result[2*i] ^= gf_sqrt_o(in[i], PALOMAParam->tab4square);

	int Deg = Deg_poly(gx,t);

	for (int i = t * 2; i >= Deg; i--){
		for(int j = 0; j < Deg + 1; j++) { //gx는 t차 다항식.
			result[i - Deg + j] ^= gf_mul_o(result[i], gx[j],&PALOMAParam->tab4mul);
		}
	}

	for (int i = 0; i <= Deg; i++)
		out[i] = result[i];
}

/**
* @brief F2m[X]/<gx(X)>상의 원소 테이블 이용 제곱근.
*/
void GF_sqrt_o(OUT gf* out, IN gf* gx, IN gf* in, IN PARAM* PALOMAParam){

	gf result[ MAXT*2-1 ]={0,}; 
	
	int t = PALOMAParam->t;

	for (int i = 0; i <= t; i++)
		result[i] = in[i];	

	for(int i=0;i<(13*t)-1;i++){	// mt-1	번 제곱
		GF_square_o(result, gx, result, PALOMAParam);
	}

	for (int i = 0; i <= t; i++)
		out[i] = result[i];

}

/**
* @brief F2m[X]/<gx(X)>상의 원소 테이블 이용 역원.
*/
void GF_inv_o(OUT gf* out, IN gf* in, IN gf *gx, IN PARAM* PALOMAParam){
	gf tmp[MAXT]={0,}, tmp2[MAXT]={0,};
	int t = PALOMAParam->t;

	for(int i=0;i<=t;i++)
		tmp[i]=in[i];
	
	EEA_gx(tmp2,tmp,gx,PALOMAParam);

	for (int i = 0; i <= t; i++)
		out[i] = tmp2[i];

}

/**
* @brief 다항식 extended euclidean algorithm.
*/
void EEA_gx(OUT gf *out, IN gf *in, IN gf *gx, IN PARAM* PALOMAParam){  //in1^{-1} mod in2= out 
	gf t0[MAXT]={0,} ,t1[MAXT]={0,} ,v0[MAXT]={0,} ,v1[MAXT]={0,} ;
	gf qq[MAXT]={0,} ,R[MAXT]={0,} ,v2[MAXT]={0,} ;
	gf inverse;
	int t = PALOMAParam->t;
	
	v1[0]=1;		//v1(X) = 1

	for(int i=0;i<t+1;i++){
		t0[i]=gx[i];			//gx가 크니까 앞으로
		t1[i]=in[i];
	}

	while(Deg_poly(t1,t)>=0){
		GF_remind(qq,R,t0,t1,PALOMAParam);

		for(int i=0;i<t+1;i++)
		{
			t0[i]=t1[i];
			t1[i]=R[i];
		}

		GF_mul_o(v2,gx,qq,v1,PALOMAParam);
		for(int i=0;i<t+1;i++)
			v2[i]^=v0[i];	

		for(int i=0;i<t+1;i++)
		{
			v0[i]=v1[i];
			v1[i]=v2[i];
			v2[i]=0;
		}  
	}
	inverse=PALOMAParam->tab4inv[t0[0]];
	for(int i=0;i<=t;i++)	//출력은 t-1차만
	{
		out[i]=gf_mul_o(inverse,v0[i],&PALOMAParam->tab4mul);
	} 
} 

/**
* @brief 다항식 나눗셈 
* a = b * quo + rem
*/
void GF_remind(OUT gf *quo, OUT gf *rem, IN gf *a, IN gf *b, IN PARAM* PALOMAParam){ //  a/b=quo,  a mod b = rem  (in poly)
	gf Q[MAXT]={0,} ,R[MAXT]={0,}  ,B[MAXT]={0,}  ,tmp[MAXT]={0,}  ,S[MAXT]={0,} , temp, c;
	int d,r;
	int t = PALOMAParam->t;

	for (int i =0; i < t+1; i++)
	{	
		R[i]=a[i];
		B[i]=b[i];
	}

	d=Deg_poly(b,t);
	c=b[d];

	while((Deg_poly(R,t)>=d)&&(d>0))
	{
		r=Deg_poly(R,t);
		temp = gf_inv_o(c,PALOMAParam->tab4inv);

		temp= gf_mul_o(R[r],temp,&PALOMAParam->tab4mul);  //temp = R[r]/c 

		S[r-d]^=temp;
		Q[r-d]=S[r-d];

		for(int j=d;j>=0;j--)
		{
			tmp[j+r-d]^=gf_mul_o(temp,B[j],&PALOMAParam->tab4mul);
		}

		for (int i =0; i <= t; i++)
		{	
			R[i]^=tmp[i];
			S[i]=0;
		}	
		for(int i=0;i<=t;i++)
			tmp[i]=0;
	
	}

	if(d==0)
	{
		temp = gf_inv_o(B[0],PALOMAParam->tab4inv);
		for(int i=0;i<t+1;i++)
		{
			quo[i]=gf_mul_o(R[i],temp,&PALOMAParam->tab4mul);
			rem[i]=0;
		}	
	}
	else{
		for(int i=0;i<t+1;i++)
		{
			quo[i]=Q[i];
			rem[i]=R[i];
		}
	}
}

/**
* @brief 다항식 evaluation
*/
gf GF_eval(IN gf* polyf , IN gf gf_a, IN int deg_f, IN PARAM* PALOMAParam){ //honor_method

	gf result=polyf[deg_f];
	for(int i=deg_f-1;i>=0;i--){
		result = gf_mul_o(result, gf_a,&PALOMAParam->tab4mul);
		result = gf_add(result,polyf[i]);
	}
	return result;
}

/**
* @brief 다항식 greatest common division 계산
*/
void GF_GCD(OUT gf *gcd, IN gf *poly1, IN gf *poly2, IN PARAM* PALOMAParam){
	int t = PALOMAParam->t;

	gf r1[MAXT]={0,};
	gf r2[MAXT]={0,};
	gf r3[MAXT]={0,};

	for(int i=0;i<=t;i++){
		r1[i]=poly1[i];
		r2[i]=poly2[i];
	}

	while (Deg_poly(r2,t)>=0){
		gf Q[MAXT]={0,};

		GF_remind(Q, r3, r1, r2, PALOMAParam);
		for(int i=0;i<=t;i++){
			r1[i]=r2[i];
			r2[i]=r3[i];
		}
	}
	int tmp = Deg_poly(r1,t);
	gf inverse=PALOMAParam->tab4inv[r1[tmp]];

	for(int i=0;i<=t;i++){
		gcd[i]=gf_mul_o(inverse,r1[i],&PALOMAParam->tab4mul);
	}

}

/**
* @brief 다항식 연산 검증
*/
void GF_verify(IN gf* gx, IN PARAM* PALOMAParam){
    printf("q=2\n");
    printf("m=13\n");
    printf("t=128\n");
    printf("Fq.<z> = GF(q)\n");
    printf("Rq.<x> = PolynomialRing(Fq)\n");
    printf("f = x^13 + x^7 + x^6 + x^5 + 1\n");
    printf("Fqm.<Z> = Fq.extension(f)\n");
    printf("Rqm.<X> = PolynomialRing(Fqm)\n");
    printf("g = X^128 + X^7 + X^2 + X^1+1\n");
    
	gf in1[129] = {0,};
    gf in2[129] = {0,};
	gf out[129]={0,};

	int count = rand()%20;
    for(int i=0;i<count;i++)
    {
        int random = rand()%128;
        in1[random] = rand()%0x1FFF;
    }
    for(int i=0;i<count;i++)
    {
        int random = rand()%128;
        in2[random] = rand()%0x1FFF;
    }
	for(int i=0;i<100;i++){
    printf("a = ");     GF_print(in1,PALOMAParam->t);
    printf("b = ");     GF_print(in2,PALOMAParam->t);
    
    GF_add(out, in1,in2,PALOMAParam->t);
    GF_print(out,PALOMAParam->t);
    printf("out = ");  GF_print(out,PALOMAParam->t);
    printf("print('addition: ',end='')\n");
    printf("print(a+b==out)\n");

    GF_mul_o(out, gx, in1,in2, PALOMAParam);
    printf("out = ");  GF_print(out,PALOMAParam->t);
    printf("print('multi: ',end='')\n");
    printf("print((a*b)%%g==out)\n");

    GF_square_o(out, gx, in1, PALOMAParam);
    printf("square = ");  GF_print(out,PALOMAParam->t);
    printf("print('square: ',end='')\n");
    printf("print((a*a)%%g==square)\n");

    GF_sqrt_o(out, gx, out, PALOMAParam);
    printf("sqrt = ");  GF_print(out,PALOMAParam->t);
    printf("print('sqrt: ',end='')\n");
    printf("print(sqrt == a)\n");

    GF_inv_o(out, in1, gx,PALOMAParam);
    printf("inv = ");  GF_print(out,PALOMAParam->t);
    printf("\nprint('inverse: ',end='')\n");
    printf("print(a.inverse_mod(g) == inv)\n");

    GF_mul_o(out, gx, out,in1,PALOMAParam);
    printf("invaa = ");  GF_print(out,PALOMAParam->t);
	
	}
}