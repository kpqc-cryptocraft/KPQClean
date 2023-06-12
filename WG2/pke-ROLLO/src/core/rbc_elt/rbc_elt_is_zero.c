
/**
 * \fn uint8_t rbc_elt_is_zero(const rbc_elt e)
 * \brief This functions tests if a finite field element is equal to zero.
 *
 * \param[in] e rbc_elt
 * \return 1 if <b>e</b> is equal to zero, 0 otherwise
 */
uint8_t rbc_elt_is_zero(const rbc_elt e) {
  int8_t result = 1;
  for(int i = RBC_ELT_DATA_SIZE - 1; i >= 0 ; i--) {
    result &= (e[i] == 0);
  }
  return result;
}

