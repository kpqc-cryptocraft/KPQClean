#ifndef KEYGEN_H
#define KEYGEN_H

#pragma once
#include "params.h"



/*   Bob   */

void Keygen(type(*GraphEdge)[2], type(*PDS), type numVertex, type start_ver);

void Shuffle(type(*arr), type arr_len);
void Add_GraphEdge(type(*GraphEdge)[2], int i, type ver1, type ver2);

void setPDS(type(*PDS), type G1PDS[G1numPDS], type G2PDS[G2numPDS]);

#endif