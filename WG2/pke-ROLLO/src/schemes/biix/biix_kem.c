/** 
 * \file kem.c
 * \brief Implementation of api.h
 */

#include "rbc_qre.h"
#include "rbc_vec.h"
#include "sha2.h"
#include "fips202.h"
#include "biix_parameters.h"
#include "string.h"
#include "rbc_lrpc.h"
#include "biix_types.h"
#include "biix_parsing.h"


int biix_keygen(uint8_t* pk, uint8_t* sk) {


  
  uint8_t pkinv_st[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  uint8_t pk_POPI_st[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  uint8_t p_st[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  
  uint8_t pk_st[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  
  uint8_t sk_seed[SEEDEXPANDER_SEED_BYTES] = {0};
  uint8_t sk_PO_seed[SEEDEXPANDER_SEED_BYTES] = {0};
  uint8_t sk_PI_seed[SEEDEXPANDER_SEED_BYTES] = {0}; 


  
  random_source prng;
  random_init(&prng, RANDOM_SOURCE_PRNG);
  
  random_get_bytes(&prng, sk_PO_seed, SEEDEXPANDER_SEED_BYTES);
  random_get_bytes(&prng, sk_PI_seed, SEEDEXPANDER_SEED_BYTES);
  random_get_bytes(&prng, sk_seed, SEEDEXPANDER_SEED_BYTES);
  
  biix_sk_generate(sk_seed, pkinv_st, sk_PI_seed, pk_POPI_st);

  rbc_qre P;
  rbc_qre PO;
  rbc_qre PN;
  
  rbc_qre pk_unit_qre1;
  rbc_qre pk_POPI;
  
  biix_publicKey pkTmp;
  
  rbc_field_init();
  rbc_qre_init_modulus(2*BIIX_PARAM_N);

  uint32_t E_Byte = BIIX_PARAM_M / 8;
  uint32_t E_Byte2 = BIIX_PARAM_M % 8;


  //P Input SL: 128,192, 256
  if(BIIX_PARAM_N == 37){ p_st[E_Byte*37] = 1; p_st[E_Byte*22] = 1;p_st[E_Byte*14] = 1; p_st[E_Byte*2] = 1; p_st[E_Byte*0] = 1;}
  if(BIIX_PARAM_N == 43){ p_st[E_Byte*43] = 1; p_st[E_Byte*27] = 1; p_st[E_Byte*22] = 1; p_st[E_Byte*5] = 1; p_st[E_Byte*0] = 1;}
  if(BIIX_PARAM_N == 53){ p_st[E_Byte*53] = 1; p_st[E_Byte*50] = 1;p_st[E_Byte*41] = 1; p_st[E_Byte*20] = 1; p_st[E_Byte*0] = 1;}

  rbc_qre_init(&PN);
  rbc_qre_init(&pk_unit_qre1);
  rbc_qre_init(&PO);
  rbc_qre_init(&P);

  rbc_qre_from_string(P, p_st); 
  rbc_qre_init(&pk_POPI);
  rbc_qre_init(&(pkTmp.h));  
 
  rbc_qre_from_string(pk_unit_qre1, pkinv_st); 	
  rbc_qre_set_arb_random(PO, 2*BIIX_PARAM_N, sk_PO_seed);
  rbc_qre_set_arb_random(PN, 2*BIIX_PARAM_N, sk_PO_seed+32);
 
 
 
  rbc_qre_from_string(pk_POPI,pk_POPI_st);
  rbc_qre_mul(pk_POPI, pk_POPI, PO);
  rbc_qre_to_string(pk_POPI_st, pk_POPI);
  
  rbc_qre_mul(pkTmp.h, pk_unit_qre1, PO);
  rbc_qre_mul(PN, P, PN);
  rbc_qre_add(pkTmp.h, pkTmp.h, PN);
  
  biix_public_key_to_string(pk_st, &pkTmp);

for(size_t i = 0 ; i < BIIX_PUBLIC_KEY_BYTES/2 ; ++i) 
  {
    pk[i]=pk_st[i];
    pk[i+BIIX_PUBLIC_KEY_BYTES/2]=pk_POPI_st[i];
  }
  
  //secret key generation ( seed, PO, PI)
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk[i]=sk_seed[i];
  }
  
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk[i+SEEDEXPANDER_SEED_BYTES]=sk_PI_seed[i];
  }
  
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk[i+2*SEEDEXPANDER_SEED_BYTES] = sk_PO_seed[i];
  }
  

  rbc_qre_clear(pkTmp.h);
  rbc_qre_clear(PN);
  rbc_qre_clear(P);
  rbc_qre_clear(PO);
  rbc_qre_clear(pk_POPI);
  rbc_qre_clear(pk_unit_qre1);

  rbc_qre_clear_modulus();
  
  return 0;
  
}

// Outcome: Secret key only for message, Public key for inv(^-1)
int biix_sk_generate(uint8_t* sk_m, uint8_t* pk_xyinv1,  uint8_t* sk_PI_seed, uint8_t* sk_2PI) {
    
  biix_secretKey skTmp1;
  biix_publicKey pkTmp1;	
  uint8_t PI_st[BIIX_PUBLIC_KEY_BYTES/4] = {0};
  
  
  rbc_qre invX1;  
  rbc_qre PI;  // (b-1)-dgree
  
  rbc_field_init();
  rbc_qre_init_modulus(BIIX_PARAM_N);
  
  uint32_t E_Byte = BIIX_PARAM_M / 8;
  uint32_t E_Byte2 = BIIX_PARAM_M % 8;


  uint8_t sk_seed1[SEEDEXPANDER_SEED_BYTES];
  uint8_t pk_st1[BIIX_PUBLIC_KEY_BYTES/4] = {0};


  rbc_qre_init(&PI);
  rbc_qre_init(&(pkTmp1.h));
  
  rbc_qre_set_arb_random(PI, 2, sk_PI_seed);

  for(size_t i = 0 ; i <  SEEDEXPANDER_SEED_BYTES ; ++i) 
  {
    sk_seed1[i]=sk_m[i];
  }
 

  biix_secret_key_from_string(&skTmp1, sk_seed1);
  

  rbc_qre_init(&invX1);
  rbc_qre_inv(invX1, skTmp1.x);

  rbc_qre_init(&(pkTmp1.h));
  rbc_qre_mul(pkTmp1.h, invX1, skTmp1.y);
  
  rbc_qre_mul(pkTmp1.h, pkTmp1.h, PI);
  biix_public_key_to_string(pk_st1, &pkTmp1);

  rbc_qre_to_string(PI_st, PI);


  for(size_t i = 0 ; i < (E_Byte)*(BIIX_PARAM_N) ; ++i) 
  {
    pk_xyinv1[i]=pk_st1[i];
    sk_2PI[i]=PI_st[i];
  }
  //edge processing in KEM
  for(size_t i = (E_Byte)*(BIIX_PARAM_N); i < (E_Byte)*(BIIX_PARAM_N+E_Byte2) ; ++i) 
  {
    pk_xyinv1[i+(E_Byte)*(BIIX_PARAM_N)]=pk_st1[i];
    sk_2PI[i+(E_Byte)*(BIIX_PARAM_N)]=PI_st[i];
  }

  rbc_vspace_clear(skTmp1.F);
  
  rbc_qre_clear(skTmp1.x);
  rbc_qre_clear(PI);
  
  rbc_qre_clear(skTmp1.y);

  rbc_qre_clear(pkTmp1.h);

  rbc_qre_clear_modulus();
  
  
  return 0;
}



int biix_encaps(uint8_t* ct, uint8_t* ss, uint8_t* pk) {

  
  uint8_t pk_st[BIIX_PUBLIC_KEY_BYTES] = {0};
  uint8_t pk_POPI_st[BIIX_PUBLIC_KEY_BYTES] = {0};
    
  uint32_t E_Byte = BIIX_PARAM_M / 8;
  uint32_t E_Byte2 = BIIX_PARAM_M % 8;
  
  for(size_t i=0; i < BIIX_PUBLIC_KEY_BYTES/2 ; i++)
  {
  	pk_st[i] = pk[i];
  	pk_POPI_st[i] = pk[i+BIIX_PUBLIC_KEY_BYTES/2];
  }					
  
  biix_encaps_errgen(pk_st, pk_POPI_st, ct, ss);
  
  return 0;
}

int biix_encaps_errgen(uint8_t* pk1_st, uint8_t* pk_POPI_st, uint8_t* ct1_st, uint8_t* ss)
{

  uint32_t E_Byte = BIIX_PARAM_M / 8;
  uint32_t E_Byte2 = BIIX_PARAM_M % 8;
  
  rbc_vspace E1;
  rbc_qre E11, E12;

  rbc_qre pk1Tmp;
  rbc_qre pk_POPI;
  
  biix_ciphertext ct1Tmp;

  rbc_field_init();
  rbc_qre_init_modulus(2*BIIX_PARAM_N);
  
  rbc_vspace_init(&E1, BIIX_PARAM_R);

  random_source prng;
  random_init(&prng,  RANDOM_SOURCE_PRNG);

  //Generate the support
  rbc_vspace_set_random_full_rank(&prng, E1, BIIX_PARAM_R);
  
  rbc_qre_init(&E11);
  rbc_qre_init(&E12);
  
  rbc_qre_init(&pk_POPI);
  rbc_qre_init(&pk1Tmp);

  rbc_qre_from_string(pk_POPI, pk_POPI_st);
  rbc_qre_from_string(pk1Tmp, pk1_st);
   
  //Generate random error vectors  
  rbc_qre_set_arb_random_pair_from_support(&prng, E11, E12, E1, 1);


  rbc_qre_init(&(ct1Tmp.syndrom));

  rbc_qre_mul(ct1Tmp.syndrom, E11, pk1Tmp);
  

  rbc_qre_mul(pk_POPI, E12, pk_POPI);

  rbc_qre_add(ct1Tmp.syndrom, ct1Tmp.syndrom, pk_POPI);

  //rbc_qre_print(ct1Tmp.syndrom);
  biix_biix_ciphertext_to_string(ct1_st, &ct1Tmp);
  
  uint8_t support_st1[BIIX_RBC_VEC_R_BYTES]={0};  
  uint8_t ss_st1[BIIX_SHARED_SECRET_BYTES]={0};
    
  rbc_vec_echelonize(E1, BIIX_PARAM_R);

  rbc_vec_to_string(support_st1, E1, BIIX_PARAM_R);


  sha512(ss, support_st1, BIIX_RBC_VEC_R_BYTES);
  
  rbc_vspace_clear(E1);
  rbc_qre_clear(pk1Tmp);

  rbc_qre_clear(E11);
  rbc_qre_clear(E12);

  rbc_qre_clear_modulus();
  
  return 0;
}


int biix_decaps(uint8_t* ss, const uint8_t* ct, uint8_t* sk) {

  unsigned char ctRes[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  unsigned char skseed_st[SEEDEXPANDER_SEED_BYTES] = {0};
  unsigned char sk_PI_seed_st[SEEDEXPANDER_SEED_BYTES] = {0};
  unsigned char sk_PO_seed_st[SEEDEXPANDER_SEED_BYTES] = {0};
  
  unsigned char PO_st[BIIX_PUBLIC_KEY_BYTES/2] = {0};
  unsigned char PI_st[BIIX_PUBLIC_KEY_BYTES/4] = {0};
  
  random_source prng;
  random_init(&prng, RANDOM_SOURCE_SEEDEXP);
  
  //Secret key generation
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; i++)
  {
  	skseed_st[i]=sk[i];
  }
  
    for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; i++)
  {
  	sk_PI_seed_st[i]=sk[i+SEEDEXPANDER_SEED_BYTES];
  }
  
  for(size_t i = 0 ; i < SEEDEXPANDER_SEED_BYTES ; i++)
  {
  	sk_PO_seed_st[i]=sk[i+2*SEEDEXPANDER_SEED_BYTES];
  }
  
  biix_decaps_multisym(ct, ctRes, sk_PO_seed_st);
  
  biix_secretKey skTmp1;
  rbc_qre ctRed;
  rbc_vspace E1;
  rbc_qre xc1;
  
  rbc_qre PI;
  rbc_qre PIinv;

  rbc_field_init();
  rbc_qre_init_modulus(BIIX_PARAM_N);
   
  rbc_qre_init(&ctRed);
  rbc_qre_mod_sparse_from_string(ctRed, ctRes);
 
  biix_secret_key_from_string(&skTmp1, skseed_st);
  
  rbc_qre_init(&xc1);
  rbc_qre_init(&PI);
  rbc_qre_init(&PIinv);
  
  rbc_qre_set_arb_random(PI, 2, sk_PI_seed_st);

  rbc_qre_inv(PIinv, PI);

 
  rbc_qre_mul(ctRed, ctRed, PIinv);
  rbc_qre_mul(xc1, skTmp1.x, ctRed);
  //for(int i = 0 ; i < 40 ; i++){ printf("%02x",xc2->v[i]);}
  uint32_t dimE1 = 0;
  //Problem: This region below

  
  rbc_vspace_init(&E1, BIIX_PARAM_N);
  
  dimE1 = rbc_lrpc_RSR(E1, BIIX_PARAM_R, skTmp1.F, BIIX_PARAM_D, xc1->v, BIIX_PARAM_N);
  
  uint8_t support_st1[BIIX_RBC_VEC_R_BYTES]={0};
  rbc_vec_to_string(support_st1, E1, BIIX_PARAM_R);
  sha512(ss, support_st1, BIIX_RBC_VEC_R_BYTES);
  rbc_vspace_clear(E1);
  

  rbc_qre_clear(xc1);
  rbc_vspace_clear(skTmp1.F);
   
  rbc_qre_clear(skTmp1.x);
  rbc_qre_clear(skTmp1.y);
  
  rbc_qre_clear(PIinv);
  rbc_qre_clear(PI);
  
  rbc_qre_clear(ctRed);

  rbc_qre_clear_modulus();

  int result;

  result = dimE1 != BIIX_PARAM_R;
  
  return 0;
}

