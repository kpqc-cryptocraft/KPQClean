


/**
 * \fn void rbc_qre_init_modulus(uint32_t degree) {
 * \brief This function init the quotient ring modulus corresponding to PARAM_N.
 *
 * \param[in] degree Degree of the modulus
 */
void rbc_qre_init_modulus(uint32_t degree) {
  if(rbc_init_qre_modulus == 0) {
    RBC_QRE_DEG = degree - 1;
    rbc_qre_modulus = rbc_qre_get_modulus(degree);
    rbc_poly_init(&rbc_qre_modulus_inv, RBC_QRE_DEG + 1);
    rbc_poly_set_zero(rbc_qre_modulus_inv, RBC_QRE_DEG + 1);
    for(uint32_t i = 0 ; i < rbc_qre_modulus->coeffs_nb ; i++) {
      rbc_elt_set_one(rbc_qre_modulus_inv->v[rbc_qre_modulus->coeffs[i]]);
    }
  }
  if(rbc_init_qre_modulus != 0)
  {
   printf("Err: qre_modulus is not working\n");
  }  

  rbc_init_qre_modulus++;
}
