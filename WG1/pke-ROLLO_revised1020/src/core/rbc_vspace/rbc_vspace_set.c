


/**
 * \fn void rbc_vspace_set(rbc_vspace o, const rbc_vspace vs, uint32_t size)
 * \brief This function copies a rbc_vspace to another one.
 *
 * \param[out] o rbc_vspace
 * \param[in] vs rbc_vspace
 * \param[in] size Size of the rbc_vspaces
 */
void rbc_vspace_set(rbc_vspace o, const rbc_vspace vs, uint32_t size) {
  rbc_vec_set(o, vs, size);
}
