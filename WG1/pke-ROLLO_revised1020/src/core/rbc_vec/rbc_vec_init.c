


/**
 * \fn void rbc_vec_init(rbc_vec* v, uint32_t size)
 * \brief This function initiates a rbc_vec element.
 *
 * \param[out] v Pointer to a rbc_vec element
 * \param[in] size Size of the vector
 */
void rbc_vec_init(rbc_vec* v, uint32_t size) {
  *v = calloc(size, sizeof(rbc_elt));
  if(v == NULL) exit(EXIT_FAILURE);
}
