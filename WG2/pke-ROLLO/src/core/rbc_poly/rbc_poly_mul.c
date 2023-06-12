


/**
 * \fn void rbc_plain_mul(rbc_poly o, const rbc_poly p1, const rbc_poly p2)
 * \brief This function multiplies two polynomials.
 *
 * This function is based on NTL multiplication of GF2EX elements
 *
 * \param[out] o Polynomial
 * \param[in] a Polynomial
 * \param[in] b Polynomial
 */
void rbc_plain_mul(rbc_poly o, const rbc_poly a, const rbc_poly b) {
  int32_t d = a->max_degree + b->max_degree;
  rbc_elt acc, tmp;

  for(int32_t i = 0 ; i <= d ; ++i) {
    int32_t jmin, jmax;
    jmin = max(0, i - b->max_degree);
    jmax = min(a->max_degree, i);

    rbc_elt_set_zero(acc);

    for(int32_t j = jmin ; j <= jmax ; ++j) {
      rbc_elt_mul(tmp, a->v[j], b->v[i - j]);
      rbc_elt_add(acc, acc, tmp);
    }

    rbc_elt_set(o->v[i], acc);
  }
}



/**
 * Karatsuba multiplication of a and b using notations from "A course in computational algebraic number theory" (H. Cohen), 3.1.2. Implementation inspired from the NTL library.
 *
 * \param[out] o Polynomial
 * \param[in] a Polynomial
 * \param[in] b Polynomial
 */
void rbc_kar_fold(rbc_vec res, rbc_vec src, int32_t max, int32_t half_size) {
  for(int32_t i = 0 ; i < max / 2 ; ++i) {
    rbc_elt_add(res[i], src[i], src[i + half_size]);
  }

  if(max % 2 == 1) {
    rbc_elt_set(res[half_size - 1], src[half_size - 1]);
  }
}



void rbc_kar_mul(rbc_vec o, rbc_vec a, rbc_vec b, int32_t a_size, int32_t b_size, rbc_vec stack) {
  int32_t ha_size;
  rbc_vec a2, b2;

  // In every call a_size must be >= b_size
  if(a_size == 1) {
    rbc_elt_mul(o[0], a[0], b[0]);
    return;
  }

  if(a_size == 2) {
    if(b_size == 2) {
      // Hardcoded mul
      rbc_elt ea2, eb2, ed;
      rbc_elt_mul(o[0], a[0], b[0]);
      rbc_elt_mul(o[2], a[1], b[1]);
      rbc_elt_add(ea2, a[0], a[1]);
      rbc_elt_add(eb2, b[0], b[1]);
      rbc_elt_mul(ed, ea2, eb2);
      rbc_elt_add(o[1], o[0], o[2]);
      rbc_elt_add(o[1], o[1], ed);
    } else { // b_size = 1
      rbc_elt_mul(o[0], a[0], b[0]);
      rbc_elt_mul(o[1], a[1], b[0]);
    }
    return;
  }

  ha_size = (a_size + 1) / 2;

  // Compute a2 = a0 + a1 and b2 = b0 + b1
  a2 = stack;
  b2 = stack + ha_size;

  rbc_kar_fold(a2, a, a_size, ha_size);
  rbc_kar_fold(b2, b, b_size, ha_size);

  // Computation of d = a2*b2
  
  // Reset the stack
  for(int32_t i = 2 * ha_size ; i < 4 * ha_size ; ++i) {
    rbc_elt_set_zero(stack[i]);
  }

  rbc_kar_mul(stack + 2 * ha_size, a2, b2, ha_size, ha_size, stack + 4 * ha_size);

  for(int32_t i = 0 ; i < 2 * (a_size - ha_size - 1) + 1 + 2 * ha_size ; ++i) {
    rbc_elt_set_zero(o[i]);
  }

  // Computation of c0 = a0*b0 in the low part of o
  rbc_kar_mul(o, a, b, ha_size, ha_size, stack + 4 * ha_size);

  // Computation of c2 = a1*b1 in the high part of o
  rbc_kar_mul(o + 2 * ha_size, a + ha_size, b + ha_size, a_size - ha_size, b_size - ha_size, stack + 4 * ha_size);

  // Computation of c1 = d + c2 + c0
  for(int32_t i = 0 ; i < 2 * (a_size - ha_size - 1) + 1 ; ++i) {
    rbc_elt_add(stack[i + 2 * ha_size], stack[i + 2 * ha_size], (o + 2 * ha_size)[i]);
  }

  for(int32_t i = 0 ; i < 2 * (ha_size - 1) + 1 ; ++i) {
    rbc_elt_add(stack[i + 2 * ha_size], stack[i + 2 * ha_size], o[i]);
  }

  // Add c1 to o
  for(int32_t i = 0 ; i <= 2 * (ha_size - 1) + 1 ; i++) {
    rbc_elt_add(o[i + ha_size], o[i + ha_size], stack[i + 2 * ha_size]);
  }
}



/**
 * \fn void rbc_poly_mul(rbc_poly o, const rbc_poly p1, const rbc_poly p2)
 * \brief This function multiplies two polynomials.
 *
 * \param[in] o Product of <b>p1</b> and <b>p2</b>
 * \param[in] p1 Polynomial
 * \param[in] p2 Polynomial
 */
void rbc_poly_mul(rbc_poly o, const rbc_poly a, const rbc_poly b) {
  if(a->max_degree <= 1 || b->max_degree <= 1) {
    rbc_plain_mul(o, a, b);
    return;
  }

  rbc_poly_mul2(o, a, b, a->max_degree, b->max_degree);
}



/**
 * \fn void rbc_poly_mul2(rbc_poly o, const rbc_poly a, const rbc_poly b, int32_t a_degree, int32_t b_degree)
 * \brief This function multiplies two polynomials using a_degree and b_degree as polynomials degrees.
 *
 * \param[out] o Product of <b>a</b> and <b>b</b>
 * \param[in] a Polynomial
 * \param[in] b Polynomial
 * \param[in] a_degree Degree to consider for a
 * \param[in] b_degree Degree to consider for b
 */
void rbc_poly_mul2(rbc_poly o, const rbc_poly a, const rbc_poly b, int32_t a_degree, int32_t b_degree) {
  int32_t max_degree;
  int32_t stack_size = 0;
  int32_t n;

  // Prepare polynomials such that the allocated space is big enough in every polynomial
  max_degree = max(a_degree, b_degree);

  n = max_degree;
  do {
    stack_size += 4 * ((n + 2) >> 1);
    n >>= 1;
  } while(n > 1);

  rbc_vec stack;
  rbc_vec_init(&stack, stack_size);

  rbc_kar_mul(o->v, a->v, b->v, max_degree + 1, max_degree + 1, stack);

  rbc_vec_clear(stack);
}
