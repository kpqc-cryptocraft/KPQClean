


/**
 * \fn void rbc_vec_from_string(rbc_vec v, uint32_t size, const uint8_t* str)
 * \brief This function parses a string into a vector of finite field elements.
 *
 * \param[out] v rbc_vec
 * \param[in] size Size of the vector
 * \param[in] str String to parse
 */
void rbc_vec_from_string(rbc_vec v, uint32_t size, const uint8_t* str) {
  uint32_t bytes1 = RBC_FIELD_M / 8;
  uint32_t bytes2 = RBC_FIELD_M % 8;
  uint32_t index = bytes1 * size;

  rbc_vec_set_zero(v, size);

  for(size_t i = 0 ; i < size ; i++) {
    memcpy(v[i], str + i * bytes1, bytes1);
  }

  uint8_t k = 0;
  for(size_t i = 0 ; i < size ; i++) {
    for(size_t j = 1 ; j <= bytes2 ; j++) {
      uint8_t bit = (str[index] >> k % 8) & 0x01;
      rbc_elt_set_coefficient_vartime(v[i], RBC_FIELD_M - j, bit);
      k++;
      if(k % 8 == 0) index++;
    }
  }
}
