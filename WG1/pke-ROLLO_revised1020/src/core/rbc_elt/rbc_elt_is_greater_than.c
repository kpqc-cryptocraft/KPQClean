
/**
 * \fn uint8_t rbc_elt_is_greater_than(const rbc_elt e1, const rbc_elt e2)
 * \brief This functions compares two finite field elements.
 *
 * \param[in] e1 rbc_elt
 * \param[in] e2 rbc_elt
 * \return 1 if <b>e1</b> > <b>e2</b>, 0 otherwise
 */
uint8_t rbc_elt_is_greater_than(const rbc_elt e1, const rbc_elt e2) {
  int8_t result = 0, flag = 0, tmp = 0;

  for(int8_t i = RBC_ELT_DATA_SIZE - 1 ; i >= 0 ; i--) {
    tmp = (e1[i] > e2[i]);
    result = (tmp && !flag) || (flag && result);

    if(i != 0) {
      flag = tmp || flag;
      tmp = (e1[i] < e2[i]);
      flag = tmp || flag;
    }
  }

  return result;
}

