


/**
 * \fn void rbc_vspace_set_random_full_rank_with_one(random_source* ctx, rbc_vspace o, uint32_t size) {
 * \brief This function sets a rbc_vspace with random values using the NIST seed expander. The rbc_vspace returned by this function has full rank and contains one.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_vspace
 * \param[in] size Size of rbc_vspace
 */
void rbc_vspace_set_random_full_rank_with_one(random_source* ctx, rbc_vspace o, uint32_t size) {
  rbc_vec_set_random_full_rank_with_one(ctx, o, size);
}
