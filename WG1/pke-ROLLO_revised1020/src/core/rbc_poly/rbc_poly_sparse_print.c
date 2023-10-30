


/**
 * \fn void rbc_poly_sparse_print(const rbc_poly_sparse p)
 * \brief This function displays a sparse polynomial.
 *
 * \param[in] p rbc_poly_sparse
 */
void rbc_poly_sparse_print(const rbc_poly_sparse p) {
  for(size_t i = 0 ; i < p->coeffs_nb - 1 ; i++) {
    printf("X^%" PRIu32 " + ", p->coeffs[i]);
  }
  printf("X^%" PRIu32 "\n", p->coeffs[p->coeffs_nb - 1]);
}
