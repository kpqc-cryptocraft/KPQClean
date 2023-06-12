


/**
 * \fn void rbc_qre_clear_modulus() {
 * \brief This function clears the quotient ring modulus in use.
 */
void rbc_qre_clear_modulus() {
  rbc_init_qre_modulus--;

  if(rbc_init_qre_modulus == 0) {
    rbc_poly_sparse_clear(rbc_qre_modulus);
    rbc_poly_clear(rbc_qre_modulus_inv);
  }
}
