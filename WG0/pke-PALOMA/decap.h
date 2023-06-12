/*
 * Copyright (c) 2022 FDL(Future cryptography Design Lab.) Kookmin University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef DECAP_H
#define DECAP_H

#include "config.h"
#include "gf.h"

void Decap(OUT u64* key, IN u64* sk, IN u64* r, IN u64* s_hat, IN PARAM* PALOMAParam);
void Decrypt(OUT u64* e_hat, IN u64* sk, IN u64* synd_hat, IN PARAM* PALOMAParam);
void RecErrVec(OUT u64* e, IN u64* sk, IN u64* synd, IN PARAM* PALOMAParam);
void Topoly(OUT gf* syndpoly, IN u64* synd, IN int t);
void ConstructKeyEqn(OUT gf* shat, OUT gf* g1, OUT gf* g2, OUT gf* g12, IN gf* syndpoly, IN gf* gx, IN PARAM* PALOMAParam);
void SolveKeyEqn(OUT gf* a1, OUT gf* b2 , IN gf* shat, IN gf* g12, IN int dega, IN int degb, IN PARAM* PALOMAParam);
void FindErrVec(OUT u64* errorvec, IN gf* sigma, IN u64* supportSet, IN PARAM* PALOMAParam);


#endif