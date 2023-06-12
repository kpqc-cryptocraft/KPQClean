#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MATRIX_NULL 0 
#define ELEMBLOCK uint64_t
#define ELEMBLOCKSIZE 64

#define INV_SUCCESS 1
#define INV_FAIL 0

#define get_element(A, i, j) 		(!!((A)->elem[(i) * (A)->words_in_row + (j) / ELEMBLOCKSIZE] & ((0x8000000000000000UL) >> ((j) % ELEMBLOCKSIZE))))
#define flip_element(A, i, j) 	((A)->elem[(i) * (A)->words_in_row + (j) / ELEMBLOCKSIZE] ^= ((0x8000000000000000UL) >> ((j) % ELEMBLOCKSIZE)))
#define set_element(A, i, j, val) 	((get_element((A), (i), (j)) == (val))? 0 : flip_element((A), (i),(j)))
#define init_zero(R) 			memset((R)->elem, 0, (R)->alloc_size)

typedef struct {
   uint32_t nrows;//number of rows.
   uint32_t ncols;//number of columns.
   uint32_t words_in_row;//number of words in a row
   uint32_t alloc_size;//number of allocated bytes
   ELEMBLOCK* elem;//row index.
} matrix;

matrix* new_matrix(uint32_t nrows, uint32_t ncols) ;
void delete_matrix(matrix * mtx) ;

matrix* copy_matrix(matrix* dest, matrix* src);
int export_matrix(unsigned char* dest, matrix* mtx);
matrix* import_matrix(matrix* dest_mtx, const unsigned char* src);

matrix* rref(matrix* mtx);
matrix* transpose(matrix *src, matrix *dest);
int inverse(matrix *mtx, matrix *mtxInv);
int is_nonsingular(matrix *mtx);

void get_pivot(matrix* mtx, uint16_t *lead, uint16_t *lead_diff);

void mat_mat_prod(matrix * mtx1, matrix * mtx2, matrix * prod); 
void vec_mat_prod(matrix *dest, matrix* m, matrix *vec);
int mat_mat_add(matrix *m1, matrix *m2, matrix *res);

void dual(matrix* G, matrix* H_sys, uint16_t *lead, uint16_t *lead_diff);
void row_interchange(matrix* mtx, uint32_t row_idx1, uint32_t row_idx2);
void partial_replace(matrix* dest, const uint32_t r1, const uint32_t c1,const uint32_t r2, const uint32_t c2, matrix* src, const int r3, const int c3);

void codeword(matrix* src, uint8_t* seed, matrix* dest);

#endif
