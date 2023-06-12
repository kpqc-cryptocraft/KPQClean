
/**
 * \fn void rbc_elt_set_random
 * \brief This function sets a finite field element with random values using the NIST seed expander.
 *
 * \param[out] ctx random_source
 * \param[out] o rbc_elt
 */
void rbc_elt_set_random(random_source* ctx, rbc_elt o) {
  uint32_t bytes = (RBC_FIELD_M % 8 == 0) ? RBC_FIELD_M / 8 : RBC_FIELD_M / 8 + 1;
  uint8_t random[bytes];
  uint8_t mask = (1 << RBC_FIELD_M % 8) - 1;

  rbc_elt_set_zero(o);
  random_get_bytes(ctx, random, bytes);
  random[bytes - 1] &= mask;
  memcpy((uint64_t*) o, random, bytes);
}
