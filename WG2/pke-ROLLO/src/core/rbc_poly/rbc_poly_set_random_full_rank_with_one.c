


/**
 * \fn void rbc_poly_set_random_full_rank_with_one(random_source* ctx, rbc_poly o, int32_t degree) {
 * \brief This function sets a polynomial with random values using the NIST seed expander. The polynomial returned by this function has full rank and contains one.
 *
 * \param[out] ctx random source
 * \param[out] o Polynomial
 * \param[in] degree Degree of the polynomial
 */
void rbc_poly_set_random_full_rank_with_one(random_source* ctx, rbc_poly o, int32_t degree) {
  rbc_vec_set_random_full_rank_with_one(ctx, o->v, degree + 1);
}
