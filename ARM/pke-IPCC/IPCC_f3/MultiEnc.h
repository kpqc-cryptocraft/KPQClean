#ifndef MYLTIENC_H
#define MYLTIENC_H

#pragma once
#include "Encryption.h"



/*   Alice   */

type EncType1(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2);

type EncType2(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3);

type EncType3(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3);

type EncType4(type(*cipher)[CIPHER_DIM + 2],
	type GraphNeighbor1[maxVERTEX][4], type v_nieg_1[G1numVertex][G1BOX], type message1,
	type GraphNeighbor2[maxVERTEX][4], type v_nieg_2[G2numVertex][G2BOX], type message2, type message3, type message4);



type MulTwoPoly(type(*polyDst)[CIPHER_DIM + 2],
	const type poly1[G1fTerms][maxDIM + 2], type numTERMS1,
	const type poly2[G2fTerms][maxDIM + 2], type numTERMS2);


type Redudction4sum(type(*polyDst)[maxDIM + 2],
	type polySrc1[G1fTerms][maxDIM + 2], type numTERMS1,
	type polySrc2[G2fTerms][maxDIM + 2], type numTERMS2);

#endif