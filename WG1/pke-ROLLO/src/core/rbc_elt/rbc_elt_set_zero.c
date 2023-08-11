
/**
 * \fn void rbc_elt_set_zero(rbc_elt o)
 * \brief This function sets a finite field element to zero.
 *
 * \param[out] o rbc_elt
 */
void rbc_elt_set_zero(rbc_elt o) {
  for(uint8_t i = 0 ; i < RBC_ELT_SIZE ; i++) {
    o[i] = 0;
  }
}

