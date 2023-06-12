


/**
 * \fn void rbc_vec_set_random_full_rank(random_source* ctx, rbc_vec o, uint32_t size)
 * \brief This function sets a vector with random values using the NIST seed expander. The returned vector has full rank. This functions leaks the number of iterations needed to compute the vector but this gives no informations about the returned vector.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_set_random_full_rank(random_source* ctx, rbc_vec o, uint32_t size) {
  int32_t rank_max = RBC_FIELD_M < size ? RBC_FIELD_M : size;
  int32_t rank = -1;

  while(rank != rank_max) {
    rbc_vec_set_random(ctx, o, size);
    rank = rbc_vec_get_rank(o, size);
  }
}
