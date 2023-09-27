#pragma once
#include "MultiEnc.h"
#include "Decryption.h"
#include <time.h>



/*************************************************
* Name:         EncType1
* 
* Description:  IPCC f1 function Encryption
*			    
*
* Arguments:    type(*cipher)[CIPHER_DIM + 2]				pointer of output ciphertext
*               type GraphNeighbor1[maxVERTEX][4]			pointer of public key 1
*				type v_nieg_1[G1numVertex][G1BOX]			pointer of public key 1 information table
*				type message1								pointer of plaintext 1
*               type GraphNeighbor2[maxVERTEX][4]			pointer of public key 2
*				type v_nieg_2[G2numVertex][G2BOX]			pointer of public key 2 information table
*				type message2								pointer of plaintext 2
**************************************************/
type EncType1(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2)
{
	//* First Graph Encryption
	// printf("========poly1========\n");
	type poly1[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS1 = Encryption(poly1, message1,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);

	//* Second Graph Encryption
	// printf("========poly2========\n");
	type poly2[G2fTerms][maxDIM + 2] = { 0, };
	type numTERMS2 = Encryption(poly2, message2,
		G2numCs, G2fDim, GraphNeighbor2, v_nieg_2, G2numVertex, Graph2Start);


	type MULpoly[MUL_TERMS][CIPHER_DIM + 2] = { 0, };
	type MULterms = MulTwoPoly(MULpoly, poly1, numTERMS1, poly2, numTERMS2);


	type dstTERMS = 0;

	for (type tmpIdx = 0; tmpIdx < MULterms; tmpIdx++) {
		for (type i = 0; i < MULpoly[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = MULpoly[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS1; tmpIdx++) {
		for (type i = 0; i < poly1[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly1[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS2; tmpIdx++) {
		for (type i = 0; i < poly2[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly2[tmpIdx][i];
		dstTERMS++;
	}


	return dstTERMS;
}


/*************************************************
* Name:         EncType2
* 
* Description:  IPCC f2 function Encryption
*			    
*
* Arguments:    type(*cipher)[CIPHER_DIM + 2]				pointer of output ciphertext
*               type GraphNeighbor1[maxVERTEX][4]			pointer of public key 1
*				type v_nieg_1[G1numVertex][G1BOX]			pointer of public key 1 information table
*				type message1								pointer of plaintext 1
*               type GraphNeighbor2[maxVERTEX][4]			pointer of public key 2
*				type v_nieg_2[G2numVertex][G2BOX]			pointer of public key 2 information table
*				type message2								pointer of plaintext 2
*				type message3								pointer of plaintext 3
**************************************************/
type EncType2(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3)
{
	//* First Graph Encryption
	type poly1[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS1 = Encryption(poly1, message1,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);

	//* Second Graph Encryption
	type poly2[G2fTerms][maxDIM + 2] = { 0, };
	type numTERMS2 = Encryption(poly2, message2,
		G2numCs, G2fDim, GraphNeighbor2, v_nieg_2, G2numVertex, Graph2Start);

	//* Third Graph Encryption
	type poly3[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS3 = Encryption(poly3, message3,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);


	type ADDpoly[2 * maxTERMS][maxDIM + 2] = { 0, };
	type ADDterms = Redudction4sum(ADDpoly, poly1, numTERMS1, poly3, numTERMS3);

	type MULpoly[CIPHER_TERMS][CIPHER_DIM + 2] = { 0, };
	type MULterms = MulTwoPoly(MULpoly, poly1, numTERMS1, poly2, numTERMS2);


	type dstTERMS = 0;
	for (type tmpIdx = 0; tmpIdx < MULterms; tmpIdx++) {
		for (type i = 0; i < MULpoly[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = MULpoly[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < ADDterms; tmpIdx++) {
		for (type i = 0; i < ADDpoly[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = ADDpoly[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS2; tmpIdx++) {
		for (type i = 0; i < poly2[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly2[tmpIdx][i];
		dstTERMS++;
	}

	return dstTERMS;
}


/*************************************************
* Name:         EncType3
* 
* Description:  IPCC f3 function Encryption
*			    
*
* Arguments:    type(*cipher)[CIPHER_DIM + 2]				pointer of output ciphertext
*               type GraphNeighbor1[maxVERTEX][4]			pointer of public key 1
*				type v_nieg_1[G1numVertex][G1BOX]			pointer of public key 1 information table
*				type message1								pointer of plaintext 1
*               type GraphNeighbor2[maxVERTEX][4]			pointer of public key 2
*				type v_nieg_2[G2numVertex][G2BOX]			pointer of public key 2 information table
*				type message2								pointer of plaintext 2
*				type message3								pointer of plaintext 3
**************************************************/
type EncType3(type(*cipher)[CIPHER_DIM + 2],
type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3)
{
	//* First Graph Encryption
	type poly1[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS1 = Encryption(poly1, message1,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);

	//* Second Graph Encryption
	type poly2[G2fTerms][maxDIM + 2] = { 0, };
	type numTERMS2 = Encryption(poly2, message2,
		G2numCs, G2fDim, GraphNeighbor2, v_nieg_2, G2numVertex, Graph2Start);

	//* Third Graph Encryption
	type poly3[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS3 = Encryption(poly3, message3,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);


	type MULpoly[CIPHER_TERMS][CIPHER_DIM + 2] = { 0, };
	type MULterms = MulTwoPoly(MULpoly, poly1, numTERMS1, poly2, numTERMS2);


	type dstTERMS = 0;

	for (type tmpIdx = 0; tmpIdx < MULterms; tmpIdx++) {
		for (type i = 0; i < MULpoly[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = MULpoly[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS2; tmpIdx++) {
		for (type i = 0; i < poly2[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly2[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS3; tmpIdx++) {
		for (type i = 0; i < poly3[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly3[tmpIdx][i];
		dstTERMS++;
	}

	return dstTERMS;
}


/*************************************************
* Name:         EncType4
* 
* Description:  IPCC f3 function Encryption
*			    
*
* Arguments:    type(*cipher)[CIPHER_DIM + 2]				pointer of output ciphertext
*               type GraphNeighbor1[maxVERTEX][4]			pointer of public key 1
*				type v_nieg_1[G1numVertex][G1BOX]			pointer of public key 1 information table
*				type message1								pointer of plaintext 1
*               type GraphNeighbor2[maxVERTEX][4]			pointer of public key 2
*				type v_nieg_2[G2numVertex][G2BOX]			pointer of public key 2 information table
*				type message2								pointer of plaintext 2
*				type message3								pointer of plaintext 3
*				type message4								pointer of plaintext 3
**************************************************/
type EncType4(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3, type message4)
{
	//* First Graph Encryption
	type poly1[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS1 = Encryption(poly1, message1,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);

	//* Second Graph Encryption
	type poly2[G2fTerms][maxDIM + 2] = { 0, };
	type numTERMS2 = Encryption(poly2, message2,
		G2numCs, G2fDim, GraphNeighbor2, v_nieg_2, G2numVertex, Graph2Start);

	//* Third Graph Encryption
	type poly3[G1fTerms][maxDIM + 2] = { 0, };
	type numTERMS3 = Encryption(poly3, message3,
		G1numCs, G1fDim, GraphNeighbor1, v_nieg_1, G1numVertex, Graph1Start);

	//* Fourth Graph Encryption
	type poly4[G2fTerms][maxDIM + 2] = { 0, };
	type numTERMS4 = Encryption(poly4, message4,
		G2numCs, G2fDim, GraphNeighbor2, v_nieg_2, G2numVertex, Graph2Start);


	type MULpoly[CIPHER_TERMS][CIPHER_DIM + 2] = { 0, };
	type MULterms = MulTwoPoly(MULpoly, poly1, numTERMS1, poly2, numTERMS2);


	type dstTERMS = 0;

	for (type tmpIdx = 0; tmpIdx < MULterms; tmpIdx++) {
		for (type i = 0; i < MULpoly[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = MULpoly[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS3; tmpIdx++) {
		for (type i = 0; i < poly3[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly3[tmpIdx][i];
		dstTERMS++;
	}
	for (type tmpIdx = 0; tmpIdx < numTERMS4; tmpIdx++) {
		for (type i = 0; i < poly4[tmpIdx][0] + 2; i++)
			cipher[dstTERMS][i] = poly4[tmpIdx][i];
		dstTERMS++;
	}

	return dstTERMS;
}




/*************************************************
* Name:         MulTwoPoly
* 
* Description:  MUL 2 Polynomial generated from distinct graph G1 G2
*
* Arguments:    type(*polyDst)[CIPHER_DIM + 2]				pointer of output polynomial list
*				const type poly1[G1fTerms][maxDIM + 2]		pointer of polynomial 1
*				type numTERMS1								pointer of number of polynomial 1
*				const type poly2[G1fTerms][maxDIM + 2]		pointer of polynomial 2
*				type numTERMS2								pointer of number of polynomial 2
**************************************************/

type MulTwoPoly(type(*polyDst)[CIPHER_DIM + 2],
	const type poly1[G1fTerms][maxDIM + 2], type numTERMS1,
	const type poly2[G2fTerms][maxDIM + 2], type numTERMS2)
{
	type dstTERMS = 0;
	for (type term1 = 0; term1 < numTERMS1; term1++) {
		type dim1 = poly1[term1][0];
		type cs1 = poly1[term1][1];

		if ((dim1 > 0) && (cs1 > 0)) {
			for (type term2 = 0; term2 < numTERMS2; term2++) {
				type dim2 = poly2[term2][0];
				type cs2 = poly2[term2][1];

				if ((dim2 > 0) && (cs2 > 0)) {
					type terms = 0;
					polyDst[dstTERMS][0] = dim1 + dim2;
					polyDst[dstTERMS][1] = (cs1 * cs2) % MOD_N;
					for (type vertex = 0; vertex < dim1; vertex++)
						polyDst[dstTERMS][2 + vertex] = poly1[term1][2 + vertex];
					for (type vertex = 0; vertex < dim2; vertex++)
						polyDst[dstTERMS][2 + dim1 + vertex] = poly2[term2][2 + vertex];

					dstTERMS++;
				}
			}
		}
	}
	return dstTERMS;
}




/*************************************************
* Name:         Redudction4sum
* 
* Description:  Check polynomials generated from same graph have repeted terms
*
* Arguments:    type(*polyDst)[CIPHER_DIM + 2]				pointer of output polynomial list
*				const type polySrc1[G1fTerms][maxDIM + 2]	pointer of polynomial 1
*				type numTERMS1								pointer of number of polynomial 1
*				const type polySrc2[G1fTerms][maxDIM + 2]	pointer of polynomial 2
*				type numTERMS2								pointer of number of polynomial 2
**************************************************/
type Redudction4sum(type(*polyDst)[maxDIM + 2],
	type polySrc1[G1fTerms][maxDIM + 2], type numTERMS1,
	type polySrc2[G2fTerms][maxDIM + 2], type numTERMS2)
{
	type polySrc[2 * maxTERMS][maxDIM + 2] = { 0, };
	type numTERMS = 0;

	for (type terms1 = 0; terms1 < numTERMS1; terms1++) {
		for (type i = 0; i < polySrc1[terms1][0] + 2; i++)
			polySrc[numTERMS][i] = polySrc1[terms1][i];
		numTERMS++;
	}
	for (type terms2 = 0; terms2 < numTERMS2; terms2++) {
		for (type i = 0; i < polySrc2[terms2][0] + 2; i++)
			polySrc[numTERMS][i] = polySrc2[terms2][i];
		numTERMS++;
	}

	type src[2 * maxTERMS][CIPHER_BOX] = { 0, };
	for (type srcIdx = 0; srcIdx < numTERMS; srcIdx++)
		for (type vertex = 0; vertex < polySrc[srcIdx][0]; vertex++){
			type box = Box(polySrc[srcIdx][vertex + 2]);
			type val = Val(polySrc[srcIdx][vertex + 2]);
			if (polySrc[srcIdx][vertex + 2] != 0)
				src[srcIdx][box] |= val;
		}

	type dst[2 * maxTERMS][CIPHER_BOX] = { 0, };
	type dstIdx = 0;

	type srcIdx = 0;
	while (0)	if (polySrc[srcIdx++][0] == 0)	break;

	for (type copy = 0; copy < maxDIM + 2; copy++)
		polyDst[dstIdx][copy] = polySrc[srcIdx][copy];
	for (type copy = 0; copy < maxBOX; copy++)
		dst[dstIdx][copy] = src[srcIdx][copy];
	srcIdx++;
	dstIdx++;

	while (srcIdx < numTERMS) {
		if (polySrc[srcIdx][0] == 0)	goto NEXTsrc;

		for (type checkDst = 0; checkDst < dstIdx; checkDst++) {
			if (polyDst[checkDst][0] == polySrc[srcIdx][0]) {
				for (type box = 0; box < maxBOX; box++)
					if (dst[checkDst][box] != src[srcIdx][box])	goto NEXTcheck;

				polyDst[checkDst][1] = (polyDst[checkDst][1] + polySrc[srcIdx][1]) % MOD_N;
				goto NEXTsrc;
			}
		NEXTcheck:;
		}
		for (type copy = 0; copy < maxDIM + 2; copy++)
			polyDst[dstIdx][copy] = polySrc[srcIdx][copy];
		for (type copy = 0; copy < maxBOX; copy++)
			dst[dstIdx][copy] = src[srcIdx][copy];

		dstIdx++;
	NEXTsrc:;
		srcIdx++;
	}

	return dstIdx;
}


