


/**
 * \fn void rbc_poly_print(const rbc_poly p)
 * \brief This function displays a polynomial.
 *
 * \param[in] p rbc_poly
 */
void rbc_poly_print(const rbc_poly p) {
  for(int i = 0 ; i < p->max_degree + 1 ; ++i) {
    if (i < 10) printf("\n%i   - ", i);
    else if (i < 100) printf("\n%i  - ", i);
    else printf("\n%i - ", i);
    rbc_elt_print(p->v[i]);
  }
}
