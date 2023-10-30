


/**
 * \fn uint8_t rbc_poly_is_equal_to(const rbc_poly p1, const rbc_poly p2)
 * \brief This function test if two polynomials are equal.
 *
 * \param[in] p1 Polynomial
 * \param[in] p2 Polynomial
 * \return 1 if the polynomials are equal, 0 otherwise
 */
uint8_t rbc_poly_is_equal_to(const rbc_poly p1, const rbc_poly p2) {
  for(int32_t i = 0 ; i < p1->max_degree ; ++i) {
    if(rbc_elt_is_equal_to(p1->v[i], p2->v[i]) == 0) {
      return 0;
    }
  }

  return 1;
}
