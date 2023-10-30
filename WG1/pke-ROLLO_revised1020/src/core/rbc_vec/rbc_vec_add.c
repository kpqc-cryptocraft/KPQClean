


/**
 * \fn void rbc_vec_add(rbc_vec o, const rbc_vec v1, const rbc_vec v2, uint32_t size)
 * \brief This function adds two vectors of finite field elements.
 *
 * \param[out] o Sum of <b>v1</b> and <b>v2</b>
 * \param[in] v1 rbc_vec
 * \param[in] v2 rbc_vec
 * \param[in] size Size of the vectors
 */
void rbc_vec_add(rbc_vec o, const rbc_vec v1, const rbc_vec v2, uint32_t size) {
  for(size_t i = 0 ; i < size ; ++i) {
    rbc_elt_add(o[i], v1[i], v2[i]);
  }
}
