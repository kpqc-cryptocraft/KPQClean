


/**
 * \fn void rbc_qre_to_string(uint8_t* str, const rbc_qre p)
 * \brief This function parses a rbc_qre into a string.
 *
 * \param[out] str String
 * \param[in] e rbc_qre
 */
void rbc_qre_to_string(uint8_t* str, const rbc_qre p) {
  rbc_poly_to_string(str, p);
}
