


/**
 * \fn void rbc_vec_scalar_mul(rbc_vec o, const rbc_vec v, const rbc_elt e, uint32_t size)
 * \brief This functions multiplies a vector of finite field elements by a scalar.
 *
 * \param[out] o rbc_vec equal to \f$ e \times v \f$
 * \param[in] v rbc_vec
 * \param[in] e Finite field element
 * \param[in] size Size of the vector
 */
void rbc_vec_scalar_mul(rbc_vec o, const rbc_vec v, const rbc_elt e, uint32_t size) {
  for(size_t i = 0 ; i < size ; ++i) {
    rbc_elt_mul(o[i], v[i], e);
  }
}
