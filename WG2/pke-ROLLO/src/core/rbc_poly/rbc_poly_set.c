


/**
 * \fn void rbc_poly_set(rbc_poly o, const rbc_poly p)
 * \brief This function copies a polynomial into another one.
 *
 * \param[in] o Polynomial
 * \param[in] p Polynomial
 */
void rbc_poly_set(rbc_poly o, const rbc_poly p) {
  rbc_vec_set(o->v, p->v, o->max_degree + 1);
}
