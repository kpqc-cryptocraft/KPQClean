#include "nearest_vector.h"

float *temp; 
void recursive_decoding_mod(float* y, const  int r1, const int m1, 
	const int f, const int l, uint16_t *perm1, uint16_t *perm2) {
	int i;
	if (r1 == 0) {
		//Calculate Euclidean distance
		float a1 = 0,a2 = 0;
	
		for ( i = f; i < l; i++) {
			a1 += pow(y[i] - 1,2); a2 += pow(y[i] + 1,2);
		}
		if (a1 <= a2) 
			for ( i = f; i < l; i++) 
				y[i] = 1;
		else
			for ( i = f; i < l; i++) 
				y[i] = -1;
		return;
	}
	
	if (r1 == m1) {
		for ( i = f; i < l; i++) 
			y[i] = (y[i]>=0)? 1: -1;
		
		return;
	}
	
	if(f == 0 && l == CODE_N/4) // partial depermutation
		y_depermute(y, f, l, perm1);
	if(f == 3*CODE_N/4 && l == CODE_N) // partial depermutation
		y_depermute(y, f, l, perm2);
	
	
	for ( i = 0; i < (l - f) / 2; i++) {
		temp[f + i] = y[i + (l + f) / 2];
	}

	for ( i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}

	recursive_decoding_mod(y, r1 - 1, m1 - 1, (l + f) / 2, l, perm1, perm2);

	for ( i = 0; i < (l - f) / 2; i++) {
		y[f + i] = (y[f + i] + y[i + (l + f) / 2] * temp[f + i]) / 2;
	}

	recursive_decoding_mod(y, r1, m1 - 1, f, (l + f) / 2, perm1, perm2);

	for ( i = 0; i < (l - f) / 2; i++) {
		y[i + (l + f) / 2] = y[i + (l + f) / 2] * y[i + f];
	}
	
	if(f == 0 && l == CODE_N/4) 
		y_permute(y, f, l, perm1);// partial depermutation
	if(f == 3*CODE_N/4 && l == CODE_N) // partial depermutation
		y_permute(y, f, l, perm2);
	
	return;
}

void init_decoding(int n){
	if(temp == 0) 
		temp = (float*)malloc(n * sizeof(float));

}

void y_permute(float *y, const int f, const int r, uint16_t *Q){
	int i; 
	float *ycpy = (float*)malloc(sizeof(float)*(r-f));
	for (i = 0; i < r - f; ++i)
		ycpy[i] = y[f + i];
	for (i = 0; i < r - f; ++i)
		y[f + i] = ycpy[Q[i]];
	free(ycpy);
}


void y_depermute(float *y, const int f, const int r, uint16_t *Q){
	int i; 
	float *ycpy = (float*) malloc(sizeof(float)*(r-f)); 
	for (i = 0; i < r - f; ++i)
		ycpy[i] = y[f + i];
	for (i = 0; i < r - f; ++i)
		y[f + Q[i]] = ycpy[i];
	free(ycpy);
}