#pragma once
#include "Encryption.h"
#include "rng.h"
#include <time.h>



/*************************************************
* Name:         Encryption
* 
* Description:  Encryption proceeds through the computation 
*				of elements belonging to the Is set of graphs
*			    
*
* Arguments:    type(*polyDst)[maxDIM + 2]					output ciphertext
*               const type message							plaintext
*				type num_cs									number of cs
*				type dim									number of dim
*               const type GraphNeighbor[maxVERTEX][4]		list containing neighbor vertex information of graph vertices
*				const type neig1_table[maxVERTEX][maxBOX]	table containing neighbor vertex information of graph vertices
*				type numVertex								the start vertex number of the graph
*				type start_ver								start vertex number of graph
**************************************************/
type Encryption(type(*polyDst)[maxDIM + 2], const type message, type num_cs, type dim,
	const type GraphNeighbor[maxVERTEX][4], const type neig1_table[maxVERTEX][maxBOX], type numVertex, type start_ver)
{
	//*  SET Is
	type Is[maxNumCs][maxDIM + 2] = { 0, };
	getISset(Is, message, num_cs, dim, start_ver, numVertex);

#if UI
	printf("[Is set]\n");
	for (int i=0; i<G1fDim; i++) {
		for (int j=0; j<7; j++) {
			printf("%3d ", Is[i][j]);
		}
		printf("\n");
	}
	printf("========================\n");
	printf("========neig IS========\n");
#endif
	//  Get Neigbor of Is
	type neig_Is[maxNumCs][maxDIM][4] = { 0, };
	getIsNeighbor(neig_Is, Is, GraphNeighbor, numVertex, num_cs, start_ver);
#if UI
	for (int i=0; i<G1numCs; i++) {
		for (int j=0; j<maxDIM; j++) {
			for (int k=0; k<4; k++) {
				printf("%3d ", neig_Is[i][j][k]);
			}printf("\n");
		}
		printf("\n");
	}printf("========================\n");
#endif

	//  SET poly
	type polyListTmp[maxTERMS][maxDIM + 2] = { 0, };
	type numOfTerms = Is2polyterms(polyListTmp, Is, neig_Is, num_cs, dim);

	//  MUL + Reduction
	type polyList[maxTERMS][maxDIM + 2] = { 0, };
	type numOfTERMS = MULwithReduction(polyList, polyListTmp, numOfTerms, numVertex, neig1_table, start_ver);

	//  Check Repeted Terms
	type numTERMS = CheckRepetedTerms(polyDst, polyList, numOfTERMS, numVertex, start_ver);
#if UI
	printf("==========Terms=========\n");
	for (int i=0; i<100; i++) {
		for (int j=0; j<7; j++) {
			printf("%3d ", polyDst[i][j]);
		}
		printf("\n");
	}
#endif
	return numTERMS;
}

/*************************************************
* Name:         getISset
* 
* Description:  Randomly select the Is set of the graph with the given parameters
*
* Arguments:    type(*Is)[maxDIM + 2]						output Is set
*               type message								plaintext
*				type num_cs									number of cs
*				type dim									number of dim
*				type start_ver								start vertex number of graph
*				type numVertex								the start vertex number of the graph
**************************************************/
void getISset(type(*Is)[maxDIM + 2], type message, type num_cs, type dim, type start_ver, type numVertex)
{
	type cs_sum = 0;
	for (type i = 0; i < num_cs - 1; i++) {

		type dim_i = (randomint() % dim) + 1;

		Is[i][0] = dim_i;
		Is[i][1] = (randomint() % (MOD_N - 1)) + 1;

		cs_sum = (cs_sum + Is[i][1]) % MOD_N;
	}
	if ((cs_sum % MOD_N) == message)
		for (type i = 0; i < num_cs - 1; i++)
			if (Is[i][1] != MOD_N - 1) {
				Is[i][1] += 1;
				cs_sum += 1;
				break;
			}
	Is[num_cs - 1][0] = dim;
	Is[num_cs - 1][1] = (message - cs_sum + MOD_N) % MOD_N;

	for (type i = 0; i < num_cs; i++) {
		type cs_vertex[maxDIM] = { 0, };
		for (type j = 0; j < dim; j++) {
			cs_vertex[j] = (randomint() % numVertex) + start_ver;

			for (type k = 0; k < j; k++)
				if (cs_vertex[j] == cs_vertex[k]) {
					j--;
					break;
				}
		}
		for (type j = 0; j < Is[i][0]; j++)
			Is[i][j + 2] = cs_vertex[j];
	} 
}

