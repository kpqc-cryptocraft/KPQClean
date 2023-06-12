#include "matrix.h"

matrix* newMatrix (int rows, int cols)
{
  matrix* A;

  A = (matrix*) malloc (sizeof (matrix));
  A->cols = cols;
  A->rows = rows;  
  A->rwdcnt = (1 + (cols - 1) / ELEMBLOCKSIZE);
  A->alloc_size = rows * A->rwdcnt * sizeof (unsigned char);
  A->elem = (unsigned char *)malloc(A->alloc_size);
  initZero(A);
  return A;
}

void mtxcpy(matrix* dest, const int r1, const int c1,const int r2, const int c2, matrix* src, const int r3, const int c3){
	for(int i = 0; i < r2 - r1; i++)
		for(int j = 0; j < c2 - c1; j++)
			setElement(dest, r1 + i, c1+j, getElement(src, r3 + i, c3 + j));
}
void deleteMatrix(matrix* A)
{
  free(A->elem);
  free(A);
}

int product(matrix * mtx1, matrix * mtx2, matrix * prod) {
	int row, col, k;
	int val;
	if(mtx1->cols != mtx2->rows) 	return -1;
	
	for (row = 0; row < mtx1->rows; row++)
		for (col = 0; col < mtx2->cols; col++) {
			val = 0;
			for (k = 0; k < mtx1->cols; k++)
				val ^= getElement(mtx1, row, k) & getElement(mtx2, k, col);
			setElement(prod, row, col, val);
		}
	return 0;
}

//assume vector is transposed
void vector_mtx_product(matrix *dest, matrix* m, matrix *vec){
	unsigned char bit = 0;
	unsigned char offset;
	int row, col;
	for(row = 0; row < m->rows; row++){
		bit = 0;
		for(col=0; col < m->rwdcnt - 1; col++)
			bit ^= m->elem[row*m->rwdcnt + col] & vec->elem[col];
	
		offset = 0xff << (ELEMBLOCKSIZE*m->rwdcnt - m->cols);
		bit ^= (m->elem[row*m->rwdcnt + col] & vec->elem[col])&offset;

		bit ^= (bit >> 4);
		bit ^= (bit >> 2);
		bit ^= (bit >> 1);
		bit &= (unsigned char)1;
		
		setElement(dest, 0, row, bit);
	}
}

void rowInterchanging(matrix* A, int row_idx1, int row_idx2){
	int col_idx;
	unsigned char temp;
	for(col_idx=0; col_idx<A->rwdcnt; ++col_idx){
		temp 	 								= A->elem[row_idx1 * A->rwdcnt + col_idx];
		A->elem[row_idx1 * A->rwdcnt + col_idx] = A->elem[row_idx2 * A->rwdcnt + col_idx];
		A->elem[row_idx2 * A->rwdcnt + col_idx] = temp;
	}
}

void rowAddition(matrix* A, int dest_row_idx, int adding_row_idx){
	int col_idx;
	for(col_idx=0; col_idx<A->rwdcnt; ++col_idx){
		A->elem[dest_row_idx * A->rwdcnt + col_idx] 
			^= A->elem[adding_row_idx * A->rwdcnt + col_idx];
	}
}

matrix * rref(matrix* A)
{
	// Considering column is longer than row
	int succ_row_idx=0;
	int col_idx, row_idx=0;
	int i;
	for (col_idx = 0; col_idx < (A->cols); ++col_idx) {
		
		// finding first row s.t. i th elem of the row is 1
		for(; row_idx < A->rows; ++row_idx)
			if(getElement(A, row_idx, col_idx) == 1) 
				break;
		// When reaches the last row,
		// increase column index and search again
		if (row_idx == A->rows){ 
			row_idx=succ_row_idx;
			continue;
		}
		// if row_idx is not succ_row_idx, 
		// interchange between:
		// <succ_row_idx> th row <-> <row_idx> th row
		if(row_idx != succ_row_idx){
			rowInterchanging(A, succ_row_idx, row_idx);
		}
				
		// By adding <succ_row_idx> th row in the other rows 
		// s.t. A(i, <succ_row_idx>) == 1,
		// making previous columns as element row.
		for(i=0; i<A->rows; ++i){
			if(i == succ_row_idx) continue;

			if(getElement(A, i, col_idx) == 1){
				rowAddition(A, i, succ_row_idx);
			}
		}
		row_idx = ++succ_row_idx;
	}
	//Gaussian elimination is finished. So return A.
	return A;
}

