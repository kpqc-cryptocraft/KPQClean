


/**
 * \fn void rbc_vec_set(rbc_vec o, const rbc_vec v, uint32_t size)
 * \brief This function copies a vector of finite field elements to another one.
 *
 * \param[out] o rbc_vec
 * \param[in] v rbc_vec
 * \param[in] size Size of the vectors
 */
void rbc_vec_set(rbc_vec o, const rbc_vec v, uint32_t size) {
  for(size_t i = 0 ; i < size ; ++i) {
    rbc_elt_set(o[i], v[i]);
  }
}
