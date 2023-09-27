#pragma once
#include "Keygen.h"
#include "rng.h"
#include <time.h>
#include <stdlib.h>



/*************************************************
* Name:        Keygen
*
* Description: Generates public and private key
*              for IPCC encryption, decryption
*
* Arguments:   type(*GraphEdge)[2]  pointer to output public key of graph
*              type(*PDS)           pointer to output private key of graph 1 and graph 2
*			   type numVertex		the start vertex number of the graph
*			   type start_ver		start vertex number of graph
**************************************************/
void Keygen(type(*GraphEdge)[2], type(*PDS), type numVertex, type start_ver)
{
	if (((numVertex) % 4) != 0) { // 3-regular graph
		printf("ERROR : It can't be 3-Regular Graph\n");
		return;
	}
	type nPDS = numVertex / 4;

	//* Generate for Secret Key PDS
	type tmp_index[maxVERTEX] = { 0, };
	Shuffle(tmp_index, numVertex);

	type A[maxPDS] = { 0, };
	type B[maxPDS] = { 0, };
	type C[maxPDS] = { 0, };
	type D[maxPDS] = { 0, };

	for (type i = 0; i < nPDS; i++) {
		A[i] = tmp_index[4 * i] + start_ver;
		B[i] = tmp_index[4 * i + 1] + start_ver;
		C[i] = tmp_index[4 * i + 2] + start_ver;
		D[i] = tmp_index[4 * i + 3] + start_ver;
	}
	for (type i = 0; i < nPDS; i++)
		PDS[i] = A[i];


	//* Generate for Public Key Graph : Neighbor Relationships
	type tmp_arr[maxPDS] = { 0, };
	
	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge,			i, A[i], B[tmp_arr[i]]);	// 1) A-B
	
	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge,	 nPDS + i, A[i], C[tmp_arr[i]]);	// 2) A-C

	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge, 2 * nPDS + i, A[i], D[tmp_arr[i]]);	// 3) A-D

	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge, 3 * nPDS + i, B[i], C[tmp_arr[i]]);	// 4) B-C

	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge, 4 * nPDS + i, B[i], D[tmp_arr[i]]);	// 5) B-D

	Shuffle(tmp_arr, nPDS);
	for (type i = 0; i < nPDS; i++)	Add_GraphEdge(GraphEdge, 5 * nPDS + i, C[i], D[tmp_arr[i]]);	// 6) C-D
}


/*************************************************
* Name:        Shuffle
*
* Description: Generates random array
*
* Arguments:   type(*GraphEdge)[2]  pointer to output array
*              type arr_len			length of array
**************************************************/
void Shuffle(type(*arr), type arr_len)
{
	for (type i = 0; i < arr_len; i++)
		arr[i] = i;

	for (type i = 0; i < arr_len - 1; i++) {
		type index = (randomint()%MAXnumVertex) % (arr_len - i) + i;
		type tmp = arr[i];
		arr[i] = arr[index];
		arr[index] = tmp;
	}
}


/*************************************************
* Name:        Add_GraphEdge
*
* Description: Generates random array
*
* Arguments:   type(*GraphEdge)[2]  pointer to output array
*			   int i				number for create neighboring relation of graph
*			   type ver1			number of vertex
*			   type ver2			number of vertex
**************************************************/
void Add_GraphEdge(type(*GraphEdge)[2], int i, type ver1, type ver2)
{
	if (ver1 < ver2) {
		GraphEdge[i][0] = ver1;
		GraphEdge[i][1] = ver2;
	}
	else {
		GraphEdge[i][0] = ver2;
		GraphEdge[i][1] = ver1;
	}
}


/*************************************************
* Name:        setPDS
*
* Description: Generates public and private key
*              for IPCC encryption, decryption
*
* Arguments:   type(*PDS)  		     pointer to output private key of graph 1 and graph 2
*			   type G1PDS[G1numPDS]	 pointer to output private key of graph 1
*			   type G2PDS[G2numPDS]	 pointer to output private key of graph 2
**************************************************/
void setPDS(type(*PDS), type G1PDS[G1numPDS], type G2PDS[G2numPDS])
{
	for (int i = 0; i < G1numPDS; i++)
		PDS[i] = G1PDS[i];
	for (int i = 0; i < G2numPDS; i++)
		PDS[i + G1numPDS] = G2PDS[i];
}