


/** 
 * \fn void rbc_vspace_set_random_full_rank(random_source* ctx, rbc_vspace o, uint32_t size)
 * \brief This function sets a vector space with random values using the NIST seed expander. The returned vector space has full rank.
 *
 * \param[out] ctx random_source
 * \param[out] o rbc_vspace
 * \param[in] size Size of the rbc_vspace
 */
void rbc_vspace_set_random_full_rank(random_source* ctx, rbc_vspace o, uint32_t size) {
  rbc_vec_set_random_full_rank(ctx, o, size);
}
