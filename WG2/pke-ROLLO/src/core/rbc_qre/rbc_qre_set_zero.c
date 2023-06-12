


/**
 * \fn void rbc_qre_set_zero(rbc_qre o)
 * \brief This functions sets a rbc_qre to zero.
 *
 * \param[in] o rbc_qre
 */
void rbc_qre_set_zero(rbc_qre o) {
  rbc_poly_set_zero(o, RBC_QRE_DEG);
}
