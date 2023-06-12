


/**
 * \fn void rbc_qre_inv(rbc_qre o, const rbc_qre p)
 * \brief This function performs the extended euclidean algorithm to compute the inverse of p.
 *
 * \param[out] o Inverse of <b>e</b> modulo <b>modulus</b>
 * \param[in] p rbc_qre
 * \param[in] modulus Polynomial
 */
void rbc_qre_inv(rbc_qre o, const rbc_qre p) {
  if(rbc_init_qre_modulus == 0) {
    printf("Call to rbc_qre_inv with uninitialized modulus\n");
    exit(1);
  }

  rbc_poly_inv(o, p, rbc_qre_modulus_inv);
}
