#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#pragma once
#include "params.h"



/*   Alice   */

type Encryption(type(*polyDst)[maxDIM + 2], const type message, type num_cs, type dim,
	const type GraphNeighbor[maxVERTEX][4], const type neig1_table[maxVERTEX][maxBOX], type numVertex, type start_ver);


void getISset(type(*Is)[maxDIM + 2], type message, type num_cs, type dim, type start_ver, type numVertex);
void getIsNeighbor(type(*neighbor)[maxDIM][4], type Is[maxNumCs][maxDIM + 2],
	 type GraphNeighbor[maxVERTEX][4], type numVertex, type num_cs, type start_ver);
type Is2polyterms(type(*polyList)[maxDIM + 2], type Is[maxNumCs][maxDIM + 2], const type neig_Is[maxNumCs][maxDIM][4], type num_cs, type maxDim);

type MULwithReduction(type(*polyDst)[maxDIM + 2], const type polyList[maxTERMS][maxDIM + 2],
	type numOfTerms, type numVertex, const type neig_table[maxVERTEX][maxBOX], type start_ver);
type CheckRepetedTerms(type(*polyDst)[maxDIM + 2], const type polyList[maxTERMS][maxDIM + 2],
	type numOfTerms, type numVertex, type start_ver);


void Neighbor(type(*GraphNeighbor)[4], type GraphEdge[6 * maxPDS][2], type numVertex, type start_ver);
void GetNeighborDigit(type(*v_sum)[maxBOX], type GraphNeighbor[maxVERTEX][4], type numVertex, type start_ver);


void resetNeighbor(type(*GraphNeighbor)[4]);
void resetNeighborTable(type(*Table)[4], type numVertex);

#endif