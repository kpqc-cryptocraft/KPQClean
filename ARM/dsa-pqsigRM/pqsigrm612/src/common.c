// common.c for pqsigRM
#include "common.h"

unsigned char* hashMsg(unsigned char *s, const unsigned char *m, 
	unsigned long long mlen, unsigned long long sign_i){
	// Hash the given message
	// syndrome s = h(h(M)|i) | (h(h(M)|i)) | ...
	unsigned char* stemp = (unsigned char*)malloc(HASHSIZEBYTES*4);

	//SHA512(m, mlen, stemp);
	sha512(stemp, m, mlen);
	
	*(unsigned long long*)(stemp+HASHSIZEBYTES) = sign_i;// concatenate i i.e. h(M)|i
	
	//SHA512(stemp, HASHSIZEBYTES+sizeof(unsigned long long), stemp); //h(h(M)|i)
	sha512(stemp, stemp, HASHSIZEBYTES+sizeof(unsigned long long));

	//SHA512(stemp                , HASHSIZEBYTES, stemp+HASHSIZEBYTES);//(h(h(M)|i))
	sha512(stemp+HASHSIZEBYTES, stemp, HASHSIZEBYTES);
	
	//SHA512(stemp+HASHSIZEBYTES  , HASHSIZEBYTES, stemp+HASHSIZEBYTES*2);
	sha512(stemp+HASHSIZEBYTES*2, stemp+HASHSIZEBYTES, HASHSIZEBYTES);
	
	//SHA512(stemp+HASHSIZEBYTES*2, HASHSIZEBYTES, stemp+HASHSIZEBYTES*3);
	sha512(stemp+HASHSIZEBYTES*3, stemp+HASHSIZEBYTES*2, HASHSIZEBYTES);

	memcpy(s, stemp, 1+(CODE_N-CODE_K-1)/8);
	
	free(stemp);
	return s;
}

int hammingWgt(matrix* error){
	int wgt=0;
	int i=0;
	for(i=0; i < error->cols; i++)
		wgt += getElement(error, 0, i);
	return wgt;
}

void swap16(uint16_t *Q, const int i, const int j){
	uint16_t temp;
	temp = Q[i];
	Q[i] = Q[j];
	Q[j] = temp;
}

void permutation_gen(uint16_t *Q, int len){
	int i; 
	for(i=0; i<len; i++)
		Q[i] = i;
	for(i=0; i<len; i++)
		swap16(Q, i, random16(len));
}

int static compare(const void* first, const void* second){
	
	return (*(uint16_t*)first > *(uint16_t*)second)?1:-1;
}

void partial_permutation_gen(uint16_t* Q){
	permutation_gen(Q, CODE_N/4);
	uint16_t* partial_elem = (uint16_t*)malloc(sizeof(uint16_t)*PARM_P);
	uint16_t* partial_perm = (uint16_t*)malloc(sizeof(uint16_t)*PARM_P);
		

	memcpy(partial_perm, Q, sizeof(uint16_t)*PARM_P);
	memcpy(partial_elem, Q, sizeof(uint16_t)*PARM_P);

	qsort(partial_elem, PARM_P, sizeof(uint16_t), compare);
	qsort(Q, CODE_N/4, sizeof(uint16_t), compare);

	int i;
	for (i = 0; i < PARM_P; ++i)
	{
		Q[partial_elem[i]] = partial_perm[i];
	}

	free(partial_elem);free(partial_perm);
}




uint16_t random16(uint16_t n){
	uint16_t r;
	randombytes((unsigned char*)&r, 2);
	return r%n;
}

void col_permute(matrix* m, const int rf, const int rr, const int cf, 
	const int cr, uint16_t* Q)
{
	matrix* mcpy = newMatrix(m->rows, m->cols); 
	memcpy(mcpy->elem, m->elem, m->alloc_size);
	int r, c;
	for(c = cf; c < cr; c++)
		for(r = rf; r < rr; r++)
			setElement(m, r, c, getElement(mcpy, r, cf + Q[c-cf]));
	deleteMatrix(mcpy);
}



