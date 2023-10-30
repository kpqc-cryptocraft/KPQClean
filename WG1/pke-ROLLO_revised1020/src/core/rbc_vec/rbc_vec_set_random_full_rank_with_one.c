

/**
 * \fn void rbc_vec_set_random_full_rank_with_one(random_source* ctx, rbc_vec o, uint32_t size) {
 * \brief This function sets a vector with random values using the NIST seed expander. The vector returned by this function has full rank and contains one as the last coordinate.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_set_random_full_rank_with_one(random_source* ctx, rbc_vec o, uint32_t size) {
  int32_t rank_max = RBC_FIELD_M < size ? RBC_FIELD_M : size;
  int32_t rank = -1; int32_t cnt = 0;
  int32_t WHILE_MAX = size + 2;
  while(rank != rank_max || cnt < WHILE_MAX)  
  {
    rbc_vec_set_random(ctx, o, size - 1);
    if(rank != rank_max)
      rbc_elt_set_one(o[size - 1]);
    rank = rbc_vec_get_rank(o, size);
    cnt++;
  }
}
