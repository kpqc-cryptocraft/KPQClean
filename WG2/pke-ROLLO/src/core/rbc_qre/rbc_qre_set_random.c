


/**
 * \fn void rbc_qre_set_random(random_source* ctx, rbc_qre o)
 * \brief This function sets a rbc_qre with random values using NIST seed expander.
 *
 * \param[out] ctx random source
 * \param[out] o rbc_qre
 */
void rbc_qre_set_random(random_source* ctx, rbc_qre o) {
  rbc_poly_set_random(ctx, o, RBC_QRE_DEG);
}
