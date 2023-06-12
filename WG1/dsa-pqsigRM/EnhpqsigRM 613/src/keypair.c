#include "api.h"
#include "common.h"

void export_sk(unsigned char *sk,uint16_t *Q, uint16_t *part_perm1, uint16_t* part_perm2, matrix* Hrep){
	//export private in order: Q, part_perm1, pert_perm2
	memcpy		(sk, 
					Q, sizeof(uint16_t)*CODE_N);
	memcpy		(sk+sizeof(uint16_t)*CODE_N, 
					part_perm1, sizeof(uint16_t)*CODE_N/4);
	memcpy		(sk+sizeof(uint16_t)*CODE_N+sizeof(uint16_t)*CODE_N/4, 
					part_perm2, sizeof(uint16_t)*CODE_N/4);
	export_matrix(sk + sizeof(uint16_t)*CODE_N + (sizeof(uint16_t)*CODE_N/4)*2,	Hrep);
}

void export_pk(unsigned char *pk, matrix *Hpub){
	export_matrix(pk, Hpub);
}

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk){
	// nrows of Gm is set to K + 1 for public key
	matrix* Gm = new_matrix(CODE_K, CODE_N);
	matrix* Hm = new_matrix(CODE_N - CODE_K, CODE_N);

	matrix* Gpub = new_matrix(CODE_K + 1, CODE_N);
	matrix* Hpub = new_matrix(CODE_N - CODE_K - 1, CODE_N);

	uint16_t Q[CODE_N];
	
	uint16_t part_perm1[(CODE_N/4)];
	uint16_t part_perm2[(CODE_N/4)];

	uint16_t s_lead[CODE_N - CODE_K];
	uint16_t s_diff[CODE_K];

	// generate secret parital permutations
	partial_permutation_gen(part_perm1);
	partial_permutation_gen(part_perm2);
	
	// Generate a partially permute generator matrix Gm
	rm_gen(Gm, RM_R, RM_M, 0, CODE_K, 0, CODE_N);

	// replace of RM(r,r)
	matrix* Grep = new_matrix(1<<RM_R - K_REP, 1<<RM_R);
	matrix* Hrep = new_matrix(K_REP, 1<<RM_R);

	uint8_t is_odd = 0;
	while(1){
		randombytes((unsigned char*)(Grep->elem), Grep->alloc_size);
		printf("asdf %d \n", Grep->alloc_size);
		dual(Grep, Hrep, 0, 0);
		for (uint32_t i = 0; i < K_REP; i++)
		{	
			ELEMBLOCK parity = 0;
			for (uint32_t j = 0; j < (1<<RM_R); j++)
			{
				parity ^= get_element(Hrep, i, j);
			}
			if (parity == 1){
				is_odd = 1;
				break;
			}
		}
		if(is_odd) break;
	}

	rref(Hrep);

	// replace the code (starting from second row)
	for (uint32_t i = 0; i < CODE_N; i += Grep->ncols)
	{
		partial_replace(Gpub, K_REP, i, K_REP + Grep->nrows, i + Grep->ncols, Grep, 0, 0); 
	}
	
	// Partial permutation
	for (uint32_t i = 0; i < 4; ++i)
	{
		col_permute(Gm, 0, rm_dim[RM_R][RM_M -2], 
			i*(CODE_N/4),(i+1)*(CODE_N/4), part_perm1);
	}
	
	col_permute(Gm, CODE_K - rm_dim[RM_R-2][RM_M-2], CODE_K, 
		3*CODE_N/4, CODE_N, part_perm2);

	// Parity check matrix of the modified RM code
	dual(Gm, Hm, 0, 0);

	// pick a random codeword from the dual code
	matrix* code_from_dual = new_matrix(1, CODE_N);
	uint8_t seed[1 + (Hm->nrows -  1)/8];
	randombytes(seed, 1 + (Hm->nrows -  1)/8);
	codeword(Hm, seed, code_from_dual);

	memcpy(Gpub->elem, Gm->elem, Gm->alloc_size);
	partial_replace(Gpub, CODE_K, 0, CODE_K + 1, CODE_N, code_from_dual, 0, 0);

	// matrix* random_rows = new_matrix(K_REP, CODE_N);
	// while(1){
	// 	randombytes((unsigned char*)(random_rows->elem), random_rows->alloc_size);
		
	// 	if(hamming_weight(random_rows)%2 == 1){
	// 		break;
	// 	}
	// }
	// partial_replace(Gpub, 0, 0, K_REP, CODE_N, random_rows, 0, 0);

	// Public code generation: permutation and export
	// Generate the permutation for whole matrix
	permutation_gen(Q, CODE_N);

	// Generate the dual code of Gm and the public key
	dual(Gpub, Hpub, 0, 0);
	matrix* Hcpy = new_matrix(Hpub->nrows, Hpub->ncols); 
	memcpy(Hcpy->elem, Hpub->elem, Hpub->alloc_size);

	col_permute(Hcpy, 0, Hcpy->nrows, 0, Hcpy->ncols, Q);
	rref(Hcpy);

	uint16_t pivot[Hcpy->nrows];
	uint16_t d_pivot[Hcpy->ncols - Hcpy->nrows];
	get_pivot(Hcpy, pivot, d_pivot);

	for (uint32_t i = 0; i < Hcpy->nrows; i++)
	{
		if(pivot[i] != i){
			uint16_t tmp = Q[i];
			Q[i] = Q[pivot[i]];
			Q[pivot[i]] = tmp;
		}		
	}
	
	col_permute(Hpub, 0, Hpub->nrows, 0, Hpub->ncols, Q);
	rref(Hpub);

	// DEBUG
	for (uint32_t i = 0; i < Hpub->nrows; i++)
	{
		if(get_element(Hpub, i, i) != 1){
			printf("not identity!, %d, %d, %d\n", i, i, get_element(Hm, i, i));
		}		
	}

	export_sk(sk, Q, part_perm1, part_perm2, Hrep);
	// printf("sk: %p, pk: %p\n", sk, pk);
	
	export_pk(pk, Hpub);

	delete_matrix(Gm);
	delete_matrix(Hm);
	delete_matrix(Gpub);
	delete_matrix(Hpub);
	delete_matrix(Hcpy);

	delete_matrix(Grep);
	delete_matrix(Hrep);

	return 0;
}
