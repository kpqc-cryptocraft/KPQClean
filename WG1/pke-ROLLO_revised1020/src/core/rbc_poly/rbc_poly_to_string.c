


/**
 * \fn void rbc_poly_to_string(uint8_t* str, const rbc_poly p)
 * \brief This function parses a polynomial into a string.
 *
 * \param[out] str String
 * \param[in] p rbc_poly
 */
void rbc_poly_to_string(uint8_t* str, const rbc_poly p) {
  rbc_vec_to_string(str, p->v, p->max_degree + 1);
}
