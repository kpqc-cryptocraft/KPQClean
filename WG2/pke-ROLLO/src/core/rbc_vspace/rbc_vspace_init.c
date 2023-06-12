


/**
 * \fn void rbc_vspace_init(rbc_vspace* vs, uint32_t size)
 * \brief This function allocates the memory for a rbc_vspace.
 *
 * \param[out] vs Pointer to the allocated rbc_vspace
 * \param[in] size Size of the rbc_vspace
 */
void rbc_vspace_init(rbc_vspace* vs, uint32_t size) {
  rbc_vec_init(vs, size);
}
