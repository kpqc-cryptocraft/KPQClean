


/**
 * \fn void rbc_vspace_clear(rbc_vspace vs)
 * \brief This functions clears the memory allocated to a rbc_vspace.
 *
 * \param[in] v rbc_vspace
 * \param[in] size Size of the rbc_vspace
 */
void rbc_vspace_clear(rbc_vspace vs) {
  rbc_vec_clear(vs);
}
