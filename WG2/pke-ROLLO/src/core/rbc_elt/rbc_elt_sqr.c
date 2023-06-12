
/**
 * \fn void rbc_elt_sqr(rbc_elt o, const rbc_elt e)
 * \brief This function computes the square of a finite field element.
 *
 * \param[out] o rbc_elt equal to \f$ e^{2} \f$
 * \param[in] e rbc_elt
 */
void rbc_elt_sqr(rbc_elt o, const rbc_elt e) {
  if(rbc_init_field == 0) {
    printf("Call to rbc_elt_sqr with uninitialized field\n");
    exit(1);
  }

  rbc_elt_ur tmp;
  rbc_elt_ur_sqr(tmp, e);
  rbc_elt_reduce(o, tmp);
}