/*************************************************
* Name:         getIsNeighbor
* 
* Description:  Find the neighboring vertices of the elements 
*				in this set in the public key
*
* Arguments:    type(*neighbor)[maxDIM][4]			pointer will containing neighborhood vertex information
*               type Is[maxNumCs][maxDIM + 2]		information of Is set
*				type GraphNeighbor[maxVERTEX][4]	information of neighborhood of graph
*				type numVertex						number of vertex of graph
*				type num_cs							number of cs
*				type start_ver						number of start vertex of graph
**************************************************/
void getIsNeighbor(type(*neighbor)[maxDIM][4], type Is[maxNumCs][maxDIM + 2],
	 type GraphNeighbor[maxVERTEX][4], type numVertex, type num_cs, type start_ver)
{
	for (type cs = 0; cs < num_cs; cs++)
		for (type dim = 0; dim < Is[cs][0]; dim++) {
			type vertex = (Is[cs][dim + 2] - start_ver) % numVertex;

			neighbor[cs][dim][0] = GraphNeighbor[vertex][0];
			neighbor[cs][dim][1] = GraphNeighbor[vertex][1];
			neighbor[cs][dim][2] = GraphNeighbor[vertex][2];
			neighbor[cs][dim][3] = GraphNeighbor[vertex][3];
		}
}



/*************************************************
* Name:         Is2polyterms
* 
* Description:  Convert isset to polynomial terms
*
* Arguments:    type(*polyList)[maxDIM + 2]				pointer will containing polynomial terms
*               type Is[maxNumCs][maxDIM + 2]			information of Is set
*				const type neig_Is[maxNumCs][maxDIM][4] information of neighborhood of graph
*				type num_cs								number of cs
*				type maxDim								number of dim
**************************************************/
type Is2polyterms(type(*polyList)[maxDIM + 2], type Is[maxNumCs][maxDIM + 2],
	const type neig_Is[maxNumCs][maxDIM][4], type num_cs, type maxDim)
{
	type list = 0;	// number of polyTerms

	for (type cs = 0; cs < num_cs; cs++) {
		type dim = Is[cs][0];

		if (dim == 0) {
            printf("  dim = 0, Error!!!\n");
			break;
        }
		if (((dim - 1) % maxDim) != (dim - 1)) {
			printf("  dim Error!!!\n");
			break;
		}

		type D_tmp[maxDIM] = { 0, };

		while (D_tmp[0] < 4) {
			polyList[list][0] = dim;
			polyList[list][1] = Is[cs][1] % MOD_N;

			for (int i = 0; i < dim; i++)
				polyList[list][2 + i] = neig_Is[cs][i][D_tmp[i]];

			list++;
			D_tmp[dim - 1] += 1;
			for (type i = dim - 1; i > 0; i--)
				if (D_tmp[i] == 4) {
					D_tmp[i] = 0;
					D_tmp[i - 1] += 1;
				}
		}
	}

    return list;
}


