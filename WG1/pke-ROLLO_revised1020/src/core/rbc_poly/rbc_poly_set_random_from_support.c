


/**
 * \fn void rbc_poly_set_random_from_support(random_source* ctx, rbc_poly o1, rbc_poly o2, int32_t degree, const rbc_vspace support, uint32_t support_size)
 * \brief This function sets a pair of polynomials with random values using NIST seedexpander. The support (o1 || o2) is the one given in input.
 *
 * \param[out] o1 Polynomial
 * \param[out] o2 Polynomial
 * \param[in] degree Degree of <b>o1</b> and <b>o2</b>
 * \param[in] support Support
 * \param[in] support_size Size of the support
 */
void rbc_poly_set_random_from_support(random_source* ctx, rbc_poly o, int32_t degree, const rbc_vspace support, uint32_t support_size, uint8_t copy_flag) {
  rbc_vec_set_random_from_support(ctx, o->v, degree + 1, support, support_size, copy_flag);
}
