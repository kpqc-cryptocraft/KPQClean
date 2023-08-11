


/**
 * \fn void rbc_poly_from_string(rbc_poly p, const uint8_t* str)
 * \brief This function parses a string into a polynomial.
 *
 * \param[out] p rbc_poly
 * \param[in] str String to parse
 */
void rbc_poly_from_string(rbc_poly p, const uint8_t* str) {
  rbc_vec_from_string(p->v, p->max_degree + 1, str);
}
