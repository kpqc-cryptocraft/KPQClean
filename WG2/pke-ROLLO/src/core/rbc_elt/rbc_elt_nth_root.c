/**
 * \fn void rbc_elt_nth_root(rbc_elt o, const rbc_elt e, uint32_t n)
 * \brief This function computes the nth root of a finite field element.
 *
 * \param[out] o Nth root of <b>e</b>
 * \param[in] e rbc_elt
 * \param[in] n Parameter defining the nth root
 */
void rbc_elt_nth_root(rbc_elt o, const rbc_elt e, uint32_t n) {
  uint32_t exp = n * (RBC_FIELD_M - 1) % RBC_FIELD_M;

  rbc_elt_sqr(o, e);
  for(size_t i = 0 ; i < exp - 1 ; ++i) {
    rbc_elt_sqr(o, o);
  }
}

