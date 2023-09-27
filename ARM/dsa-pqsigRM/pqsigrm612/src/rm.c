#include "rm.h"

const uint16_t rm_dim[7][13]  
={{1,1,1,1,1,1,1,1,1,1,1,1,1}, 
  {0,2,3,4,5,6,7,8,9,10,11,12,13}, 
  {0,0,4,7,11,16,22,29,37,46,56,67,79}, 
  {0,0,0,8,15,26,42,64,93,130,176,232,299}, 
  {0,0,0,0,16,31,57,99,163,256,386,562,794}, 
  {0,0,0,0,0,32,63,120,219,382,638,1024,1586}, 
  {0,0,0,0,0,0,64,127,247,466,848,1486,2510}};

matrix* rm_gen(int r, int m, uint16_t row_f, uint16_t row_r, uint16_t col_f, 
	uint16_t col_r, matrix* gen) {

	if (r == 0) {
		for (int i = 0; i < (1<<m); i++) {
			setElement(gen, row_f, col_f + i, 1);
		}
	} else if (m == r) {
		for (int i = 0; i < (1<<m); i++) {
			 setElement(gen, row_f + i, col_f + i, 1);
		}
	} else {
		uint16_t col_m = (col_f+col_r)/2;

		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], 
				col_f, 	col_m ,	gen);
		rm_gen(r,   m-1, 	row_f, 					row_f + rm_dim[r][m-1], 
				col_m, 	col_r, 	gen);
		rm_gen(r-1, m-1,  	row_f + rm_dim[r][m-1], row_r,
				col_m, 	col_r, 	gen);
	}
	return gen;
}

matrix* rm_gen_mod(matrix* gen, uint16_t *part_perm1, uint16_t *part_perm2) {

	rm_gen(RM_R, RM_M, 0, CODE_K, 0, CODE_N, gen);

	int i =0;
	for (i = 0; i < 4; ++i)
	{
		col_permute(gen, 0, rm_dim[RM_R][RM_M -2], 
			i*(CODE_N/4),(i+1)*(CODE_N/4), part_perm1);
	}
	
	col_permute(gen, CODE_K - rm_dim[RM_R-2][RM_M-2], CODE_K, 
		3*CODE_N/4, CODE_N, part_perm2);
	
	return gen;
}
