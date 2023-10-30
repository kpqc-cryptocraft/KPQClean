


/**
 * \fn uint32_t rbc_vec_get_rank(const rbc_vec v, uint32_t size)
 * \brief This function computes the rank of a vector of finite field elements.
 *
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 *
 * \return Rank of the vector <b>v</b>
 */
uint32_t rbc_vec_get_rank(const rbc_vec v, uint32_t size) {
  rbc_vec copy;
  uint32_t dimension;

  rbc_vec_init(&copy, size);
  rbc_vec_set(copy, v, size);
  dimension = rbc_vec_gauss(copy, size, NULL, 0);
  rbc_vec_clear(copy);

  return dimension;
}



/**
 * \fn uint32_t rbc_vec_get_rank_vartime(const rbc_vec v, uint32_t size)
 * \brief This function computes the rank of a vector of finite field elements.
 *
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 *
 * \return Rank of the vector <b>v</b>
 */
uint32_t rbc_vec_get_rank_vartime(const rbc_vec v, uint32_t size) {
  rbc_vec copy;
  uint32_t dimension;

  rbc_vec_init(&copy, size);
  rbc_vec_set(copy, v, size);
  dimension = rbc_vec_gauss_vartime(copy, size, NULL, 0);
  rbc_vec_clear(copy);

  return dimension;
}
