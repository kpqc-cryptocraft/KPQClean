


/**
 * \fn void rbc_vec_print(rbc_vec v, uint32_t size)
 * \brief Display an rbc_vec element.
 *
 * \param[out] v rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_print(rbc_vec v, uint32_t size) {
  printf("[\n");
  for(size_t i = 0 ; i < size ; ++i) {
    rbc_elt_print(v[i]);
    printf("\n");
  }
  printf("]\n");
}
