


/**
 * \fn void rbc_poly_clear(rbc_poly p)
 * \brief This function clears the memory allocated for a rbc_poly element.
 *
 * \param p Polynomial
 */
void rbc_poly_clear(rbc_poly p) {
  rbc_vec_clear(p->v);
  free(p);
  p = NULL;
}

