


/**
 * \fn void rbc_poly_add(rbc_poly o, const rbc_poly p1, const rbc_poly p2)
 * \brief This function adds two polynomials.
 *
 * \param[out] o Sum of <b>p1</b> and <b>p2</b>
 * \param[in] p1 Polynomial
 * \param[in] p2 Polynomial
 */
void rbc_poly_add(rbc_poly o, const rbc_poly p1, const rbc_poly p2) {
  rbc_vec_add(o->v, p1->v, p2->v, p1->max_degree + 1);
}
