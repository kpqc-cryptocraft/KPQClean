


/**
 * \fn void rbc_poly_sparse_clear(rbc_poly_sparse p)
 * \brief This function clears the memory allocated for a rbc_poly_sparse element.
 *
 * \param p Polynomial
 */
void rbc_poly_sparse_clear(rbc_poly_sparse p) {
  free(p->coeffs);
  free(p);
}
