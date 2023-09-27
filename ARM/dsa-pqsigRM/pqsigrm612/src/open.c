#include "api.h"
#include "common.h"

void import_signed_msg(matrix *errorMtx, unsigned long long *sign_i, const unsigned char *sm){
	importMatrix(errorMtx, sm);
	*sign_i = *((unsigned long long*)(sm+errorMtx->alloc_size));
}


void import_pk(const unsigned char *pk, matrix *H_pub){
	importMatrix(H_pub, pk);
}

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk){
	matrix *errorMtx = newMatrix(1, CODE_N);

	matrix *H_pub = newMatrix(CODE_N-CODE_K, CODE_N);

	matrix *syndrome_by_hash = newMatrix(1, CODE_N - CODE_K);
	matrix *syndrome_by_e	 = newMatrix(1, CODE_N - CODE_K);

	unsigned long long sign_i;
	unsigned long long mlen_rx;
	unsigned char* m_rx;
	
	int i;

	memcpy(&mlen_rx, sm, sizeof(unsigned long long));
	m_rx = (unsigned char*)malloc(mlen_rx);

	memcpy(m_rx, sm + sizeof(unsigned long long), mlen_rx);

	import_signed_msg(errorMtx, &sign_i, sm + sizeof(unsigned long long) + mlen_rx);
	
	if(hammingWgt(errorMtx) > WEIGHT_PUB) 
		return VERIF_REJECT;
	

	hashMsg(syndrome_by_hash->elem, m_rx, mlen_rx, sign_i);
	
	//import public key
	import_pk(pk, H_pub);
	
	vector_mtx_product(syndrome_by_e, H_pub, errorMtx);

	for(i=0; i<CODE_N-CODE_K; ++i)
		if(getElement(syndrome_by_hash, 0, i) != getElement(syndrome_by_e, 0, i))
			return VERIF_REJECT;

	memcpy(m, m_rx, mlen_rx);
	*mlen = mlen_rx;

	deleteMatrix(errorMtx);
	deleteMatrix(H_pub);

	deleteMatrix(syndrome_by_hash);
	deleteMatrix(syndrome_by_e);
	free(m_rx);

	return 0;
}
