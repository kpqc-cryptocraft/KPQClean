#include "api.h"
#include "common.h"

void import_signed_msg(matrix *errorMtx, uint64_t *sign_i, const unsigned char *sm){
	import_matrix(errorMtx, sm);
	*sign_i = *((uint64_t*)(sm+errorMtx->alloc_size));
}


void import_pk(const unsigned char *pk, matrix *H_pub){
	import_matrix(H_pub, pk);
}

void print_matrix_open(matrix* mtx){
    uint32_t row = mtx->nrows;
    uint32_t col = mtx->ncols;
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            printf("%d",get_element(mtx, i,j));
        }printf("\n");
    }
    
}

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk){
	matrix *errorMtx = new_matrix(1, CODE_N);

	matrix *H_pub = new_matrix(CODE_N - CODE_K - 1, CODE_N);

	matrix *syndrome_by_hash = new_matrix(1, CODE_N - CODE_K - 1);
	matrix *syndrome_by_e	 = new_matrix(1, CODE_N - CODE_K - 1);

	uint64_t sign_i;
	uint64_t mlen_rx;
	unsigned char* m_rx;
	
	memcpy(&mlen_rx, sm, sizeof(uint64_t));
	m_rx = (unsigned char*)malloc(mlen_rx);

	memcpy(m_rx, sm + sizeof(uint64_t), mlen_rx);

	import_signed_msg(errorMtx, &sign_i, sm + sizeof(uint64_t) + mlen_rx);
	
	if(hamming_weight(errorMtx) > WEIGHT_PUB) {
		fprintf(stderr, "larger weight\n");
		return VERIF_REJECT;
	}
		
	hash_message((unsigned char*)syndrome_by_hash->elem, m_rx, mlen_rx, sign_i);
	
	//import public key
	import_pk(pk, H_pub);

	vec_mat_prod(syndrome_by_e, H_pub, errorMtx);

	for(uint32_t i=0; i < CODE_N-CODE_K - 1; ++i){
		if(get_element(syndrome_by_hash, 0, i) != get_element(syndrome_by_e, 0, i)){
			fprintf(stderr, "different hash\n");
			printf("hashed value: \n");
			print_matrix_open(syndrome_by_hash);
			printf("H*e value: \n");
			print_matrix_open(syndrome_by_e);
			return VERIF_REJECT;
		}
	}
	memcpy(m, m_rx, mlen_rx);
	*mlen = mlen_rx;

	delete_matrix(errorMtx);
	delete_matrix(H_pub);

	delete_matrix(syndrome_by_hash);
	delete_matrix(syndrome_by_e);
	free(m_rx);

	return 0;
}
