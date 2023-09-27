#ifndef PARAMS_H
#define PARAMS_H

#define UI 0

/*=================    user SETTING    =================*/


//* Multi-Graph Combination Method
#ifndef IPCC_f
#define IPCC_f 3 		/* Change this for different security strengths */
#endif



//* MESSAGE
#define MOD_N		65521	// public parameter	, prime Modulus < 4,294,967,295


//* Graph 1
#define G1numVertex	200	// public parameter , 200 < #.vertex < 300

#define G1numCs		3	// secret parameter
#define G1fDim		3	// public parameter , 1 < #.highest_dimension < 6
#define G1fTerms	500

//* Graph 2
#define G2numVertex	200	// public parameter
#define G2numCs		3	// secret parameter
#define G2fDim		2	// public parameter
#define G2fTerms	500

//* Static Array
#define maxTERMS	500
#define MUL_TERMS	10000	// #TERMS[f(Graph1)*f(Graph2)]










/*=================    auto SETTING   =================*/

#define DATASIZE	32

// #define MAXnumVertex (G1numVertex>G2numVertex) ? G1numVertex:G2numVertex
#define MAXnumVertex G1numVertex

//* Graph 1
#define Graph1Start	1
#define G1numPDS	G1numVertex / 4
#define G1BOX		(G1numVertex / DATASIZE) + 1

//* Graph 2
#define Graph2Start	1 + G1numVertex
#define G2numPDS	G2numVertex / 4
#define G2BOX		(G2numVertex / DATASIZE) + 1


//* Static Array
#define maxVERTEX	300
#define maxNumCs	10
#define maxDIM		5
#define maxPDS		maxVERTEX / 4
#define maxBOX		(maxVERTEX / DATASIZE) + 1


//* Chipher
#define CIPHER_VERTEX	G1numVertex + G2numVertex
#define CIPHER_BOX		(CIPHER_VERTEX / DATASIZE) + 1
#define CIPHER_DIM		G1fDim + G2fDim
#define CIPHER_PDS		G1numPDS + G2numPDS
#define CIPHER_TERMS	MUL_TERMS + (3 * maxTERMS)

/* F1) {f(G1, m1) + 1} * {f(G2, m2) + 1} - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m1) + f(G2, m2)
 * F2) {f(G1, m1) + 1} * {f(G2, m2) + 1} + f(G1, m3) - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m1) + f(G1, m3) + f(G2, m2)
 * F3) {f(G1, m1) + 1} * f(G2, m2) + f(G1, m3) + 1 - 1
 *     = f(G1, m1)f(G2, m2) + f(G1, m3) + f(G2, m2)
 * F4) f(G1, m1)f(G2, m2) + f(G1, m3) + f(G2, m4)
*/

//* FINAL_MESSAGE
#if (IPCC_f == 1)
	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * (MESSAGE2 + 1) - 1\n")
#elif (IPCC_f == 2)
	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * (MESSAGE2 + 1) + MESSAGE3 - 1\n")
#elif (IPCC_f == 3)
	#define PRINT_selectF()	printf("   (MESSAGE1 + 1) * MESSAGE2 + MESSAGE3\n")
#elif (IPCC_f == 4)
	#define PRINT_selectF()	printf("   MESSAGE1 * MESSAGE2 + MESSAGE3 + MESSAGE\n4")
#elif ((IPCC_f > 4) || (IPCC_f < 1))
	#define PRINT_selectF()	printf("   selection ERROR : This option is NOT supported\n\n");
#endif


// for DigitValue (bit digit with laxical order)
#define Box(X) (X - 1) / DATASIZE
#define Val(X) 1 << ((X - 1) % DATASIZE)


#define TRUE	1
#define FALSE	0









#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <stdint.h>
// #include <crtdbg.h>
#include <math.h>

//*  Define
typedef unsigned int	type;
typedef unsigned char	u8;
typedef unsigned int	u32;

#define IPCC_PUBLICKEYBYTES CIPHER_VERTEX // pk
#define IPCC_PUBLICKEYBYTES_ROW 6*maxPDS // pk_row
#define IPCC_PUBLICKEYBYTES_COL 2 // pk_col
#define IPCC_SECRETKEYBYTES CIPHER_PDS // sk
#define IPCC_CIPHERTEXTBYTES CIPHER_TERMS * CIPHER_BOX // ct
#define IPCC_CIPHERTEXTBYTES_ROW CIPHER_TERMS // ct_row
#define IPCC_CIPHERTEXTBYTES_COL CIPHER_DIM + 2 // ct_col

#endif