


/**
 * \fn void rbc_poly_set_zero(rbc_poly o, int32_t degree)
 * \brief This functions sets a polynomial to zero.
 *
 * \param[in] p Polynomial
 * \param[in] degree Degree of the polynomial
 */
void rbc_poly_set_zero(rbc_poly o, int32_t degree) {
  rbc_vec_set_zero(o->v, degree + 1);
}
