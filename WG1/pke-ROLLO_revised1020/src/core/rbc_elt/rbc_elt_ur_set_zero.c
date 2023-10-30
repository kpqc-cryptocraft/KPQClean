
/**
 * \fn void rbc_elt_ur_set_zero(rbc_elt o)
 * \brief This function sets an unreduced finite field element to zero.
 *
 * \param[out] o rbc_elt_ur
 */
void rbc_elt_ur_set_zero(rbc_elt_ur o) {
  for(uint8_t i = 0 ; i < RBC_ELT_UR_SIZE ; i++) {
    o[i] = 0;
  }
}