/*************************************************
* Name:         MULwithReduction
* 
* Description:  IPCC operation - multiplication and reduction
*
* Arguments:    type(*polydst)[maxDIM + 2]				  pointer will containing output list
*               const type polyList[maxTERMS][maxDIM + 2] pointer will containing polynomial terms
*				type numOfTerms							  number of 
*				type numVertex							  number of vertex of graph
*				const type neig_table[maxVERTEX][maxBOX]  information of neighborhood of graph
*				type start_ver							  start vertex number of graph
**************************************************/
type MULwithReduction(type(*polyDst)[maxDIM + 2], const type polyList[maxTERMS][maxDIM + 2],
	type numOfTerms, type numVertex, const type neig_table[maxVERTEX][maxBOX], type start_ver)
{
	type indexTerm = 0;

	for (type i = 0; i < numOfTerms; i++) {
		if ((polyList[i][0] < 1) && (polyList[i][1] < 1))
			break;

		if ((polyList[i][0] == 1) && (polyList[i][1] > 0)) {
			polyDst[indexTerm][0] = polyList[i][0];
			polyDst[indexTerm][1] = polyList[i][1];
			polyDst[indexTerm][2] = polyList[i][2];

			indexTerm++;
			continue;
		}

		if ((polyList[i][0] > 1) && (polyList[i][1] > 0)) {
			for (type j = 0; j < polyList[i][0] - 1; j++) {
				type x1 = (polyList[i][2 + j] - start_ver) % numVertex + 1;
				for (type k = j + 1; k < polyList[i][0]; k++) {
					type x2 = (polyList[i][2 + k] - start_ver) % numVertex + 1;

					if (x1 == x2)
						continue;
					
					for (type box = 0; box < maxBOX; box++)
						if ((neig_table[x1 - 1][box] & neig_table[x2 - 1][box]) > 0)	// reduction
							goto NEXT_I;
				}
			}

			type Digit[maxBOX] = { 0, };
			for (type j = 0; j < polyList[i][0]; j++) {
				type box = ((polyList[i][j + 2] - start_ver) % numVertex) / DATASIZE;
				type val = 1 << (((polyList[i][j + 2] - start_ver) % numVertex) % DATASIZE);
				Digit[box] |= val;
			}
			type cnt = 0;
			for (type j = 0; j < maxBOX; j++) {
				for (type k = 0; k < DATASIZE; k++) {
					if (((Digit[j] >> k) & 1) == 1) {
						polyDst[indexTerm][cnt + 2] = (DATASIZE * j) + k + start_ver;
						cnt++;
					}
				}
			}

			polyDst[indexTerm][0] = cnt;
			polyDst[indexTerm][1] = polyList[i][1] % MOD_N;

			indexTerm++;
		}
	NEXT_I: ;
	}
	return indexTerm;
}


/*************************************************
* Name:         CheckRepetedTerms
* 
* Description:  IPCC operation - CheckRepetedTerms : 
*				Delete if neighbor relationship or neighbor relationship terms are multiplied, 
*				and decrease order first if same vertex is multiplied
*
* Arguments:    type(*polyDst)[maxDIM + 2]				  pointer will containing output list
*               const type polyList[maxTERMS][maxDIM + 2] pointer will containing polynomial terms
*				type numOfTerms							  number of 
*				type numVertex							  number of vertex of graph
*				type start_ver							  start vertex number of graph
**************************************************/
type CheckRepetedTerms(type(*polyDst)[maxDIM + 2], const type polyList[maxTERMS][maxDIM + 2],
	type numOfTerms, type numVertex, type start_ver)
{
	type tmp[maxTERMS][maxBOX] = { 0, };

	for (type terms = 0; terms < numOfTerms; terms++) {
		if ((polyList[terms][0] == 0) || (polyList[terms][1] == 0))	break;
		for (type vertex = 0; vertex < polyList[terms][0]; vertex++) {
			if (polyList[terms][vertex + 2] == 0)	continue;
			else {
				type box = ((polyList[terms][vertex + 2] - start_ver) % numVertex) / DATASIZE;
				type val = 1 << (((polyList[terms][vertex + 2] - start_ver) % numVertex) % DATASIZE);
				tmp[terms][box] |= val;
			}
		}
	}

	type dst[maxTERMS][maxBOX] = { 0, };

	for (type i = 0; i < maxDIM + 2; i++)
		polyDst[0][i] = polyList[0][i];
	for (type i = 0; i < maxBOX; i++)
		dst[0][i] = tmp[0][i];
	

	type D_list = 1;
	type check = 1;

	while (check < numOfTerms) {
		for (type i = 0; i < D_list; i++) {
			if (polyDst[i][0] == polyList[check][0]) {
				for (type j = 0; j < maxBOX; j++)
					if (dst[i][j] != tmp[check][j])
						goto NEXT;
				
				polyDst[i][1] = (polyDst[i][1] + polyList[check][1]) % MOD_N;
				check++;
				i = -1;
			}
		NEXT: ;
		}
		for (type j = 0; j < maxDIM + 2; j++)	polyDst[D_list][j] = polyList[check][j];
		for (type i = 0; i < maxBOX; i++)		dst[D_list][i] = tmp[check][i];
		
		D_list++;
		check++;
	}

	return D_list;
}




