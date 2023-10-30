


/**
 * \fn void rbc_vspace_product(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size)
 * \brief This function computes the product vector space of a and b.
 *
 * \param[out] o Product vector space
 * \param[in] vs1 rbc_vspace
 * \param[in] vs2 rbc_vspace
 * \param[in] vs1_size Size of a
 * \param[in] vs2_size Size of b
 */
void rbc_vspace_product(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size) {
  for(size_t i = 0 ; i < vs1_size ; i++) {
    for(size_t j = 0 ; j < vs2_size ; j++) {
      rbc_elt_mul(o[i * vs2_size + j], vs1[i], vs2[j]);
    }
  }
}
