


/**
 * \fn void rbc_poly_init(rbc_poly* p, int32_t degree)
 * \brief This function allocates the memory for a rbc_poly element.
 *
 * \param[out] p Pointer to the allocated rbc_poly
 * \param[in] degree Degree of the polynomial
 */
void rbc_poly_init(rbc_poly* p, int32_t degree) {
  *p = malloc(sizeof(rbc_poly_struct));
  if(p == NULL) exit(EXIT_FAILURE);
  rbc_vec_init(&((*p)->v), degree + 1);
  (*p)->max_degree = degree;
  (*p)->degree = degree;
}