/*************************************************
* Name:         Neighbor
* 
* Description:  Find the neighboring vertices of the elements 
*				in the public key
*
* Arguments:    type(*GraphNeighbor)[4]			pointer will containing neighborhood vertex information
*				type GraphEdge[6 * maxPDS][2]	pointer of relation of graph of edge : public key
*				type numVertex						number of vertex of graph
*				type start_ver						number of start vertex of graph
**************************************************/
void Neighbor(type(*GraphNeighbor)[4], type GraphEdge[6 * maxPDS][2], type numVertex, type start_ver)
{
	type index[maxVERTEX] = { 0, };

	for (type i = 0; i < numVertex; i++) {
		GraphNeighbor[i][0] = i + 1 + (start_ver - 1);
		index[i]++;
	}
	
	type nEdge = (numVertex / 4) * 6;

	for (type i = 0; i < nEdge; i++) {
		type a = (GraphEdge[i][0] - start_ver) % numVertex;
		type b = (GraphEdge[i][1] - start_ver) % numVertex;

		GraphNeighbor[a][index[a]] = b + start_ver;
		index[a]++;
		GraphNeighbor[b][index[b]] = a + start_ver;
		index[b]++;
	}
}



/*************************************************
* Name:         GetNeighborDigit
* 
* Description:  Find the neighboring vertices of digit forms of the elements 
*				in the public key
*
* Arguments:    type(*v_sum)[maxBOX]			    pointer will containing neighborhood vertex information digit forms
*				type GraphNeighbor[maxVERTEX][4]    pointer of containing neighborhood vertex information
*				type numVertex						number of vertex of graph
*				type start_ver						number of start vertex of graph
**************************************************/
void GetNeighborDigit(type(*v_sum)[maxBOX], type GraphNeighbor[maxVERTEX][4], type numVertex, type start_ver)
{
	for (type i = 0; i < numVertex; i++) {
		for (type j = 0; j < 4; j++) {
			if (GraphNeighbor[i][j] == 0)	continue;

			type box = ((GraphNeighbor[i][j] - start_ver) % numVertex) / DATASIZE;
			type val = 1 << (((GraphNeighbor[i][j] - start_ver) % numVertex) % DATASIZE);
			v_sum[i][box] ^= val;
		}
	}
}


/*************************************************
* Name:         resetNeighbor
* 
* Description:  Initializing of the neighboring vertex information
*
* Arguments:    type(*GraphNeighbor)[4]			pointer of containing neighborhood vertex information
**************************************************/
void resetNeighbor(type (*GraphNeighbor)[4])
{
	for (int i = 0; i < maxVERTEX; i++)
		for (int j = 0; j < 4; j++)
			GraphNeighbor[i][j] = 0;
}


/*************************************************
* Name:         resetNeighborTable
* 
* Description:  Initializing of the neighboring vertex table information
*
* Arguments:    type(*Table)[4]			pointer of containing neighborhood vertex information
*				type numVertex			number of vertex of graph
**************************************************/
void resetNeighborTable(type(*Table)[4], type numVertex)
{
	for (int i = 0; i < numVertex; i++)
		for (int j = 0; j < maxBOX; j++)
			Table[i][j] = 0;
}
