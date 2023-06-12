/** 
 * \file kem.c
 * \brief Implementation of api.h
 */

#include "rbc_qre.h"
#include "rbc_vec.h"
#include "sha2.h"
#include "fips202.h"
#include "biiy_parameters.h"
#include "string.h"
#include "rbc_lrpc.h"
#include "biiy_types.h"
#include "biiy_parsing.h"

//additional part
//#include <algorithm>
//#include <vector>
//#include <random>
//Not used in C
//Note: PK size should be even(sub-sk, not necessarily)


int biiy_keygen(uint8_t* pk, uint8_t* sk) {

  //Pre-processing: sk-generate by sub-code rule
  
  uint8_t pkinv_st[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  uint8_t pk_POPI_st[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  
  uint8_t pk_st[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  
  uint8_t sk_seed[SEEDEXPANDER_SEED_BYTES] = {0};
  
  uint8_t sk_PO[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  uint8_t sk_PI[BIIY_PUBLIC_KEY_BYTES/6] = {0}; 
  
  //uint8_t sk_POinv[BIIY_PUBLIC_KEY_BYTES/2] = {0}; 
  //uint8_t sk_PIinv[BIIY_PUBLIC_KEY_BYTES/4] = {0}; 


  random_source prng;
  random_init(&prng, RANDOM_SOURCE_PRNG);
  
  biiy_sk_generate(sk_seed, pkinv_st, sk_PI, pk_POPI_st, prng);
  
  rbc_qre P;
  rbc_qre PO;
  //rbc_qre POinv;
  
  rbc_qre pk_unit_qre1;
  rbc_qre pk_POPI;
  
  biiy_publicKey pkTmp;
  
  //rbc_qre invX1, invX2; //necessary
  rbc_field_init();
  rbc_qre_init_modulus(3*BIIY_PARAM_N);
  
  //Possible reason: Init 2*N parameter error Two different qre_ use in error...
  //TODO_Build different core

//generate sk_message, TODO: add polynomial part


  uint32_t E_Byte = BIIY_PARAM_M / 8;
  uint32_t E_Byte2 = BIIY_PARAM_M % 8;

  //rbc_qre_init(&P);
  rbc_qre_init(&pk_unit_qre1);
  
  rbc_qre_init(&PO);
  //rbc_qre_init(&POinv);
  

  rbc_qre_init(&pk_POPI);
  rbc_qre_init(&(pkTmp.h));  
 
  rbc_qre_from_string(pk_unit_qre1, pkinv_st); 	
//  rbc_qre_from_string(PO, sk_PO);
  rbc_qre_set_arb_random(&prng, PO, 3*BIIY_PARAM_N);
  
  
 // rbc_qre_inv(POinv, PO);
  //rbc_qre_to_string(sk_POinv, POinv);
 
  
  rbc_qre_mul(pk_POPI, pk_POPI, PO);
  rbc_qre_to_string(pk_POPI_st, pk_POPI);
  rbc_qre_mul(pkTmp.h, pk_unit_qre1, PO);
  biiy_public_key_to_string(pk_st, &pkTmp);
  rbc_qre_to_string(sk_PO, PO);
//Public key generation (Pinv, POPI)
for(size_t i = 0 ; i < BIIY_PUBLIC_KEY_BYTES/2 ; ++i) 
  {
    pk[i]=pk_st[i];
    pk[i+BIIY_PUBLIC_KEY_BYTES/2]=pk_POPI_st[i];
    //Secret key seed addition
  }
  
  
  //secret key generation ( seed, PO, PI)
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk[i]=sk_seed[i];
  }
  
  for(size_t i = 0 ; i < BIIY_PUBLIC_KEY_BYTES/2 ; ++i) 
  {
    sk[i+SEEDEXPANDER_SEED_BYTES]=sk_PO[i];
  }
  
  for(size_t i = 0 ; i < BIIY_PUBLIC_KEY_BYTES/6 ; ++i) 
  {
    sk[i+SEEDEXPANDER_SEED_BYTES+BIIY_PUBLIC_KEY_BYTES/2]=sk_PI[i];
    //printf("%02x",sk_PI[i]);
  }
  

  rbc_qre_clear(pkTmp.h);
  //rbc_qre_clear(P);
  rbc_qre_clear(PO);
  rbc_qre_clear(pk_POPI);
  rbc_qre_clear(pk_unit_qre1);
 // rbc_qre_clear(pk_unit_qreP);
  
  rbc_qre_clear_modulus();
  
  return 0;
  
}


// Outcome: Secret key only for message, Public key for inv(^-1)
int biiy_sk_generate(uint8_t* sk_m, uint8_t* pk_xyinv1,  uint8_t* PI_st, uint8_t* sk_2PI, random_source prng) {
    
  biiy_secretKey skTmp1;
  biiy_publicKey pkTmp1;	
  
  rbc_qre invX1;
  
  //rbc_qre PO;  // (n/b-1)-degree
  rbc_qre PI;  // (b-1)-dgree
  //rbc_qre PIinv;  // (b-1)-dgree
  
  rbc_field_init();
  rbc_qre_init_modulus(BIIY_PARAM_N);
  
  uint32_t E_Byte = BIIY_PARAM_M / 8;
  uint32_t E_Byte2 = BIIY_PARAM_M % 8;


  uint8_t sk_seed1[SEEDEXPANDER_SEED_BYTES];
  uint8_t pk_st1[BIIY_PUBLIC_KEY_BYTES/6] = {0};
  
  random_get_bytes(&prng, sk_seed1, SEEDEXPANDER_SEED_BYTES);
  

  rbc_qre_init(&PI);
  //rbc_qre_init(&PIinv);
  rbc_qre_init(&(pkTmp1.h));
  
  rbc_qre_set_arb_random(&prng, PI, 3);
  //rbc_qre_inv(PIinv, PI);
  //rbc_qre_to_string(sk_PIinv, PIinv);

  biiy_secret_key_from_string(&skTmp1, sk_seed1);
  
   for(size_t i = 0 ; i <  SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk_m[i]=sk_seed1[i];
  }
  
  rbc_qre_init(&invX1);
  rbc_qre_inv(invX1, skTmp1.x);


  rbc_qre_init(&(pkTmp1.h));
  rbc_qre_mul(pkTmp1.h, invX1, skTmp1.y);
  
  //Inner permutation added
  rbc_qre_mul(pkTmp1.h, pkTmp1.h, PI);
  biiy_public_key_to_string(pk_st1, &pkTmp1);

  rbc_qre_to_string(PI_st, PI);


  for(size_t i = 0 ; i < (E_Byte)*(BIIY_PARAM_N) ; ++i) 
  {
    pk_xyinv1[i]=pk_st1[i];
    sk_2PI[i]=PI_st[i];
  }
  //edge processing in KEM
  for(size_t i = 2*(E_Byte)*(BIIY_PARAM_N); i < 2*(E_Byte)*(BIIY_PARAM_N+E_Byte2) ; ++i) 
  {
    pk_xyinv1[i+2*(E_Byte)*(BIIY_PARAM_N)]=pk_st1[i];
    sk_2PI[i+2*(E_Byte)*(BIIY_PARAM_N)]=PI_st[i];
  }
 
  // C: Should add the permutation operation in here

  rbc_vspace_clear(skTmp1.F);
  
  rbc_qre_clear(skTmp1.x);
  rbc_qre_clear(PI);
  
  rbc_qre_clear(skTmp1.y);

  rbc_qre_clear(pkTmp1.h);

  rbc_qre_clear_modulus();
  
  
  return 0;
}



int biiy_encaps(uint8_t* ct, uint8_t* ss, uint8_t* pk) {

  //uint8_t* pk1, pk2;
  //uint8_t* ct1, ct2;
  
  // Added from the modification
  // TODO: connection with the cipertext
  //pk1 = pk;
  //pk2 = pk+BIIY_PUBLIC_KEY_BYTES/2;
  
  //ct1 = ct;
  //ct2 = ct+BIIY_CIPHERTEXT_BYTES/2;
  //E -> n/b-subpspace,(generate with the different subspace)
  
  uint8_t pk_st[BIIY_PUBLIC_KEY_BYTES] = {0};
  uint8_t pk_POPI_st[BIIY_PUBLIC_KEY_BYTES] = {0};
    
  uint32_t E_Byte = BIIY_PARAM_M / 8;
  uint32_t E_Byte2 = BIIY_PARAM_M % 8;
  
  for(size_t i=0; i < BIIY_PUBLIC_KEY_BYTES/2 ; i++)
  {
  	pk_st[i] = pk[i];
  	pk_POPI_st[i] = pk[i+BIIY_PUBLIC_KEY_BYTES/2];
  }					
  
  biiy_encaps_errgen(pk_st, pk_POPI_st, ct, ss);
  
  return 0;
}

//newly defined ftn.: not used in the external main
//From now, pk1, pk2 gen. are generated from the P^{-1}pk,
int biiy_encaps_errgen(uint8_t* pk1_st, uint8_t* pk_POPI_st, uint8_t* ct1_st, uint8_t* ss)
{

  uint32_t E_Byte = BIIY_PARAM_M / 8;
  uint32_t E_Byte2 = BIIY_PARAM_M % 8;
  
  rbc_vspace E1;
  rbc_qre E11, E12;

  rbc_qre pk1Tmp;
  rbc_qre pk_POPI;
  
  biiy_ciphertext ct1Tmp; //Shared as input and output

  rbc_field_init();
  rbc_qre_init_modulus(3*BIIY_PARAM_N);
  
  rbc_vspace_init(&E1, BIIY_PARAM_R);

  random_source prng;
  random_init(&prng, RANDOM_SOURCE_PRNG);

  //Generate the support
  rbc_vspace_set_random_full_rank(&prng, E1, BIIY_PARAM_R);
  
  rbc_qre_init(&E11);
  rbc_qre_init(&E12);
  
  rbc_qre_init(&pk_POPI);
  rbc_qre_init(&pk1Tmp);

  rbc_qre_from_string(pk_POPI, pk_POPI_st);
  rbc_qre_from_string(pk1Tmp, pk1_st);
   
  //Generate random error vectors  
  rbc_qre_set_arb_random_pair_from_support(&prng, E11, E12, E1, 1);
  //rbc_qre_print(E11);
  
  //E11->v
  //rbc_elt_set_coefficient_vartime(rbc_elt o, uint32_t index, uint64_t bit)

  rbc_qre_init(&(ct1Tmp.syndrom));

  rbc_qre_mul(ct1Tmp.syndrom, E11, pk1Tmp);
  

  rbc_qre_mul(pk_POPI, E12, pk_POPI);

  rbc_qre_add(ct1Tmp.syndrom, ct1Tmp.syndrom, pk_POPI);

  //rbc_qre_print(ct1Tmp.syndrom);
  biiy_biiy_ciphertext_to_string(ct1_st, &ct1Tmp);
  
  uint8_t support_st1[BIIY_RBC_VEC_R_BYTES]={0};  
    
  rbc_vec_echelonize(E1, BIIY_PARAM_R);

  rbc_vec_to_string(support_st1, E1, BIIY_PARAM_R);


  sha512(ss, support_st1, BIIY_RBC_VEC_R_BYTES);
  
  rbc_vspace_clear(E1);
  rbc_qre_clear(pk1Tmp);

  rbc_qre_clear(E11);
  rbc_qre_clear(E12);

  rbc_qre_clear_modulus();
  
  return 0;
}


int biiy_decaps(uint8_t* ss, const uint8_t* ct, uint8_t* sk) {

  unsigned char ctRes[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  unsigned char skseed_st[SEEDEXPANDER_SEED_BYTES] = {0};
  unsigned char PO_st[BIIY_PUBLIC_KEY_BYTES/2] = {0};
  unsigned char PI_st[BIIY_PUBLIC_KEY_BYTES/6] = {0};
  
  //Secret key generation
  for(size_t i = 0 ; i <  SEEDEXPANDER_SEED_BYTES ; i++)
  {
  	skseed_st[i]=sk[i];
  }
  
  for(size_t i = 0 ; i < BIIY_PUBLIC_KEY_BYTES/2 ; i++)
  {
  	PO_st[i]=sk[i+SEEDEXPANDER_SEED_BYTES];
  }

  for(size_t i = 0 ; i < BIIY_PUBLIC_KEY_BYTES/6 ; i++)
  {
  	PI_st[i]=sk[i+SEEDEXPANDER_SEED_BYTES+BIIY_PUBLIC_KEY_BYTES/2];
  }
  
  biiy_decaps_multisym(ct, ctRes, PO_st);
  
  biiy_secretKey skTmp1;
  rbc_qre ctRed;
  rbc_vspace E1;
  rbc_qre xc1;
  
  rbc_qre PI;
  rbc_qre PIinv;

  rbc_field_init();
  rbc_qre_init_modulus(BIIY_PARAM_N);
   
  rbc_qre_init(&ctRed);
  rbc_qre_mod_sparse_from_string(ctRed, ctRes);
 
  biiy_secret_key_from_string(&skTmp1, skseed_st);
  
  rbc_qre_init(&xc1);
  rbc_qre_init(&PI);
  rbc_qre_init(&PIinv);
  
  rbc_qre_from_string(PI,PI_st); 

  rbc_qre_inv(PIinv, PI);

 
  rbc_qre_mul(ctRed, ctRed, PIinv);
  rbc_qre_mul(xc1, skTmp1.x, ctRed);
  //for(int i = 0 ; i < 40 ; i++){ printf("%02x",xc2->v[i]);}
  uint32_t dimE1 = 0;
  //Problem: This region below

  
  rbc_vspace_init(&E1, BIIY_PARAM_N);
  
  dimE1 = rbc_lrpc_RSR(E1, BIIY_PARAM_R, skTmp1.F, BIIY_PARAM_D, xc1->v, BIIY_PARAM_N);
  
  printf("%d\n",dimE1);
  
  uint8_t support_st1[BIIY_RBC_VEC_R_BYTES]={0};
  rbc_vec_to_string(support_st1, E1, BIIY_PARAM_R);
  sha512(ss, support_st1, BIIY_RBC_VEC_R_BYTES);
  rbc_vspace_clear(E1);
  
  
 // TODO: Recover E(from F1). modify it if possible


  rbc_qre_clear(xc1);
  rbc_vspace_clear(skTmp1.F);
   
  rbc_qre_clear(skTmp1.x);
  rbc_qre_clear(skTmp1.y);
  
  rbc_qre_clear(PIinv);
  rbc_qre_clear(PI);
  
  rbc_qre_clear(ctRed);

  rbc_qre_clear_modulus();

  int result;

  result = dimE1 != BIIY_PARAM_R;
  
  return 0;
  //return result;
}

int biiy_decaps_multisym(uint8_t* ct, uint8_t* ctRes, uint8_t* PO_st)
{
//TODO: Carisis, 220805
   rbc_qre PO;
   rbc_qre POinv;
   
   biiy_ciphertext ctTmp;
  
  
  rbc_field_init();
  rbc_qre_init_modulus(3*BIIY_PARAM_N);

  rbc_qre_init(&PO);
  rbc_qre_init(&POinv);
  rbc_qre_from_string(PO, PO_st);
  rbc_qre_inv(POinv, PO);
  
  rbc_qre_init(&(ctTmp.syndrom));

  biiy_biiy_ciphertext_from_string(&ctTmp, ct);

  rbc_qre_mul(ctTmp.syndrom, ctTmp.syndrom, POinv);

  //rbc_qre_print(ctTmp.syndrom);
  biiy_biiy_ciphertext_to_string(ctRes, &ctTmp);
  
  rbc_qre_clear(ctTmp.syndrom);
  rbc_qre_clear(PO);
  rbc_qre_clear(POinv);
  
  rbc_qre_clear_modulus();
   
   return 0;
  
}
//Convert to n-length rbc poly. from b*n string
void rbc_qre_mod_sparse_from_string(rbc_qre o, const uint8_t* str) {
  
  rbc_poly_sparse modulus;
  modulus = rbc_qre_get_modulus(BIIY_PARAM_N);
  int32_t modulus_degree = modulus->coeffs[modulus->coeffs_nb - 1];
  
  rbc_poly unreduced;
  rbc_poly_init(&unreduced, 3 * modulus_degree - 2);
  rbc_poly_set_zero(unreduced, 3 * modulus_degree - 2);

  rbc_poly_from_string(unreduced, str);

  // Modular reduction
  for(int32_t i = unreduced->max_degree - modulus_degree ; i >= 0 ; i--) {
    for(size_t j = 0 ; j < 1 * modulus->coeffs_nb - 1 ; j++) {
      rbc_elt_add(unreduced->v[i + modulus->coeffs[j]],
      unreduced->v[i + modulus->coeffs[j]], unreduced->v[i + modulus_degree]);
    }
    rbc_elt_set_zero(unreduced->v[i + modulus_degree]);
  }

  rbc_poly_set(o, unreduced);
  rbc_poly_clear(unreduced);
}

void rbc_qre_set_arb_random(random_source* ctx, rbc_qre o, int32_t deg) {
  //rbc_poly_set_random(ctx, o, deg);
  //rbc_vec_set_random(ctx, o->v, degree + 1);
  
  uint32_t bytes = (BIIY_PARAM_M % 8 == 0) ? BIIY_PARAM_M / 8 : BIIY_PARAM_M / 8 + 1;
  uint8_t mask = (1 << BIIY_PARAM_M % 8) - 1;
  uint8_t random[deg * bytes];

  rbc_vec_set_zero(o->v, BIIY_PARAM_N);
  random_get_bytes(ctx, random, deg * bytes);

  for(size_t i = 0 ; i < deg ; ++i) {
    random[(i + 1) * bytes - 1] &= mask;
    memcpy(o->v[i], random + i * bytes, bytes);
  }
  
  
}


void rbc_qre_set_arb_random_pair_from_support(random_source* ctx, rbc_qre o1, rbc_qre o2, const rbc_vec support, uint8_t copy_flag) {
  //Predefined from
   uint32_t size =  BIIY_PARAM_N-2;
   uint32_t support_size = BIIY_PARAM_R;
   
  if (copy_flag) {
    uint32_t random1_size = 4 * support_size;
    uint32_t random2_size = (support_size * (2 * size - support_size)) / 8 + 2;
    uint8_t random1[random1_size];
    uint8_t random2[random2_size];

    // Copy the support vector in support_size random positions of o
    random_get_bytes(ctx, random1, random1_size);

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t position;

    while(i != support_size) {
      position = random1[j];

      // Check that the position is not already taken
      if(position < size * (256 / size)) {
        if(random1[j + 1] & 0x1) {
          if(rbc_elt_is_zero(o1->v[position % size])) {
            rbc_elt_set(o1->v[position % size], support[i]);
            i++;
          }
        } 
        else {
          if(rbc_elt_is_zero(o2->v[position % size])) {
            rbc_elt_set(o2->v[position % size], support[i]);
            i++;
          }
        }
      }

      // Get more randomness if necessary
      j = j + 2;
      if(j % random1_size == 0 && i != support_size) {
        random_get_bytes(ctx, random1, random1_size);
        j = 0;
      }
    }

    // Set all the remaining coordinates with random linear combinations of the support coordinates
    random_get_bytes(ctx, random2, random2_size);

    uint32_t k = 0;
    uint32_t l = 0;

    for(i = 0 ; i < size ; ++i) {
      if(rbc_elt_is_zero(o1->v[i])) {
        for(j = 0 ; j < support_size ; ++j) {
          if(random2[k] & 0x1) {
            rbc_elt_add(o1->v[i], support[j], o1->v[i]);
          }

          random2[k] = random2[k] >> 1;
          l++;
          if(l == 8) {
            l = 0;
            k++;
          }
        }
      }
    }

    k++;

    for(i = 0 ; i < size ; ++i) {
      if(rbc_elt_is_zero(o2->v[i])) {
        for(j = 0 ; j < support_size ; ++j) {
          if(random2[k] & 0x1) {
            rbc_elt_add(o2->v[i], support[j], o2->v[i]);
          }

          random2[k] = random2[k] >> 1;
          l++;
          if(l == 8) {
            l = 0;
            k++;
          }
        }
      }
    }
  }
  // Not defined from the original one
  else {
    //Set all the coordinates to random linear combinations of the support
    uint32_t random_size = 2 * support_size * size / 8 + 1;
    uint8_t random[random_size];
    random_get_bytes(ctx, random, random_size);

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t l = 0;

    for(i = 0 ; i < size ; ++i) {
      for(j = 0 ; j < support_size ; ++j) {
        if(random[k] & 0x1) {
          rbc_elt_add(o1->v[i], support[j], o1->v[i]);
        }

        random[k] = random[k] >> 1;
        l++;
        if(l == 8) {
          l = 0;
          k++;
        }
      }
    }

    for(i = 0 ; i < size ; ++i) {
      for(j = 0 ; j < support_size ; ++j) {
        if(random[k] & 0x1) {
          rbc_elt_add(o2->v[i], support[j], o2->v[i]);
        }

        random[k] = random[k] >> 1;
        l++;
        if(l == 8) {
          l = 0;
          k++;
        }
      }
    }
  }
}

