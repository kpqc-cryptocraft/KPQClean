


/**
 * \fn void rbc_qre_mul(rbc_qre o, const rbc_qre p1, const rbc_qre p2)
 * \brief This function multiplies two rbc_qre.
 *
 * \param[out] o Product of <b>p1</b> and <b>p2</b>
 * \param[in] p1 rbc_qre
 * \param[in] p2 rbc_qre
 */
void rbc_qre_mul(rbc_qre o, const rbc_qre p1, const rbc_qre p2) {
  if(rbc_init_qre_modulus == 0) {
    printf("Call to rbc_qre_mul with uninitialized modulus\n");
    exit(1);
  }

  rbc_poly_mulmod_sparse(o, p1, p2, rbc_qre_modulus);
}
