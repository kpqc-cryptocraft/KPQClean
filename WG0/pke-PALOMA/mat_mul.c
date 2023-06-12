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
 
#include "mat_mul.h"
#include <stdio.h>
#include <time.h>

/**
* @brief 행렬 x 벡터 
* @param inmat  입력 행렬
* @param invec  입력 벡터
* @param rownum 입력 행렬의 행 개수
* @param colnum 입력 행렬의 열 개수 (입력벡터의 길이)
* @param outvec 출력 벡터
*/
void matXvec(OUT u64* outvec, IN u64* inmat, IN u64* invec, IN int rownum, IN int colnum){ // 모든 64비트는 열단위.
    int row64num = rownum / 64;

    for(int i=0;i<colnum;i++){
        if((invec[i/64]>>(i%64))&1){
            for(int j=0;j<row64num;j++)
                outvec[j]^= inmat[(i*row64num)+j];
        }
    }
}

/**
* @brief 행렬 x 행렬
* @param inmat1  입력 행렬1
* @param inmat2  입력 행렬2
* @param mat1rownum 입력 행렬1의 행 개수
* @param mat1colnum 입력 행렬1의 열 개수 (입력 행렬2의 행개수)
* @param mat2colnum 입력 행렬2의 열 개수
* @param outmat 출력 행렬
*/
void matXmat(OUT u64* outmat, IN u64* inmat1, IN u64* inmat2, IN int mat1rownum, IN int mat1colnum, IN int mat2colnum){ // 모든 64비트는 열단위.
    int mat1row64num = mat1rownum / 64;
    int mat2row64num = mat1colnum / 64;

    u64 tmp[MAXN/64]={0,};

    for(int i=0;i<mat2colnum;i++){// 두번째 행렬 열개수만큼 반복
        u64 tmp2[MAXN/64]={0,};
        for(int j=0;j<mat2row64num;j++){
            tmp[j]=inmat2[(i*mat2row64num)+j];            
        }
        matXvec(tmp2, inmat1, tmp, mat1rownum, mat1colnum);

        for(int j=0;j<(mat1row64num);j++){
            outmat[(i*mat1row64num)+j] = tmp2[j];
        }
    }
}


/**
* @brief 항등행렬 생성
* @param row  행렬의 행 개수
*/
void Gen_Identity_Mat(OUT u64* out, IN int row){
    int rowgf = row/64;
    u64 a=1;
    for(int i=0;i<row;i++){
        out[(i*rowgf)+(i/64)] = a<<(i%64);
    }
}

/**
* @brief gaussian elimination
* @param in_mat  입력 행렬
* @param rownum  입력 행렬 행개수
* @param colnum  입력 행렬 열개수
* @param out     출력
*/
int Gaussian_row(OUT u64* out, IN u64* in_mat, IN int rownum, IN int colnum){
    int row64num = rownum >> 6;
    int col64num = colnum >> 6; //int row64num=rownum/64;
    
    u64 M[13*MAXT][MAXN/64]={{0,},};  //H  행렬로 하여 역행렬 계산.
    u64 tmp2;
    
    for(int i=0;i<(rownum*col64num);i++){
        M[i/col64num][i%col64num] = in_mat[i];
    }

    int r=0;
    int flag=0;
    while(r<rownum){
        flag=0;
        for(int i=r;i<rownum;i++){
            if(((M[i][r/64]>>(r%64))&1)==1)
            {  
                if(i!=r){
                    for(int j=0;j<col64num;j++){  // M행렬 i,r 번째 행 스왑. 
                        tmp2 = M[i][j];
                        M[i][j] = M[r][j];
                        M[r][j] = tmp2;
                    }
                }
                flag=1;
            }
            if(flag==1){        
                for(int j=0;j<rownum;j++){
                    if((j!=r)&&((M[j][r/64]>>(r%64))&1)){
                        for(int k=0;k<col64num;k++){
                            M[j][k] ^=  M[r][k];
                        }
                    }
                }
                break;
            }
        }
        if(flag==0)
            return -1;
        r++;
    }

    int count =0;
    for(int j=0;j<colnum;j++){
        for(int i=0;i<rownum;i++){
            out[(i/64)+(j*row64num)] |= ((u64)((M[i][j/64]>>(j%64))&1)<<(i%64));
        }
    }

    return 0;

}