


/**
 * \fn uint32_t rbc_vspace_intersection(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size)
 * \brief This function computes the intersection of vs1 and vs2.
 *
 * \param[out] o Intersection of vs1 and vs2
 * \param[in] vs1 rbc_vspace
 * \param[in] vs2 rbc_vspace
 * \param[in] vs1_size Size of vs1
 * \param[in] vs2_size Size of vs2
 *
 * \return Dimension of o
 */
uint32_t rbc_vspace_intersection(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size) {
  if(vs1_size == 0 || vs2_size == 0) {
    return 0;
  }

  rbc_vspace direct_sum;
  rbc_vspace result;  
  int32_t inter_dim;
  int32_t trailing_zeros = 0;

  // Initialization
  rbc_vspace_init(&direct_sum, vs1_size + vs2_size);
  rbc_vspace_init(&result, vs1_size + vs2_size);

  rbc_vspace_direct_sum(direct_sum, vs1, vs2, vs1_size, vs2_size);
  rbc_vspace_set(result, vs1, vs1_size);

  // Perform Gauss on direct_sum and repeat operations on result
  int dimension = rbc_vec_gauss(direct_sum, vs1_size + vs2_size, &result, 1);

  inter_dim = vs1_size + vs2_size - dimension;
  rbc_vspace_set_zero(o, max(vs1_size, vs2_size));
  
  for(uint32_t i =  vs1_size + vs2_size - 1 ; i >= vs1_size + vs2_size - inter_dim ; i--) {
    if(rbc_elt_is_zero(result[i])) trailing_zeros++;
  }

  inter_dim -= trailing_zeros;

  for(int32_t i = 0 ; i < inter_dim ; i++) {
    rbc_elt_set(o[i], result[dimension + i]);
  }

  rbc_vspace_clear(direct_sum);
  rbc_vspace_clear(result);

  return inter_dim;
}



/**
 * \fn uint32_t rbc_vspace_intersection(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size)
 * \brief This function computes the intersection of vs1 and vs2.
 *
 * \param[out] o Intersection of vs1 and vs2
 * \param[in] vs1 rbc_vspace
 * \param[in] vs2 rbc_vspace
 * \param[in] vs1_size Size of vs1
 * \param[in] vs2_size Size of vs2
 *
 * \return Dimension of o
 */
uint32_t rbc_vspace_intersection_vartime(rbc_vspace o, const rbc_vspace vs1, const rbc_vspace vs2, uint32_t vs1_size, uint32_t vs2_size) {
  if(vs1_size == 0 || vs2_size == 0) {
    return 0;
  }

  rbc_vspace direct_sum;
  rbc_vspace result;  
  int32_t inter_dim;
  int32_t trailing_zeros = 0;

  // Initialization
  rbc_vspace_init(&direct_sum, vs1_size + vs2_size);
  rbc_vspace_init(&result, vs1_size + vs2_size);

  rbc_vspace_direct_sum(direct_sum, vs1, vs2, vs1_size, vs2_size);
  rbc_vspace_set(result, vs1, vs1_size);

  // Perform Gauss on direct_sum and repeat operations on result
  int dimension = rbc_vec_gauss_vartime(direct_sum, vs1_size + vs2_size, &result, 1);

  inter_dim = vs1_size + vs2_size - dimension;
  rbc_vspace_set_zero(o, max(vs1_size, vs2_size));  

  for(uint32_t i =  vs1_size + vs2_size - 1 ; i >= vs1_size + vs2_size - inter_dim ; i--) {
    if(rbc_elt_is_zero(result[i])) trailing_zeros++;
  }

  inter_dim -= trailing_zeros;

  for(int32_t i = 0 ; i < inter_dim ; i++) {
    rbc_elt_set(o[i], result[dimension + i]);
  }

  rbc_vspace_clear(direct_sum);
  rbc_vspace_clear(result);

  return inter_dim;
}
