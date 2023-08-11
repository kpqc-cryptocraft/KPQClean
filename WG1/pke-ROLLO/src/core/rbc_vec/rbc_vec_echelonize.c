


/**
 * \fn uint32_t rbc_vec_echelonize(rbc_vec v, uint32_t size)
 * \brief This function computes the row reduced echelon form of a vector of finite field elements and returns its rank.
 *
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 *
 * \return Rank of the vector <b>v</b>
 */
uint32_t rbc_vec_echelonize(rbc_vec v, uint32_t size) {
  uint32_t degree;
  uint32_t dimension = rbc_vec_gauss(v, size, NULL, 0);

  for(int32_t i = dimension - 1 ; i >= 1 ; i--) {
    degree = rbc_elt_get_degree(v[i]);

    for(int32_t j = i - 1 ; j >= 0 ; j--) {
      if(rbc_elt_get_coefficient_vartime(v[j], degree) == 1) {
        rbc_elt_add(v[j], v[j], v[i]);
      }
    }
  }

  return dimension;
}
