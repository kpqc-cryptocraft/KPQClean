
/**
 * \fn rbc_elt_inv(rbc_elt o, const rbc_elt e)
 * \brief This function computes the multiplicative inverse of a finite field element.
 *
 * \param[out] o rbc_elt
 * \param[in] e rbc_elt
 */
void rbc_elt_inv(rbc_elt o, const rbc_elt e) {
  rbc_elt tmp;

  rbc_elt_set(tmp, e);
  for(int i = 0 ; i < (RBC_FIELD_M - 2) ; i++) {
    rbc_elt_sqr(o, tmp);
    rbc_elt_mul(tmp, o, e);
  }
  rbc_elt_sqr(o, tmp);
}