int biix_decaps_multisym(uint8_t* ct, uint8_t* ctRes, uint8_t* PO_seed, random_source prng)
{
//TODO: Carisis, 220805
   rbc_qre PO;
   rbc_qre POinv;
   
   biix_ciphertext ctTmp;
   
  
  rbc_field_init();
  rbc_qre_init_modulus(2*BIIX_PARAM_N);

  rbc_qre_init(&PO);
  rbc_qre_init(&POinv);
  rbc_qre_set_arb_random(PO, 2*BIIX_PARAM_N, PO_seed);

  rbc_qre_inv(POinv, PO);
  rbc_qre_init(&(ctTmp.syndrom));
  biix_biix_ciphertext_from_string(&ctTmp, ct);

  rbc_qre_mul(ctTmp.syndrom, ctTmp.syndrom, POinv);

  biix_biix_ciphertext_to_string(ctRes, &ctTmp);
  
  rbc_qre_clear(ctTmp.syndrom);
  rbc_qre_clear(PO);
  rbc_qre_clear(POinv);
  
  rbc_qre_clear_modulus();
   
   return 0;
  
}

void rbc_qre_mod_sparse_from_string(rbc_qre o, const uint8_t* str) {
  
  rbc_poly_sparse modulus;
  modulus = rbc_qre_get_modulus(BIIX_PARAM_N);
  int32_t modulus_degree = modulus->coeffs[modulus->coeffs_nb - 1];
  
  rbc_poly unreduced;
  rbc_poly_init(&unreduced, 2 * modulus_degree - 1);
  rbc_poly_set_zero(unreduced, 2 * modulus_degree - 1);

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

void rbc_qre_set_arb_random(rbc_qre o, int32_t deg, const uint8_t* skString) {
  
  uint32_t bytes = (BIIX_PARAM_M % 8 == 0) ? BIIX_PARAM_M / 8 : BIIX_PARAM_M / 8 + 1;
  uint8_t mask = (1 << BIIX_PARAM_M % 8) - 1;
  uint8_t random[deg * bytes];
  
  random_source sk_seedexpander;
  random_init(&sk_seedexpander, RANDOM_SOURCE_SEEDEXP);
    
  uint8_t sk_seed[SEEDEXPANDER_SEED_BYTES] = {0};
  memcpy(sk_seed, skString, SEEDEXPANDER_SEED_BYTES);
  random_seed(&sk_seedexpander, sk_seed, sk_seed + 32, SEEDEXPANDER_MAX_LENGTH);

  rbc_vec_set_zero(o->v, BIIX_PARAM_N);
  random_get_bytes(&sk_seedexpander, random, deg * bytes);

  for(size_t i = 0 ; i < deg ; ++i) {
    random[(i + 1) * bytes - 1] &= mask;
    memcpy(o->v[i], random + i * bytes, bytes);
  }
   random_clear(&sk_seedexpander);
  
}


void rbc_qre_set_arb_random_pair_from_support(random_source* ctx, rbc_qre o1, rbc_qre o2, const rbc_vec support, uint8_t copy_flag) {
  //Predefined from
   uint32_t size =  BIIX_PARAM_N-1;
   uint32_t support_size = BIIX_PARAM_R;
   
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

