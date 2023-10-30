
/**
 * \fn void rbc_elt_set_one(rbc_elt o)
 * \brief This function sets a finite field element to one.
 *
 * \param[out] o rbc_elt
 */
void rbc_elt_set_one(rbc_elt o) {
  o[0] = 1;
  for(uint8_t i = 1 ; i < RBC_ELT_SIZE ; i++) {
    o[i] = 0;
  }
}

