
/**
 * \fn void rbc_elt_mul(rbc_elt o, const rbc_elt e1, const rbc_elt e2)
 * \brief This function multiplies two finite field elements.
 *
 * \param[out] o Product of <b>e1</b> and <b>e2</b>
 * \param[in] e1 rbc_elt
 * \param[in] e2 rbc_elt
 */
void rbc_elt_mul(rbc_elt o, const rbc_elt e1, const rbc_elt e2) {
  rbc_elt_ur tmp;
  rbc_elt_ur_set_zero(tmp);
  rbc_elt_ur_mul(tmp, e1, e2);
  rbc_elt_reduce(o, tmp);
}

