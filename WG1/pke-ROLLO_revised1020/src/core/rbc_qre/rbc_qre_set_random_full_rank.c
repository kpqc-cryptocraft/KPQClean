


/**
 * \fn void rbc_qre_set_random_full_rank(random_source* ctx, rbc_qre o)
 * \brief This function sets a rbc_qre with random values using the NIST seed expander. The returned rbc_qre has full rank.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_qre
 */
void rbc_qre_set_random_full_rank(random_source* ctx, rbc_qre o) {
  rbc_poly_set_random_full_rank(ctx, o, RBC_QRE_DEG);
}
