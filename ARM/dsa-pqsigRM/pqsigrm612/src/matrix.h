#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MATRIX_NULL 0 
#define ELEMBLOCKSIZE 8

#define INV_SUCCESS 1
#define INV_FAIL 0

#define getElement(A, i, j) 		(!!((A)->elem[(i) * (A)->rwdcnt + (j) / ELEMBLOCKSIZE] & (0x80 >> ((j) % ELEMBLOCKSIZE))))
#define flipElement(A, i, j) 	((A)->elem[(i) * (A)->rwdcnt + (j) / ELEMBLOCKSIZE] ^= (0x80 >> ((j) % ELEMBLOCKSIZE)))
#define setElement(A, i, j, val) 	((getElement((A), (i), (j)) == (val))? 0 : flipElement((A), (i),(j)))
#define initZero(R) 			memset((R)->elem,0,(R)->alloc_size)

typedef struct {
   int rows;//number of rows.
   int cols;//number of columns.
   int rwdcnt;//number of words in a row
   int alloc_size;//number of allocated bytes
   unsigned char *elem;//row index.
} matrix;

matrix* newMatrix(int rows, int cols) ;
void deleteMatrix(matrix * mtx) ;

matrix* rref(matrix* mtx);
matrix* transpose(matrix *dest, matrix *src);
int inverse(matrix *mtx, matrix *mtxInv);
int isNonsingular(matrix *mtx);

void getPivot(matrix* mtx, uint16_t *lead, uint16_t *lead_diff);

matrix* matrixcpy(matrix* dest, matrix* src);

int product(matrix * mtx1, matrix * mtx2, matrix * prod); 
void vector_mtx_product(matrix *dest, matrix* m, matrix *vec);
int add(matrix *m1, matrix *m2, matrix *res);

int exportMatrix(unsigned char* dest, matrix* mtx);
matrix* importMatrix(matrix* dest_mtx, const unsigned char* src);

void dual(matrix* G, matrix* H_sys, uint16_t *lead, uint16_t *lead_diff);
void rowInterchanging(matrix* mtx, int row_idx1, int row_idx2);
void mtxcpy(matrix* dest, const int r1, const int c1,const int r2, const int c2, matrix* src, const int r3, const int c3);
#endif
