


/**
 * \fn void rbc_vec_clear(rbc_vec v)
 * \brief This function clears a rbc_vec element.
 *
 * \param[out] v rbc_vec
 */
void rbc_vec_clear(rbc_vec v) {
  free(v);
}