int inverse(matrix *mtx, matrix *mtxInv){
	if(mtx->rows != mtx->cols) 			return INV_FAIL;
	if(mtxInv->rows != mtxInv->cols) 	return INV_FAIL;
	if(mtx->rows != mtxInv->rows) 		return INV_FAIL;

	matrix* temp = newMatrix(mtx->rows, mtx->cols);
	matrixcpy(temp, mtx);

	int r, c;
	for(r = 0; r< mtxInv->alloc_size;++r){
		mtxInv->elem[r] = 0;
	}
	for ( r = 0; r <  mtxInv->rows; ++r)
	{
		setElement(mtxInv, r, r, 1);
	}

	for (c = 0; c < temp->cols; ++c)
	{
		if(getElement(temp, c, c) == 0)
		{	
			for (r = c+1; r < mtx->rows; ++r)
			{
				if(getElement(temp, r, c) != 0){
					rowInterchanging(temp, r, c);
					rowInterchanging(mtxInv, r, c);
					break;
				}
			}
			if(r >= temp->rows) 		return INV_FAIL;
		}
		

		for(r = 0; r < temp->rows; r++){
			if(r == c) continue;
			if(getElement(temp, r, c) != 0){
				rowAddition(temp, r, c);
				rowAddition(mtxInv, r, c);
			}
		}
	}
	deleteMatrix(temp);
	return INV_SUCCESS;
}

int isNonsingular(matrix *mtx){

	matrix* temp = newMatrix(mtx->rows, mtx->cols);
	matrixcpy(temp, mtx);

	int r, c;
	unsigned char bit_one =	0x80;

	for (c = 0; c < temp->cols; ++c)
	{
		if(getElement(temp, c, c) == 0)
		{	
			for (r = c+1; r < mtx->rows; ++r)
			{
				if(getElement(temp, r, c) != 0){
					rowInterchanging(temp, r, c);
					break;
				}
			}
			if(r >= temp->rows) 		return INV_FAIL;
		}
		

		for(r = 0; r < temp->rows; r++){
			if(r == c) continue;
			if(getElement(temp, r, c) != 0){
				rowAddition(temp, r, c);
			}
		}
	}
	deleteMatrix(temp);
	return INV_SUCCESS;
}


matrix* matrixcpy(matrix* dest, matrix* src){
	if(dest->rows != src->rows || dest->cols!=src->cols) return MATRIX_NULL;
	
	memcpy(dest->elem, src->elem, dest->alloc_size);
	return dest;
}

matrix* transpose(matrix *dest, matrix *src){
	if((dest->rows != src->cols) || (dest->cols != src->rows))
		return MATRIX_NULL;
	int row, col;
	for(row=0; row < dest->rows; ++row)
		for(col=0; col < dest->cols; ++col)
			setElement(dest, row, col, getElement(src, col, row));
	return dest;
}

// Exports a matrix into unsigned char destination.
int exportMatrix(unsigned char* dest, matrix* src_mtx){
	memcpy(dest, src_mtx->elem, src_mtx->alloc_size);
	return src_mtx->alloc_size;
}

matrix* importMatrix(matrix* dest_mtx, const unsigned char* src){
	memcpy(dest_mtx->elem, src, dest_mtx->alloc_size);

	return dest_mtx;
}

int add(matrix *m1, matrix *m2, matrix *res){
	if((m1->rows != m2->rows) || (m1->cols != m2->cols))
		return -1;
	
	for(int i =0; i< res->alloc_size; i++)
		res->elem[i] = (m1->elem[i])^(m2->elem[i]);

	return 0;
}

void getPivot(matrix* mtx, uint16_t *lead, uint16_t *lead_diff){
	int row=0, col=0;
	int lead_idx=0, diff_idx=0;
	while((col < mtx->cols) && (row < mtx->rows) && (lead_idx < mtx->rows) && (diff_idx < (mtx->cols - mtx->rows))){
		if(getElement(mtx, row, col) == 1){
			lead[lead_idx++] = col;
			row++;
		}
		else{
			lead_diff[diff_idx++] =col;
		}
		col++;
	}

	while(col < mtx->cols){
		lead_diff[diff_idx++]=col++;
	}
}

void dual(matrix* G, matrix* H_sys, uint16_t *lead, uint16_t *lead_diff){
	int row, col, flg = 0; 
	rref(G);
	if(lead == 0 || lead_diff == 0){
		lead = (uint16_t*)malloc(sizeof(uint16_t)*G->rows);
		lead_diff = (uint16_t*)malloc(sizeof(uint16_t)*(G->cols - G->rows));	
		flg = 1;
	}
	getPivot(G, lead, lead_diff);
	// Fill not-identity part (P')
	for ( row = 0; row < H_sys->rows; row++) 
		for ( col = 0; col < G->rows; col++) 
			setElement(H_sys, row, lead[col], getElement(G, col, lead_diff[row]));

	for ( row = 0; row < H_sys->rows; row++) 
			setElement(H_sys, row, lead_diff[row], 1);
	
	if(flg){
		free(lead);
		free(lead_diff);
	}
}
