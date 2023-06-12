


/**
 * \fn void rbc_qre_set_random_from_support(random_source* ctx, rbc_qre o, const rbc_vspace support, uint32_t support_size)
 * \brief This function sets a rbc_qre with random values using the NIST seed expander. The support of the rbc_qre returned by this function is the one given in input.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_qre
 * \param[in] support Support of <b>o</b>
 * \param[in] support_size Size of the support
 * \param[in] copy_flag If not 0, the support is copied into random coordinates of the resulting vector
 */
void rbc_qre_set_random_from_support(random_source* ctx, rbc_qre o, const rbc_vspace support, uint32_t support_size, uint8_t copy_flag) {
  rbc_poly_set_random_from_support(ctx, o, RBC_QRE_DEG, support, support_size, copy_flag);
}
