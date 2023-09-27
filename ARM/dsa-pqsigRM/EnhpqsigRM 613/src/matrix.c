#include "matrix.h"

matrix* new_matrix (uint32_t nrows, uint32_t ncols)
{
  matrix* A;

  A = (matrix*) malloc (sizeof (matrix));
  A->ncols = ncols;
  A->nrows = nrows;  
  A->words_in_row = (1 + (ncols - 1) / ELEMBLOCKSIZE);
  A->alloc_size = nrows * A->words_in_row * sizeof(ELEMBLOCK);
  A->elem = (ELEMBLOCK*)malloc(A->alloc_size);
  init_zero(A);
  return A;
}

void delete_matrix(matrix* A)
{
	// fprintf(stderr, "elem pointer %p\n", A->elem);	
	free(A->elem);
	// fprintf(stderr, "free elem\n");
	free(A);
	// fprintf(stderr, "free A\n");
}

// internal method
void row_addition_internal(matrix* A, int dest_row_idx, int adding_row_idx){
	for(uint32_t col_idx=0; col_idx < A->words_in_row; ++col_idx){
		A->elem[dest_row_idx * A->words_in_row + col_idx] 
			^= A->elem[adding_row_idx * A->words_in_row + col_idx];
	}
}

matrix* copy_matrix(matrix* dest, matrix* src){
	assert(dest->nrows == src->nrows && dest->ncols == src->ncols);
	
	memcpy(dest->elem, src->elem, src->alloc_size);
	return dest;
}

// Exports a matrix into unsigned char destination.
int export_matrix(unsigned char* dest, matrix* src_mtx){
	memcpy(dest, src_mtx->elem, src_mtx->alloc_size);
	return src_mtx->alloc_size;
}

matrix* import_matrix(matrix* dest_mtx, const unsigned char* src){
	memcpy(dest_mtx->elem, src, dest_mtx->alloc_size);

	return dest_mtx;
}

matrix* rref(matrix* A)
{
	// Assume column is longer than row
	uint32_t succ_row_idx = 0;
	uint32_t col_idx, row_idx = 0;
	for (col_idx = 0; col_idx < (A->ncols); ++col_idx) {
		
		// finding first row s.t. i th elem of the row is 1
		for(; row_idx < A->nrows; ++row_idx)
			if(get_element(A, row_idx, col_idx) == 1) 
				break;
		// When reaches the last row, increase column index and search again
		if (row_idx == A->nrows){ 
			row_idx = succ_row_idx;
			continue;
		}
		// if row_idx is not succ_row_idx, 
		// interchange between:
		// <succ_row_idx> th row <-> <row_idx> th row
		if(row_idx != succ_row_idx){
			row_interchange(A, succ_row_idx, row_idx);
		}
				
		// By adding <succ_row_idx> th row in the other nrows 
		// s.t. A(i, <succ_row_idx>) == 1,
		// making previous columns as element row.
		for(uint32_t i = 0; i < A->nrows; ++i){
			if(i == succ_row_idx) continue;

			if(get_element(A, i, col_idx) == 1){
				row_addition_internal(A, i, succ_row_idx);
			}
		}
		row_idx = ++succ_row_idx;
	}
	//Gaussian elimination is finished. So return A.
	return A;
}

matrix* transpose(matrix *src, matrix *dest){
	assert((dest->nrows == src->ncols) && (dest->ncols == src->nrows));

	for(uint32_t row = 0; row < dest->nrows; ++row){
		for(uint32_t col = 0; col < dest->ncols; ++col){
			set_element(dest, row, col, get_element(src, col, row));
		}
	}
		
	return dest;
}

int inverse(matrix *mtx, matrix *mtxInv){
	if(mtx->nrows != mtx->ncols) 			return INV_FAIL;
	if(mtxInv->nrows != mtxInv->ncols) 	return INV_FAIL;
	if(mtx->nrows != mtxInv->nrows) 		return INV_FAIL;

	matrix* temp = new_matrix(mtx->nrows, mtx->ncols);
	copy_matrix(temp, mtx);

	uint32_t r, c;
	init_zero(mtxInv);

	for (r = 0; r <  mtxInv->nrows; ++r)
	{
		set_element(mtxInv, r, r, 1);
	}

	for (c = 0; c < temp->ncols; ++c)
	{
		if(get_element(temp, c, c) == 0)
		{	
			for (r = c+1; r < mtx->nrows; ++r)
			{
				if(get_element(temp, r, c) != 0){
					row_interchange(temp, r, c);
					row_interchange(mtxInv, r, c);
					break;
				}
			}
			if(r >= temp->nrows) 		return INV_FAIL;
		}
		
		for(r = 0; r < temp->nrows; r++){
			if(r == c) continue;
			if(get_element(temp, r, c) != 0){
				row_addition_internal(temp, r, c);
				row_addition_internal(mtxInv, r, c);
			}
		}
	}
	
	// fprintf(stderr, "delete temp\n");
	delete_matrix(temp);
	return INV_SUCCESS;
}

int is_nonsingular(matrix *mtx){

	matrix* temp = new_matrix(mtx->nrows, mtx->ncols);
	copy_matrix(temp, mtx);

	uint32_t r, c;

	for (c = 0; c < temp->ncols; ++c)
	{
		if(get_element(temp, c, c) == 0)
		{	
			for (r = c+1; r < mtx->nrows; ++r)
			{
				if(get_element(temp, r, c) != 0){
					row_interchange(temp, r, c);
					break;
				}
			}
			if(r >= temp->nrows) 		
				return INV_FAIL;
		}

		for(r = 0; r < temp->nrows; r++){
			if(r == c) continue;
			if(get_element(temp, r, c) != 0){
				row_addition_internal(temp, r, c);
			}
		}
	}

	delete_matrix(temp);
	return INV_SUCCESS;
}

