


/**
 * \fn void rbc_qre_set_random_full_rank_with_one(random_source* ctx, rbc_qre o)
 * \brief This function sets a rbc_qre with random values using the NIST seed expander. The returned rbc_qre has full rank and contains one.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_qre
 */
void rbc_qre_set_random_full_rank_with_one(random_source* ctx, rbc_qre o) {
  rbc_poly_set_random_full_rank_with_one(ctx, o, RBC_QRE_DEG);
}
