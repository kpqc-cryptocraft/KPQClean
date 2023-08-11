


/**
 * \fn void rbc_poly_sparse_init(rbc_poly_sparse* p, uint32_t coeff_nb, uint32_t *coeffs)
 * \brief This function allocates the memory for a rbc_poly_sparse element.
 *
 * \param p Pointer to the allocated rbc_poly_sparse
 * \param[in] coeff_nb Number of coefficients
 * \param[in] coeffs Coefficients
 */
void rbc_poly_sparse_init(rbc_poly_sparse* p, uint32_t coeff_nb, uint32_t *coeffs) {
  *p = malloc(sizeof(rbc_poly_sparse_struct));
  if(p == NULL) exit(EXIT_FAILURE);
  (*p)->coeffs_nb = coeff_nb;
  (*p)->coeffs = malloc(coeff_nb * sizeof(uint32_t));
  if((*p)->coeffs == NULL) exit(EXIT_FAILURE);
  memcpy((*p)->coeffs, coeffs, coeff_nb * sizeof(uint32_t));
}
