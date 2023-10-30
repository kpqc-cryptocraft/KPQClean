


/**
 * \fn void rbc_vec_set_random(random_source*, rbc_vec v, uint32_t size)
 * \brief This function sets a vector of finite field elements with random values using NIST seed expander.
 *
 * \param[out] ctx random source
 * \param[out] v rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_set_random(random_source* ctx, rbc_vec v, uint32_t size) {
  uint32_t bytes = (RBC_FIELD_M % 8 == 0) ? RBC_FIELD_M / 8 : RBC_FIELD_M / 8 + 1;
  uint8_t random[size * bytes];
  uint8_t mask = (1 << RBC_FIELD_M % 8) - 1;

  rbc_vec_set_zero(v, size);
  random_get_bytes(ctx, random, size * bytes);

  for(size_t i = 0 ; i < size ; ++i) {
    random[(i + 1) * bytes - 1] &= mask;
    memcpy(v[i], random + i * bytes, bytes);
  }
}
