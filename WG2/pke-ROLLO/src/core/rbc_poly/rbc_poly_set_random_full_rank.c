


/**
 * \fn void rbc_poly_set_random_full_rank(random_source* ctx, rbc_poly o, int32_t degree)
 * \brief This function sets a polynomial with random values using the NIST seed expander. The polynomial returned by this function has full rank.
 *
 * \param[out] o Polynomial
 * \param[in] degree Degree of the polynomial
 * \param[in] ctx random_source
 */
void rbc_poly_set_random_full_rank(random_source* ctx, rbc_poly o, int32_t degree) {
  rbc_vec_set_random_full_rank(ctx, o->v, degree + 1);
}
