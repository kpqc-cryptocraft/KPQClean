
//
//  PQCgenKAT_pke.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "Keygen.h"
#include "Encryption.h"
#include "MultiEnc.h"
#include "Decryption.h"
#include "rng.h"
#include "api.h"
#include "fips202.h"

#define	MAX_MARKER_LEN		50
#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void    fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);
void    fprintBstr1(FILE *fp, char *S, unsigned int *A, unsigned long long L);
void    fprintBstr2(FILE *fp, char *S, unsigned int (*A)[2], unsigned long long L);
void    fprintBstr3(FILE *fp, char *S, unsigned int (*A)[CRYPTO_CIPHERTEXTBYTES_COL], unsigned long long L);
void    hashing_fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);
void    u32tou8(unsigned char* out, unsigned int (*A)[CRYPTO_CIPHERTEXTBYTES_COL], unsigned long long L);
#define PUTU32(out, in) { out[0]=(unsigned char)((in)>>24);out[1]=(unsigned char)((in)>>16);out[2]=(unsigned char)((in)>>8);out[3]=(unsigned char)(in); }

int 
main() 
{
    char            fn_req[32], fn_rsp[32];
    FILE            *fp_req, *fp_rsp;
    unsigned char   seed[48];
    unsigned int    pk1[CRYPTO_PUBLICKEYBYTES_ROW][CRYPTO_PUBLICKEYBYTES_COL];
    unsigned int    pk2[CRYPTO_PUBLICKEYBYTES_ROW][CRYPTO_PUBLICKEYBYTES_COL];
    unsigned int    ct[CRYPTO_CIPHERTEXTBYTES_ROW][CRYPTO_CIPHERTEXTBYTES_COL];
    unsigned char   tmp_ct[CRYPTO_CIPHERTEXTBYTES];
    unsigned char   hashing_ct[64];
    unsigned int    sk[CRYPTO_SECRETKEYBYTES];
    unsigned int    numTerms;
    int             done;
    int             count;
    int             ret_val;
    unsigned int    clen[1];
    unsigned int    m[1];
    unsigned char   entropy_input[48];


    // Create the REQUEST file
    sprintf(fn_req, "PQCKAT_%d.req", CRYPTO_SECRETKEYBYTES*3);
    if ( (fp_req = fopen(fn_req, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    sprintf(fn_rsp, "PQCKAT_%d.rsp", CRYPTO_SECRETKEYBYTES*3);
    if ( (fp_rsp = fopen(fn_rsp, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

    for (int i=0; i<48; i++)
        entropy_input[i] = i;

    randombytes_init(entropy_input, NULL, 256);
    for (int i=0; i<100; i++) {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 48);
        fprintBstr(fp_req, "seed = ", seed, 48);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
    }
    fclose(fp_req);


    // //Create the RESPONSE file based on what's in the REQUEST file
    if ( (fp_req = fopen(fn_req, "r")) == NULL ) {
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }
    fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);
    done = 0;
    
    
    do {
        
        while (1) {
            m[0] = randomint()%MOD_N;
            if (m[0]>=5) break;
        }
        
        if ( FindMarker(fp_req, "count = ") )
            fscanf(fp_req, "%d", &count);
        else {
            done = 1;
            break;
        }
        fprintf(fp_rsp, "count = %d\n", count);
        
        if ( !ReadHex(fp_req, seed, 48, "seed = ") ) {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
            return KAT_DATA_ERROR;
        }

        fprintBstr(fp_rsp, "seed = ", seed, 48);
        randombytes_init(seed, NULL, 256);


        // Generate the public/private keypair
        if ( (ret_val = crypto_encrypt_keypair(pk1, pk2, sk)) != 0) {
            printf("crypto_encrypt_keypair returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }

        fprintBstr2(fp_rsp, "pk1 = ", pk1, CRYPTO_PUBLICKEYBYTES);
        fprintBstr2(fp_rsp, "pk2 = ", pk2, CRYPTO_PUBLICKEYBYTES);
        fprintBstr1(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);


        for (int i=0; i<CRYPTO_CIPHERTEXTBYTES_ROW; i++)
            for (int j=0; j<CRYPTO_CIPHERTEXTBYTES_COL; j++)
                ct[i][j]=0;
        
        if ( (ret_val = crypto_encrypt(ct, clen, m, sizeof(m), pk1, pk2)) != 0) { 
            printf("crypto_encrypt returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }

        u32tou8(tmp_ct, ct, CRYPTO_CIPHERTEXTBYTES);
        sha3_256(hashing_ct,tmp_ct, sizeof(tmp_ct));
        hashing_fprintBstr(fp_rsp, "ct = ", hashing_ct, 32);
        

        fprintf(fp_rsp, "\n");
        if ( (ret_val = crypto_encrypt_open(m, sizeof(m), ct, clen, sk)) != 0) {
            printf("crypto_kem_dec returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }

    } while ( !done );

    fclose(fp_req);
    fclose(fp_rsp);

    return KAT_SUCCESS;

    return 0;


}



//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
	char	line[MAX_MARKER_LEN];
	int		i, len;
	int curr_line;

	len = (int)strlen(marker);
	if ( len > MAX_MARKER_LEN-1 )
		len = MAX_MARKER_LEN-1;

	for ( i=0; i<len; i++ )
	  {
	    curr_line = fgetc(infile);
	    line[i] = curr_line;
	    if (curr_line == EOF )
	      return 0;
	  }
	line[len] = '\0';

	while ( 1 ) {
		if ( !strncmp(line, marker, len) )
			return 1;

		for ( i=0; i<len-1; i++ )
			line[i] = line[i+1];
		curr_line = fgetc(infile);
		line[len-1] = curr_line;
		if (curr_line == EOF )
		    return 0;
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
	int			i, ch, started;
	unsigned char	ich;

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) )
		while ( (ch = fgetc(infile)) != EOF ) {
			if ( !isxdigit(ch) ) {
				if ( !started ) {
					if ( ch == '\n' )
						break;
					else
						continue;
				}
				else
					break;
			}
			started = 1;
			if ( (ch >= '0') && (ch <= '9') )
				ich = ch - '0';
			else if ( (ch >= 'A') && (ch <= 'F') )
				ich = ch - 'A' + 10;
			else if ( (ch >= 'a') && (ch <= 'f') )
				ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;
			
			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
		}
	else
		return 0;

	return 1;
}

void
fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
        fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}


void
fprintBstr1(FILE *fp, char *S, unsigned int *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
        fprintf(fp, "%04X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}

void
fprintBstr2(FILE *fp, char *S, unsigned int (*A)[2], unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<(L>>2); i++ )
        fprintf(fp, "%04X", A[i][0]);
        fprintf(fp, "%04X", A[i][1]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}


void
fprintBstr3(FILE *fp, char *S, unsigned int (*A)[CRYPTO_CIPHERTEXTBYTES_COL], unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ ) {
        if (A[i][0] == 0) break;
        for (int j=0; j<A[i][0]; j++) {
            fprintf(fp, "%03X", A[i][j]);
        }
    }

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}


void    
u32tou8(unsigned char *out, unsigned int (*A)[CRYPTO_CIPHERTEXTBYTES_COL], unsigned long long L) {
    unsigned long long typeindex=0;

    for (int i=0; i<L; i++) {
        if (A[i][0]==0) 
            break;
        for (int j=0; j<A[i][0]; j++) {
            PUTU32((out+typeindex), A[i][j]);
            // PUTU32(out, A[i][j]);
            typeindex+=4;
        }
    }
}

void
hashing_fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
	unsigned long long  i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}

