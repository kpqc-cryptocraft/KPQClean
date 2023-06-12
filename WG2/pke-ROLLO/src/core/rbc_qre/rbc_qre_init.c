


/**
 * \fn void rbc_qre_init(rbc_qre* p)
 * \brief This function allocates the memory for a rbc_qre element.
 *
 * \param[out] p Pointer to the allocated rbc_qre
 */
void rbc_qre_init(rbc_qre* p) {
  rbc_poly_init(p, RBC_QRE_DEG);
}
