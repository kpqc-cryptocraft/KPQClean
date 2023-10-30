


/**
 * \fn void rbc_qre_add(rbc_qre o, const rbc_qre p1, const rbc_qre p2)
 * \brief This function adds two rbc_qre.
 *
 * \param[out] o Sum of <b>p1</b> and <b>p2</b>
 * \param[in] p1 rbc_qre
 * \param[in] p2 rbc_qre
 */
void rbc_qre_add(rbc_qre o, const rbc_qre p1, const rbc_qre p2) {
  rbc_poly_add(o, p1, p2);
}
