


/**
 * \fn uint32_t rbc_vec_gauss(rbc_vec v, uint32_t size, rbc_vec *other_matrices, uint32_t nMatrices)
 * \brief This function transform a vector of finite field elements to its row echelon form and returns its rank.
 *
 * Replicates linear operations on the nMatrices matrices indexed by other_matrices.
 *
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 * \param[in] other_matrices Pointer to other matrices to replicate the operations on
 * \param[in] nMatrices Number of other matrices
 *
 * \return Rank of the vector <b>v</b>
 */
uint32_t rbc_vec_gauss(rbc_vec v, uint32_t size, rbc_vec *other_matrices, uint32_t nMatrices) {
  if(size == 0) return 0;

  uint32_t dimension = 0;
  rbc_elt sum, tmp;
  rbc_elt zero;
  rbc_elt_set_zero(zero);
  rbc_elt dummy;

  //For each column
  for(size_t i = 0 ; i < RBC_FIELD_M ; i++) {
    int64_t mask;
    size_t column = RBC_FIELD_M - i - 1;
    uint32_t row = min(dimension, size-1);

    //For each line below
    for(size_t j=0 ; j < size ; j++) {
        rbc_elt_add(sum, v[row], v[j]);
        mask = rbc_elt_get_coefficient_vartime(sum, column);
        rbc_elt_set_mask1(tmp, v[j], zero, mask);
        if(j > row) {
          rbc_elt_add(v[row], v[row], tmp);
          for(size_t k=0 ; k<nMatrices ; k++) {
            rbc_elt_set_mask1(tmp, other_matrices[k][j], zero, mask);
            rbc_elt_add(other_matrices[k][row], other_matrices[k][row], tmp);
          }
        }
        else {
          rbc_elt_add(dummy, v[row], tmp);
          for(size_t k=0 ; k<nMatrices ; k++) {
            rbc_elt_add(dummy, other_matrices[k][row], tmp);
          }
        }
    }

    //For each other line
    for(size_t j=0 ; j < size ; j++) {
      if(j == row) continue;
      mask = rbc_elt_get_coefficient_vartime(v[j], column);
      rbc_elt_set_mask1(tmp, v[row], zero, mask);
      if(dimension < size) {
        rbc_elt_add(v[j], v[j], tmp);
        for(size_t k=0 ; k<nMatrices ; k++) {
          rbc_elt_set_mask1(tmp, other_matrices[k][row], zero, mask);
          rbc_elt_add(other_matrices[k][j], other_matrices[k][j], tmp);
        }
      }
      else {
        rbc_elt_add(dummy, v[j], tmp);
        for(size_t k=0 ; k<nMatrices ; k++) {
          rbc_elt_add(dummy, other_matrices[k][j], tmp);
        }
      }
    }

    dimension += rbc_elt_get_coefficient_vartime(v[row], column);
  }

  return min(dimension, size);
}



/**
 * \fn uint32_t rbc_vec_gauss_vartime(rbc_vec v, uint32_t size, rbc_vec *other_matrices, uint32_t nMatrices)
 * \brief This function transform a vector of finite field elements to its row echelon form and returns its rank.
 *
 * Replicates linear operations on the nMatrices matrices indexed by other_matrices.
 *
 * \param[in] v rbc_vec
 * \param[in] size Size of the vector
 * \param[in] other_matrices Pointer to other matrices to replicate the operations on
 * \param[in] nMatrices Number of other matrices
 *
 * \return Rank of the vector <b>v</b>
 */
uint32_t rbc_vec_gauss_vartime(rbc_vec v, uint32_t size, rbc_vec *other_matrices, uint32_t nMatrices) {
  if(size == 0) return 0;

  int32_t degree;
  int32_t target_deg, current_deg;
  size_t pivot;

  target_deg = RBC_FIELD_M;

  for(size_t i = 0 ; i < size ; i++) {
    // Take the pivot as the greatest coordinate of v
    pivot = i;
    current_deg = rbc_elt_get_degree(v[pivot]);

    for(size_t j = pivot + 1 ; j < size ; j++) {
      if(current_deg == target_deg) {
        break;
      }

      int j_deg = rbc_elt_get_degree(v[j]);
      if(j_deg > current_deg) {
        pivot = j;
        current_deg = j_deg;
      }
    }

    // Stop computation if the pivot is equal to zero
    if(current_deg == -1) {
      return pivot;
    }

    target_deg = current_deg - 1;

    // Swap current coordinate with pivot
    rbc_elt tmp;
    rbc_elt_set(tmp, v[i]);
    rbc_elt_set(v[i], v[pivot]);
    rbc_elt_set(v[pivot], tmp);

    for(size_t k=0 ; k<nMatrices ; k++) {
      rbc_elt_set(tmp, other_matrices[k][i]);
      rbc_elt_set(other_matrices[k][i], other_matrices[k][pivot]);
      rbc_elt_set(other_matrices[k][pivot], tmp);
    }
    degree = rbc_elt_get_degree(v[i]);

    // Add matrix columns whenever necessary
    for(size_t j = i + 1; j < size; j++) {
      if(!(rbc_elt_is_zero(v[j]) || rbc_elt_get_degree(v[j]) < degree)) {
        rbc_elt_add(v[j], v[j], v[i]);
        for(size_t k=0 ; k<nMatrices ; k++) {
          rbc_elt_add(other_matrices[k][j], other_matrices[k][j], other_matrices[k][i]);
        }
      }
    }
  }

  return size;
}
