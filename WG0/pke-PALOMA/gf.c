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

#include "gf.h"
#include <stdio.h>

/**
* @brief 유한체 F2m 원소 출력 함수
*/
void gf_print(IN gf in){
    int flag =0;

    for(int i=0;i<=16;i++)
    {
        if(in & (1<<(Param_M-i)))
        {
            if(flag)
                printf("+");
            flag =1;    
            printf("Z^%d",Param_M-i);
        }    
    }
    if(in==0)
        printf("0");
    //printf("\n");
}

/**
* @brief 유한체 F2m 덧셈 함수
*/
gf gf_add(IN gf in1, IN gf in2)
{
	return in1 ^ in2;
}

/**
* @brief 유한체 F2m 곱셈 함수
*/
gf gf_mul(IN gf in1, IN gf in2){
    unsigned int t1 = in1;
    gf t2 = in2;
    unsigned int result=0;


    for(int i=1;i<Param_M;i++)
    {
        result ^= (t1 * ( t2 & (1<<(Param_M-i))));            // F2^13 원소이므로 최대 24차.  역순으로 확인 24 > 23 > ... >13 
       
        result ^= (( (result>>(2*Param_M-1-i)) & 1) *(IRR_POLY<<(Param_M-1-i)));       // 현재 확인 차수의 계수가 1이면 z^13+ z^4+ z^3+z+1로 1번 나누어 줌.(최고차항 낮추기)
    }

    result ^= (t1 * ( t2 & 1));         // x1 은 나누기 필요없음. 

    return result;
}

/**
* @brief 유한체 F2m 테이블 이용 곱셈 함수
*/
gf gf_mul_o(IN gf in1, IN gf in2, IN table4mul* tab4mul){   // 테이블 참조를 이용한 곱셈
    gf mask6bit = 0x3F;
    gf mask7bit = 0x7F;

    gf int1front6bit = (in1>>7)&mask6bit;
    gf int1back7bit  = (in1)&mask7bit;
    gf int2front6bit = (in2>>7)&mask6bit;
    gf int2back7bit  = (in2)&mask7bit;

    gf tmp1 = tab4mul->mul6bx6bTab[int1front6bit][int2front6bit];
    gf tmp2 = tab4mul->mul6bx7bTab[int1front6bit][int2back7bit];
    gf tmp3 = tab4mul->mul6bx7bTab[int2front6bit][int1back7bit];
    gf tmp4 = tab4mul->mul7bx7bTab[int1back7bit][int2back7bit];

    gf result = (tmp1 ^ tmp2 ^ tmp3 ^ tmp4);

    return result;
}

/**
* @brief 유한체 F2m 제곱 함수
*/
gf gf_square(IN gf in){                 // 제곱연산
    gf result = in;
    result = gf_mul(result, result);

    return result;
}

/**
* @brief 유한체 F2m 테이블이용 제곱 함수
*/
gf gf_square_o(IN gf in, IN gf* tab4square){    //테이블 참조를 이용한 제곱
    gf result = tab4square[in];
    return result;
}

/**
* @brief 유한체 F2m 제곱근 함수
*/
gf gf_sqrt(IN gf in){               //제곱근 연산
    gf result = in;

    for(int i=0;i<12;i++)
		result=gf_square(result);      // a^(2^12) 

    return result;
}

/**
* @brief 유한체 F2m 테이블 이용 제곱근 함수
*/
gf gf_sqrt_o(IN gf in, IN gf* tab4sqrt){        //테이블 참조를 이용한 제곱근
    
    gf result = tab4sqrt[in];
    return result;
}

/**
* @brief 유한체 F2m 역원 계산 함수
*/
gf gf_inv(IN gf in){            //역원 연산   111 111 111 111 0
    gf a = in;
    gf a_2 = gf_square(a);              // a^2
    gf a_4 = gf_square(a_2);            // a^4
    gf a_6 = gf_mul(a_4,a_2);           // a^6
    gf a_7 = gf_mul(a_6,a);             // a^7
    gf a_63 = a_7;                  

    for(int i =0;i<3;i++)
        a_63 = gf_square(a_63);         // a^7 -> a^14-> a^28 -> a^56

    a_63 = gf_mul(a_63,a_7);            // a^63  = a^56 * a^7

    gf result = a_63;   

    for(int i =0;i<6;i++)
        result = gf_square(result);     //a^4032

    result = gf_mul(result,a_63);       // a^4095
    result = gf_square(result);         // a^8190

    return result;
}

/**
* @brief 유한체 F2m 테이블 이용 역원 계산 함수
*/
gf gf_inv_o(IN gf in, IN gf* tab4inv){  //테이블 참조를 이용한 역원

    gf result = tab4inv[in];
    return result;
}

