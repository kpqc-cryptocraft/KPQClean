


/**
 * \fn void rbc_vec_to_string(uint8_t* str, const rbc_vec v, uint32_t size)
 * \brief This function parses a vector of finite field elements into a string.
 *
 * \param[out] str Output string
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 */
void rbc_vec_to_string(uint8_t* str, const rbc_vec v, uint32_t size) {
  uint32_t bytes1 = RBC_FIELD_M / 8;
  uint32_t bytes2 = RBC_FIELD_M % 8;
  uint32_t index = bytes1 * size;
  uint32_t str_size = ((size * RBC_FIELD_M) % 8 == 0) ? (size * RBC_FIELD_M) / 8 : (size * RBC_FIELD_M) / 8 + 1;

  memset(str, 0, str_size);

  for(size_t i = 0 ; i < size ; i++) {
    memcpy(str + i * bytes1, v[i], bytes1);
  }

  uint8_t k = 0;
  for(size_t i = 0 ; i < size ; i++) {
    for(size_t j = 1 ; j <= bytes2 ; j++) {
      uint8_t bit = rbc_elt_get_coefficient_vartime(v[i], RBC_FIELD_M - j);
      *(str + index) |= (bit << k % 8);
      k++;
      if(k % 8 == 0) index++;
    }
  }
}
