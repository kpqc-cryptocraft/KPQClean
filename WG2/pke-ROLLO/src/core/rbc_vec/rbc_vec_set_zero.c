


/**
 * \fn void rbc_vec_set_zero(rbc_vec v, uint32_t size)
 * \brief This function sets a vector of finite elements to zero.
 *
 * \param[out] v rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_set_zero(rbc_vec v, uint32_t size) {
  for(size_t i = 0 ; i < size ; ++i) {
    rbc_elt_set_zero(v[i]);
  }
}
