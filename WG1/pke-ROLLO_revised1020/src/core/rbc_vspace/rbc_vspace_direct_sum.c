


/**
 * \fn void rbc_vspace_direct_sum(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size)
 * \brief This function computes the direct sum of two vector spaces.
 *
 * \param[out] o Direct sum of vs1 and vs2
 * \param[in] vs1 rbc_vspace
 * \param[in] vs2 rbc_vspace
 * \param[in] vs1_size Size of vs1
 * \param[in] vs2_size Size of vs2
 */
void rbc_vspace_direct_sum(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size) {
  for(size_t i = 0; i < vs1_size; i++) {
    rbc_elt_set(o[i], vs1[i]);
  }

  for(size_t i = 0; i < vs2_size; i++) {
    rbc_elt_set(o[i + vs1_size], vs2[i]);
  }
}
