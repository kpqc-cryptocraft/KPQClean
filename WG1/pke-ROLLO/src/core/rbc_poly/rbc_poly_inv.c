/**
 * \fn void rbc_poly_inv(rbc_poly o, const rbc_poly a, const rbc_poly modulus)
 * \brief This function uses the Bernstein Yang algorithm and its implementation is inspired from the paper "A Constant-time AVX2 Implementation of a Variant of ROLLO"
 *
 * \param[out] o Inverse of <b>a</b> modulo <b>modulus</b>
 * \param[in] a Polynomial
 * \param[in] modulus Polynomial
 */
 void rbc_poly_inv(rbc_poly o, const rbc_poly a, const rbc_poly modulus) {
  int size = modulus->max_degree;

  rbc_poly f_prime, g_prime;
  rbc_poly_init(&f_prime, size);
  rbc_poly_init(&g_prime, size);

  rbc_poly tmp;
  rbc_poly_init(&tmp, size);

  rbc_poly v, r;
  rbc_poly_init(&v, size);
  rbc_poly_init(&r, size);
  rbc_poly_set_zero(v, size);
  rbc_poly_set_zero(r, size);

  rbc_elt_set_one(r->v[0]);

  //Reverse f and g
  for(int i=0 ; i<size + 1 ; i++) rbc_elt_set(f_prime->v[i], modulus->v[size - i]);
  for(int i=0 ; i<size ; i++) rbc_elt_set(g_prime->v[i], a->v[size - 1 - i]);

  rbc_elt f0, g0, tmp_elt;

  rbc_elt_int delta = 1;
  rbc_elt_uint mask;

  int n = 2*size - 1;
  int s = 1;

  int n1;
  int s1 = s;

  while(n > 0) {
    n1 = (n > size + 1 ? size + 1: n);
    s1 = (s > size ? size : s);

    //Multiply v by x
    for(int i=s1 ; i>0 ; i--) rbc_elt_set(v->v[i], v->v[i-1]);
    rbc_elt_set_zero(v->v[0]);

    //mask = 1 if delta > 0 && !rbc_elt_is_zero(g(0))
    mask = ((-delta) >> (RBC_INTEGER_LENGTH - 1) & (1 - rbc_elt_is_zero(g_prime->v[0])));

    //Conditional swaps
    rbc_vec_add(tmp->v, f_prime->v, g_prime->v, n1);
    for(int i=0 ; i<n1 ; i++) {
      for(int j=0 ; j<RBC_ELT_SIZE ; j++) tmp->v[i][j] &= -mask;
    }
    rbc_vec_add(f_prime->v, f_prime->v, tmp->v, n1);
    rbc_vec_add(g_prime->v, g_prime->v, tmp->v, n1);

    rbc_vec_add(tmp->v, v->v, r->v, s1);
    for(int i=0 ; i<s1 ; i++) {
      for(int j=0 ; j<RBC_ELT_SIZE ; j++) tmp->v[i][j] &= -mask;
    }
    rbc_vec_add(v->v, v->v, tmp->v, s1);
    rbc_vec_add(r->v, r->v, tmp->v, s1);

    delta += (((-delta) & (-mask)) + ((-delta) & (-mask)) + 1);

    rbc_elt_set(f0, f_prime->v[0]);
    rbc_elt_set(g0, g_prime->v[0]);

    //g = f0 g - g0 f
    for(int i=0 ; i<n1 ; i++) {
     
      rbc_elt_mul(g_prime->v[i], g_prime->v[i], f0);
      rbc_elt_mul(tmp_elt, f_prime->v[i], g0);
      rbc_elt_add(g_prime->v[i], g_prime->v[i], tmp_elt);
    }

    //r = f0 r - g0 v
    for(int i=0 ; i<s1 ; i++) {
      rbc_elt_mul(r->v[i], r->v[i], f0);
      rbc_elt_mul(tmp_elt, v->v[i], g0);
      rbc_elt_add(r->v[i], r->v[i], tmp_elt);
    }

    //Divide g by x
    for(int i=0 ; i<n1-1 ; i++) {
      rbc_elt_set(g_prime->v[i], g_prime->v[i+1]);
    }

    rbc_elt_set_zero(g_prime->v[n1-1]);

    n--;
    s++;
  }

  rbc_elt_inv(tmp_elt, f_prime->v[0]);

  for(int i=0 ; i<size ; i++) {
    rbc_elt_mul(v->v[i], v->v[i], tmp_elt);
    rbc_elt_set(o->v[size - 1 - i], v->v[i]);
  }

  rbc_poly_clear(f_prime);
  rbc_poly_clear(g_prime);

  rbc_poly_clear(tmp);

  rbc_poly_clear(v);
  rbc_poly_clear(r);
 }