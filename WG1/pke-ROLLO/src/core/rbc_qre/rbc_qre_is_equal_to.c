


/**
 * \fn uint8_t rbc_qre_is_equal_to(const rbc_qre p1, const rbc_qre p2)
 * \brief This function test if two rbc_qre are equal.
 *
 * \param[in] p1 rbc_qre
 * \param[in] p2 rbc_qre
 * \return 1 if the rbc_qre are equal, 0 otherwise
 */
uint8_t rbc_qre_is_equal_to(const rbc_qre p1, const rbc_qre p2) {
  return rbc_poly_is_equal_to(p1, p2);
}
