


/**
 * \fn void rbc_poly_set_random(random_source* ctx, rbc_poly o, int32_t degree)
 * \brief This function sets a polynomial with random values using NIST seed expander.
 *
 * \param[out] ctx random_source
 * \param[out] o Polynomial
 * \param[in] degree Degree of the polynomial
 */
void rbc_poly_set_random(random_source* ctx, rbc_poly o, int32_t degree) {
  rbc_vec_set_random(ctx, o->v, degree + 1);
}
