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
 
#ifndef GFPOLY_H
#define GFPOLY_H

#include "config.h"
#include "gf.h"

/*
   A_t X^t + ... + A_0X^0 > A[t], A[t-1], ... A[0]
*/
void GF_print(IN gf* in, IN int t);
void GF_add(OUT gf* out, IN gf* in1, IN gf* in2, IN int t);  //t차 덧셈. 
int Deg_poly(gf *in, IN int t);
void POLY_mul_o(OUT gf* out, IN gf* in1, IN gf* in2, IN PARAM* PALOMAParam);
void GF_mul_o(OUT gf* out, IN gf *gx, IN gf* in1, IN gf* in2, IN PARAM* PALOMAParam);
void GF_square_o(OUT gf* out, IN gf* gx, IN gf* in, IN PARAM* PALOMAParam);
void GF_sqrt_o(OUT gf* out, IN gf* gx, IN gf* in, IN PARAM* PALOMAParam);
void GF_inv_o(OUT gf* out, IN gf* in, IN gf *gx, IN PARAM* PALOMAParam);
void EEA_gx(OUT gf *out, IN gf *in, IN gf *gx, IN PARAM* PALOMAParam);
void GF_remind(OUT gf *quo, OUT gf *rem, IN gf *a, IN gf *b, IN PARAM* PALOMAParam);

gf GF_eval(IN gf* polyf , IN gf gf_a, IN int deg_f, IN PARAM* PALOMAParam); //honor_method
void GF_GCD(OUT gf *gcd, IN gf *poly1, IN gf *poly2, IN PARAM* PALOMAParam);
void GF_verify(IN gf* gx, IN PARAM* PALOMAParam);


#endif