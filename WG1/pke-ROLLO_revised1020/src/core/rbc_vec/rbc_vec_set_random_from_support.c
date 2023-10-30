


/**
 * \fn void rbc_vec_set_random_from_support(random_source* o, rbc_vec o, uint32_t size, const rbc_vec support, uint32_t support_size)
 * \brief This function sets a vector with random values using the NIST seed expander. The support of the vector returned by this function is included in the one given in input.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_vec
 * \param[in] size Size of <b>o</b>
 * \param[in] support Support of <b>v</b>
 * \param[in] rank Size of the support
 * \param[in] copy_flag If not 0, the support is copied into random coordinates of the resulting vector
 */
void rbc_vec_set_random_from_support(random_source* ctx, rbc_vec o, uint32_t size, const rbc_vec support, uint32_t support_size, uint8_t copy_flag) {
  rbc_vec_set_zero(o, size);

  uint32_t i=0;
  uint32_t j=0;

  if(copy_flag) {
    uint32_t random1_size = 2 * support_size;
    uint32_t random2_size = (support_size * (size - support_size)) / 8 + 1;
    uint8_t random1[random1_size];
    uint8_t random2[random2_size];

    // Copy the support vector in support_size random positions of o
    random_get_bytes(ctx, random1, random1_size);

    uint32_t position;
    while(i != support_size) {
      position = random1[j];

      // Check that the position is not already taken
      if(position < size * (256 / size) && rbc_elt_is_zero(o[position % size])) {
        rbc_elt_set(o[position % size], support[i]);
        i++;
      }

      // Get more randomness if necessary
      j++;
      if(j % random1_size == 0 && i != support_size) {
        random_get_bytes(ctx, random1, random1_size);
        j = 0;
      }
    }

    // Set all the remaining coordinates with random linear combinations of the support coordinates
    random_get_bytes(ctx, random2, random2_size);

    uint32_t k = 0;
    uint32_t l = 0;
    for(i = 0 ; i < size ; ++i) {
      if(rbc_elt_is_zero(o[i])) {

        for(j = 0 ; j < support_size ; ++j) {
          if(random2[k] & 0x1) {
            rbc_elt_add(o[i], support[j], o[i]);
          }

          random2[k] = random2[k] >> 1;
          l++;
          if(l == 8) {
            l = 0;
            k++;
          }

        }
      }
    }
  }
  else {
    // Set all the coordinates with random linear combinations of the support coordinates
    uint32_t random_size = support_size * size / 8 + 1;
    unsigned char random[random_size];
    random_get_bytes(ctx, random, random_size);

    uint32_t k = 0;
    uint32_t l = 0;

    for(i=0 ; i<size ; i++) {
      for(j=0 ; j<support_size ; j++) {
        if(random[k] & 0x01) {
          rbc_elt_add(o[i], o[i], support[j]);
        }

        random[k] = random[k] >> 1;
        l++;
        if(l == 8) {
          l=0;
          k++;
        }
      }
    }
  }
}
