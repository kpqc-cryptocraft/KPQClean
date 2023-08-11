


/**
 * \fn void rbc_qre_from_string(rbc_qre o, const uint8_t* str)
 * \brief This function parses a string into a rbc_qre.
 *
 * \param[out] o rbc_qre
 * \param[in] str String to parse
 */
void rbc_qre_from_string(rbc_qre o, const uint8_t* str) {
  rbc_poly_from_string(o, str);
}
