


/**
 * \fn void rbc_qre_set_random_pair_from_support(random_source* ctx, rbc_qre o1, rbc_qre o2, const rbc_vspace support, uint32_t support_size)
 * \brief This function sets a pair of rbc_qre with random values using the NIST seed expander. The support of (o1 || o2) is the one given in input.
 *
 * \param[out] ctx random_source
 * \param[out] o1 rbc_qre
 * \param[out] o2 rbc_qre
 * \param[in] support Support
 * \param[in] support_size Size of the support
 * \param[in] copy_flag If not 0, the support is copied into random coordinates of the resulting vector
 */
void rbc_qre_set_random_pair_from_support(random_source* ctx, rbc_qre o1, rbc_qre o2, const rbc_vspace support, uint32_t support_size, uint8_t copy_flag) {
  rbc_poly_set_random_pair_from_support(ctx, o1, o2, RBC_QRE_DEG, support, support_size, copy_flag);
}
