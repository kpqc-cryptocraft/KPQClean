
/**
 * \fn uint8_t rbc_elt_is_equal_to(const rbc_elt e1, const rbc_elt e2)
 * \brief This functions tests if two finite field elements are equal.
 *
 * \param[in] e1 rbc_elt
 * \param[in] e2 rbc_elt
 * \return 1 if <b>e1</b> and <b>e2</b> are equal, 0 otherwise
 */
uint8_t rbc_elt_is_equal_to(const rbc_elt e1, const rbc_elt e2) {
  int8_t result = 1;

  for(uint8_t i = 0 ; i < RBC_ELT_DATA_SIZE ; i++) {
    result &= (e1[i] == e2[i]);
  }

  return result;
}

