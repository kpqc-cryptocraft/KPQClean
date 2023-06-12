


/**
 * \fn void rbc_vec_set_random_pair_from_support(random_source* o, rbc_vec o1, rbc_vec o2, uint32_t size, const rbc_vec support, uint32_t support_size)
 * \brief This function sets a pair of vector with random values using the NIST seed expander. The support of the vector (o1 || o2) is the one given in input.
 *
 * This function copies the support vector in rank random positions of <b>o1</b> or <b>o2</b>. Next, all the remaining coordinates of <b>o1</b> and <b>o2</b> are set using random linear combinations of the support coordinates. 
 *
 * This function should not be called with size < support_size.
 * 
 * \param[out] ctx random source
 * \param[out] o1 rbc_vec
 * \param[out] o2 rbc_vec
 * \param[in] size Size of <b>o1</b> and <b>o2</b>
 * \param[in] support Support
 * \param[in] support_size Size of the support
 * \param[in] copy_flag If not 0, the support is copied into random coordinates of the resulting vector
 */
void rbc_vec_set_random_pair_from_support(random_source* ctx, rbc_vec o1, rbc_vec o2, uint32_t size, const rbc_vec support, uint32_t support_size, uint8_t copy_flag) {
  if (copy_flag) {
    uint32_t random1_size = 4 * support_size;
    uint32_t random2_size = (support_size * (2 * size - support_size)) / 8 + 2;
    uint8_t random1[random1_size];
    uint8_t random2[random2_size];

    // Copy the support vector in support_size random positions of o
    random_get_bytes(ctx, random1, random1_size);

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t position;

    while(i != support_size) {
      position = random1[j];

      // Check that the position is not already taken
      if(position < size * (256 / size)) {
        if(random1[j + 1] & 0x1) {
          if(rbc_elt_is_zero(o1[position % size])) {
            rbc_elt_set(o1[position % size], support[i]);
            i++;
          }
        } 
        else {
          if(rbc_elt_is_zero(o2[position % size])) {
            rbc_elt_set(o2[position % size], support[i]);
            i++;
          }
        }
      }

      // Get more randomness if necessary
      j = j + 2;
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
      if(rbc_elt_is_zero(o1[i])) {
        for(j = 0 ; j < support_size ; ++j) {
          if(random2[k] & 0x1) {
            rbc_elt_add(o1[i], support[j], o1[i]);
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

    k++;

    for(i = 0 ; i < size ; ++i) {
      if(rbc_elt_is_zero(o2[i])) {
        for(j = 0 ; j < support_size ; ++j) {
          if(random2[k] & 0x1) {
            rbc_elt_add(o2[i], support[j], o2[i]);
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
    //Set all the coordinates to random linear combinations of the support
    uint32_t random_size = 2 * support_size * size / 8 + 1;
    uint8_t random[random_size];
    random_get_bytes(ctx, random, random_size);

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t l = 0;

    for(i = 0 ; i < size ; ++i) {
      for(j = 0 ; j < support_size ; ++j) {
        if(random[k] & 0x1) {
          rbc_elt_add(o1[i], support[j], o1[i]);
        }

        random[k] = random[k] >> 1;
        l++;
        if(l == 8) {
          l = 0;
          k++;
        }
      }
    }

    for(i = 0 ; i < size ; ++i) {
      for(j = 0 ; j < support_size ; ++j) {
        if(random[k] & 0x1) {
          rbc_elt_add(o2[i], support[j], o2[i]);
        }

        random[k] = random[k] >> 1;
        l++;
        if(l == 8) {
          l = 0;
          k++;
        }
      }
    }
  }
}
