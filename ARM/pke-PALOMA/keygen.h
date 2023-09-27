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
 
#ifndef KEYGEN_H
#define KEYGEN_H

#include "config.h"
#include "gf.h"

void GenKeyPair(OUT u64* pk, OUT u64* sk, IN PARAM* PALOMAParam);
void GenRandGoppaCode(OUT gf* SupportSet, OUT gf* GoppaPoly, OUT u64* H, IN PARAM* PALOMAParam);
void GetScrambledCode(OUT u64* H_hat, OUT u64* S_inv, OUT u64* rand4P, IN u64* H, IN PARAM* PALOMAParam);
void makeParityCheckMat(OUT u64* H, IN gf* GPPoly, IN gf* SupSet, IN PARAM* PALOMAParam);
void GenRandPermMat(OUT gf* permmatP, IN int row, IN u64* r);

#endif