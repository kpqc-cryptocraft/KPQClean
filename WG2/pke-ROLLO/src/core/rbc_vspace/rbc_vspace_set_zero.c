


/**
 * \fn void rbc_vspace_set_zero(rbc_vspace o, uint32_t size)
 * \brief This function sets a rbc_vspace to zero.
 *
 * \param[out] o rbc_vspace
 * \param[in] size Size of the rbc_vspace
 */
void rbc_vspace_set_zero(rbc_vspace o, uint32_t size) {
  rbc_vec_set_zero(o, size);
}