void get_pivot(matrix* mtx, uint16_t *lead, uint16_t *lead_diff){
	uint32_t row=0, col=0;
	uint32_t lead_idx=0, diff_idx=0;

	while((col < mtx->ncols) && (row < mtx->nrows) && (lead_idx < mtx->nrows) && (diff_idx < (mtx->ncols - mtx->nrows))){
		if(get_element(mtx, row, col) == 1UL){
			lead[lead_idx++] = col;
			row++;
		}
		else{
			lead_diff[diff_idx++] =col;
		}
		col++;
	}

	while(col < mtx->ncols){
		lead_diff[diff_idx++] = col++;
	}
}

void mat_mat_prod(matrix * mtx1, matrix * mtx2, matrix * prod) {
	uint32_t row, col, k;
	uint32_t val;

	assert(mtx1->ncols == mtx2->nrows);
	
	for (row = 0; row < mtx1->nrows; row++){ 
		for (col = 0; col < mtx2->ncols; col++) {
			val = 0;
			for (k = 0; k < mtx1->ncols; k++)
				val ^= get_element(mtx1, row, k) & get_element(mtx2, k, col);
			set_element(prod, row, col, val);
		}
	}
}

//assume vector is transposed
void vec_mat_prod(matrix *dest, matrix* m, matrix *vec){
	uint64_t bit = 0;
	uint64_t offset;
	uint32_t row, col_word;
	for(row = 0; row < m->nrows; row++){
		bit = 0;
		//assume all zero bit in unnecessary position
		for(col_word = 0; col_word < m->words_in_row; col_word++){
			bit ^= m->elem[(m->words_in_row)*row + col_word] & vec->elem[col_word];
		}
		
		// for(col=0; col < m->words_in_row - 1; col++)
		// 	bit ^= m->elem[row*m->words_in_row + col] & vec->elem[col];
	
		// offset = 0xffffffffffffffffUL << (ELEMBLOCKSIZE*m->words_in_row - m->ncols);
		// bit ^= (m->elem[row*m->words_in_row + col] & vec->elem[col]) & offset;
		// printf("bit: \n");
		// for (size_t i = 0; i < 64; i++)
		// {	
		// 	if(i % 4 == 0){
		// 		printf(" ");
		// 	}
		// 	printf("%d", (bit >> (ELEMBLOCKSIZE - 1 - i)) & (uint64_t)1);
		// }printf("\n32\n");

		bit ^= (bit >> 32);
		bit ^= (bit >> 16);
		bit ^= (bit >> 8);
		bit ^= (bit >> 4);
		bit ^= (bit >> 2);
		bit ^= (bit >> 1);
		bit &= (uint64_t)1;
		
		set_element(dest, 0, row, bit);
	}
}

int mat_mat_add(matrix *m1, matrix *m2, matrix *res){
	if((m1->nrows != m2->nrows) || (m1->ncols != m2->ncols))
		return -1;
	
	for(int i =0; i< res->alloc_size; i++)
		res->elem[i] = (m1->elem[i])^(m2->elem[i]);

	return 0;
}

void dual(matrix* G, matrix* H_sys, uint16_t *lead, uint16_t *lead_diff){
	uint8_t flg = 0; 

	if(lead == 0 || lead_diff == 0){
		lead = (uint16_t*)malloc(sizeof(uint16_t)*G->nrows);
		lead_diff = (uint16_t*)malloc(sizeof(uint16_t)*(G->ncols - G->nrows));	
		flg = 1;
	}

	rref(G);
	get_pivot(G, lead, lead_diff);

	// Fill not-identity part (P')
	for (uint32_t row = 0; row < H_sys->nrows; row++) 
		for (uint32_t col = 0; col < G->nrows; col++) 
			set_element(H_sys, row, lead[col], get_element(G, col, lead_diff[row]));
	
	for (uint32_t row = 0; row < H_sys->nrows; row++) 
			set_element(H_sys, row, lead_diff[row], 1);
	
	if(flg == 1){
		free(lead);
		free(lead_diff);
	}
}

void row_interchange(matrix* A, uint32_t row_idx1, uint32_t row_idx2){
	uint32_t col_idx;
	uint64_t temp;
	for(col_idx=0; col_idx<A->words_in_row; ++col_idx){
		temp 	 								= A->elem[row_idx1 * A->words_in_row + col_idx];
		A->elem[row_idx1 * A->words_in_row + col_idx] = A->elem[row_idx2 * A->words_in_row + col_idx];
		A->elem[row_idx2 * A->words_in_row + col_idx] = temp;
	}
}

void partial_replace(matrix* dest, const uint32_t r1, const uint32_t c1,const uint32_t r2, const uint32_t c2, matrix* src, const int r3, const int c3){
	for(uint32_t i = 0; i < r2 - r1; i++)
		for(uint32_t j = 0; j < c2 - c1; j++)
			set_element(dest, r1 + i, c1+j, get_element(src, r3 + i, c3 + j));
}

void codeword(matrix* src, uint8_t* seed, matrix* dest){
	for (uint32_t i = 0; i < src->nrows; i++)
	{
		uint8_t window = 0x80 >> (i%8);
		uint8_t bit = seed[i/8] & window;

		if (bit == 1)
		{
			for (uint32_t j = 0; j < src->words_in_row; j++)
			{
				dest->elem[j] ^= src->elem[src->words_in_row * i + j];
			}
			
		}
		
	}
	
	
}