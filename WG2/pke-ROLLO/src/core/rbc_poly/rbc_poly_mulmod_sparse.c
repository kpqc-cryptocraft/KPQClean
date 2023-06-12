


/**
 * \fn void rbc_poly_mulmod_sparse(rbc_poly o, const rbc_poly p1, const rbc_poly p2, const rbc_poly_sparse modulus)
 * \brief This function computes the product of two polynomials modulo a sparse one.
 *
 * \param[out] o Product of <b>p1</b> and <b>p2</b> modulo <b>modulus</b>
 * \param[in] p1 Polynomial
 * \param[in] p2 Polynomial
 * \param[in] modulus Sparse polynomial
 */
void rbc_poly_mulmod_sparse(rbc_poly o, const rbc_poly p1, const rbc_poly p2, const rbc_poly_sparse modulus) {
  int32_t modulus_degree = modulus->coeffs[modulus->coeffs_nb - 1];
  rbc_poly unreduced;

  rbc_poly_init(&unreduced, 2 * modulus_degree - 1);
  rbc_poly_set_zero(unreduced, 2 * modulus_degree - 1);

  rbc_poly_mul2(unreduced, p1, p2, modulus_degree - 1, modulus_degree - 1);

  // Modular reduction
  for(int32_t i = unreduced->max_degree - modulus_degree ; i >= 0 ; i--) {
    for(size_t j = 0 ; j < modulus->coeffs_nb - 1 ; j++) {
      rbc_elt_add(unreduced->v[i + modulus->coeffs[j]],
      unreduced->v[i + modulus->coeffs[j]], unreduced->v[i + modulus_degree]);
    }
    rbc_elt_set_zero(unreduced->v[i + modulus_degree]);
  }

  rbc_poly_set(o, unreduced);
  rbc_poly_clear(unreduced);
}
