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
 
#ifndef MAT_MUL_H
#define MAT_MUL_H

#include "config.h"
#include "gf.h"

int Gaussian(OUT u64* out, OUT u64* inv_mat, IN u64* in_mat, IN int rownum, IN int colnum);
void Gen_Identity_Mat(OUT u64* out, IN int row);
void matXvec(OUT u64* outvec, IN u64* inmat, IN u64* invec, IN int rownum, IN int colnum);
void matXmat(OUT u64* outmat, IN u64* inmat1, IN u64* inmat2, IN int mat1rownum, IN int mat1colnum, IN int mat2colnum);
int Gaussian_row(OUT u64* out, IN u64* in_mat, IN int rownum, IN int colnum);


#endif