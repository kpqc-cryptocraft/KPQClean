#include "params.h"
#include "packing.h"
#include "poly.h"
#include <string.h>

#ifdef DBENCH
#include "test/cpucycles.h"
extern const uint64_t timing_overhead;
extern uint64_t *tred, *tadd, *tmul, *tround, *tsample, *tpack;
#define DBENCH_START() uint64_t time = cpucycles()
#define DBENCH_STOP(t) t += cpucycles() - time - timing_overhead
#else
#define DBENCH_START()
#define DBENCH_STOP(t)
#endif

void polyeta_pack(uint8_t *r, const poly *a) {
  unsigned int i;
  uint8_t t[8];
  DBENCH_START();

#if N==1021
  for(i = 0; i < 127; ++i) {
    t[0] = ETA - a->coeffs[8*i+0];
    t[1] = ETA - a->coeffs[8*i+1];
    t[2] = ETA - a->coeffs[8*i+2];
    t[3] = ETA - a->coeffs[8*i+3];
    t[4] = ETA - a->coeffs[8*i+4];
    t[5] = ETA - a->coeffs[8*i+5];
    t[6] = ETA - a->coeffs[8*i+6];
    t[7] = ETA - a->coeffs[8*i+7];

    r[3*i+0]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[3*i+1]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[3*i+2]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
  }

  t[0] = ETA - a->coeffs[1016];
  t[1] = ETA - a->coeffs[1017];
  t[2] = ETA - a->coeffs[1018];
  t[3] = ETA - a->coeffs[1019];
  t[4] = ETA - a->coeffs[1020];

  r[381] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
  r[382] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4);
#elif N==1429
  for(i = 0; i < 178; ++i) {
    t[0] = ETA - a->coeffs[8*i+0];
    t[1] = ETA - a->coeffs[8*i+1];
    t[2] = ETA - a->coeffs[8*i+2];
    t[3] = ETA - a->coeffs[8*i+3];
    t[4] = ETA - a->coeffs[8*i+4];
    t[5] = ETA - a->coeffs[8*i+5];
    t[6] = ETA - a->coeffs[8*i+6];
    t[7] = ETA - a->coeffs[8*i+7];

    r[3*i+0]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[3*i+1]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[3*i+2]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
  }

  t[0] = ETA - a->coeffs[1424];
  t[1] = ETA - a->coeffs[1425];
  t[2] = ETA - a->coeffs[1426];
  t[3] = ETA - a->coeffs[1427];
  t[4] = ETA - a->coeffs[1428];
  
  r[534] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
  r[535] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4);
#elif N==1913
  for(i = 0; i < 239; ++i) {
    t[0] = ETA - a->coeffs[8*i+0];
    t[1] = ETA - a->coeffs[8*i+1];
    t[2] = ETA - a->coeffs[8*i+2];
    t[3] = ETA - a->coeffs[8*i+3];
    t[4] = ETA - a->coeffs[8*i+4];
    t[5] = ETA - a->coeffs[8*i+5];
    t[6] = ETA - a->coeffs[8*i+6];
    t[7] = ETA - a->coeffs[8*i+7];

    r[3*i+0]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[3*i+1]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[3*i+2]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
  }

  t[0] = ETA - a->coeffs[1912];
  
  r[717] = t[0];
#endif

  DBENCH_STOP(*tpack);
}

void polyeta_unpack(poly *r, const uint8_t *a) {
  unsigned int i;
  DBENCH_START();

#if N==1021
  for(i = 0; i < 127; ++i) {
    r->coeffs[8*i+0] =  (a[3*i+0] >> 0) & 7;
    r->coeffs[8*i+1] =  (a[3*i+0] >> 3) & 7;
    r->coeffs[8*i+2] = ((a[3*i+0] >> 6) | (a[3*i+1] << 2)) & 7;
    r->coeffs[8*i+3] =  (a[3*i+1] >> 1) & 7;
    r->coeffs[8*i+4] =  (a[3*i+1] >> 4) & 7;
    r->coeffs[8*i+5] = ((a[3*i+1] >> 7) | (a[3*i+2] << 1)) & 7;
    r->coeffs[8*i+6] =  (a[3*i+2] >> 2) & 7;
    r->coeffs[8*i+7] =  (a[3*i+2] >> 5) & 7;

    r->coeffs[8*i+0] = ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = ETA - r->coeffs[8*i+7];
  }

  r->coeffs[1016] =  (a[381] >> 0) & 7;
  r->coeffs[1017] =  (a[381] >> 3) & 7;
  r->coeffs[1018] = ((a[381] >> 6) | (a[382] << 2)) & 7;
  r->coeffs[1019] =  (a[382] >> 1) & 7;
  r->coeffs[1020] =  (a[382] >> 4) & 7;

  r->coeffs[1016] = ETA - r->coeffs[1016];
  r->coeffs[1017] = ETA - r->coeffs[1017];
  r->coeffs[1018] = ETA - r->coeffs[1018];
  r->coeffs[1019] = ETA - r->coeffs[1019];
  r->coeffs[1020] = ETA - r->coeffs[1020];
#elif N==1429
  for(i = 0; i < 178; ++i) {
    r->coeffs[8*i+0] =  (a[3*i+0] >> 0) & 7;
    r->coeffs[8*i+1] =  (a[3*i+0] >> 3) & 7;
    r->coeffs[8*i+2] = ((a[3*i+0] >> 6) | (a[3*i+1] << 2)) & 7;
    r->coeffs[8*i+3] =  (a[3*i+1] >> 1) & 7;
    r->coeffs[8*i+4] =  (a[3*i+1] >> 4) & 7;
    r->coeffs[8*i+5] = ((a[3*i+1] >> 7) | (a[3*i+2] << 1)) & 7;
    r->coeffs[8*i+6] =  (a[3*i+2] >> 2) & 7;
    r->coeffs[8*i+7] =  (a[3*i+2] >> 5) & 7;

    r->coeffs[8*i+0] = ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = ETA - r->coeffs[8*i+7];
  }
  
  r->coeffs[1424] =  (a[534] >> 0) & 7;
  r->coeffs[1425] =  (a[534] >> 3) & 7;
  r->coeffs[1426] = ((a[534] >> 6) | (a[535] << 2)) & 7;
  r->coeffs[1427] =  (a[535] >> 1) & 7;
  r->coeffs[1428] =  (a[535] >> 4) & 7;

  r->coeffs[1424] = ETA - r->coeffs[1424];
  r->coeffs[1425] = ETA - r->coeffs[1425];
  r->coeffs[1426] = ETA - r->coeffs[1426];
  r->coeffs[1427] = ETA - r->coeffs[1427];
  r->coeffs[1428] = ETA - r->coeffs[1428];
#elif N==1913
  for(i = 0; i < 239; ++i) {
    r->coeffs[8*i+0] =  (a[3*i+0] >> 0) & 7;
    r->coeffs[8*i+1] =  (a[3*i+0] >> 3) & 7;
    r->coeffs[8*i+2] = ((a[3*i+0] >> 6) | (a[3*i+1] << 2)) & 7;
    r->coeffs[8*i+3] =  (a[3*i+1] >> 1) & 7;
    r->coeffs[8*i+4] =  (a[3*i+1] >> 4) & 7;
    r->coeffs[8*i+5] = ((a[3*i+1] >> 7) | (a[3*i+2] << 1)) & 7;
    r->coeffs[8*i+6] =  (a[3*i+2] >> 2) & 7;
    r->coeffs[8*i+7] =  (a[3*i+2] >> 5) & 7;

    r->coeffs[8*i+0] = ETA - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = ETA - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = ETA - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = ETA - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = ETA - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = ETA - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = ETA - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = ETA - r->coeffs[8*i+7];
  }

  r->coeffs[1912] = a[717] & 7;

  r->coeffs[1912] = ETA - r->coeffs[1912];
#endif

  DBENCH_STOP(*tpack);
}

void polyt1_pack(uint8_t *r, const poly *a) {
  unsigned int i;
  DBENCH_START();

#if N==1021
  for(i = 0; i < 510; ++i) {
    r[3*i+0] = (a->coeffs[2*i+0] >> 0);
    r[3*i+1] = (a->coeffs[2*i+0] >> 8) | (a->coeffs[2*i+1] << 4);
    r[3*i+2] = (a->coeffs[2*i+1] >> 4);
  }

  r[1530] = a->coeffs[1020] >> 0;
  r[1531] = a->coeffs[1020] >> 8;
#elif N==1429
  for(i = 0; i < 178; ++i) {
    r[11*i+ 0] = (a->coeffs[8*i+0] >>  0);                          
    r[11*i+ 1] = (a->coeffs[8*i+0] >>  8) | (a->coeffs[8*i+1] << 3);
    r[11*i+ 2] = (a->coeffs[8*i+1] >>  5) | (a->coeffs[8*i+2] << 6);
    r[11*i+ 3] = (a->coeffs[8*i+2] >>  2);                          
    r[11*i+ 4] = (a->coeffs[8*i+2] >> 10) | (a->coeffs[8*i+3] << 1);
    r[11*i+ 5] = (a->coeffs[8*i+3] >>  7) | (a->coeffs[8*i+4] << 4);
    r[11*i+ 6] = (a->coeffs[8*i+4] >>  4) | (a->coeffs[8*i+5] << 7);
    r[11*i+ 7] = (a->coeffs[8*i+5] >>  1);                          
    r[11*i+ 8] = (a->coeffs[8*i+5] >>  9) | (a->coeffs[8*i+6] << 2);
    r[11*i+ 9] = (a->coeffs[8*i+6] >>  6) | (a->coeffs[8*i+7] << 5);
    r[11*i+10] = (a->coeffs[8*i+7] >>  3);                          
  }

  r[1958] = (a->coeffs[1424] >>  0);                          
  r[1959] = (a->coeffs[1424] >>  8) | (a->coeffs[1425] << 3); 
  r[1960] = (a->coeffs[1425] >>  5) | (a->coeffs[1426] << 6); 
  r[1961] = (a->coeffs[1426] >>  2);                          
  r[1962] = (a->coeffs[1426] >> 10) | (a->coeffs[1427] << 1); 
  r[1963] = (a->coeffs[1427] >>  7) | (a->coeffs[1428] << 4); 
  r[1964] = (a->coeffs[1428] >>  4);
#elif N==1913
  for(i = 0; i < 239; ++i) {
    r[11*i+ 0] = (a->coeffs[8*i+0] >>  0);                          
    r[11*i+ 1] = (a->coeffs[8*i+0] >>  8) | (a->coeffs[8*i+1] << 3);
    r[11*i+ 2] = (a->coeffs[8*i+1] >>  5) | (a->coeffs[8*i+2] << 6);
    r[11*i+ 3] = (a->coeffs[8*i+2] >>  2);                          
    r[11*i+ 4] = (a->coeffs[8*i+2] >> 10) | (a->coeffs[8*i+3] << 1);
    r[11*i+ 5] = (a->coeffs[8*i+3] >>  7) | (a->coeffs[8*i+4] << 4);
    r[11*i+ 6] = (a->coeffs[8*i+4] >>  4) | (a->coeffs[8*i+5] << 7);
    r[11*i+ 7] = (a->coeffs[8*i+5] >>  1);                          
    r[11*i+ 8] = (a->coeffs[8*i+5] >>  9) | (a->coeffs[8*i+6] << 2);
    r[11*i+ 9] = (a->coeffs[8*i+6] >>  6) | (a->coeffs[8*i+7] << 5);
    r[11*i+10] = (a->coeffs[8*i+7] >>  3);                          
  }

  r[2629] = (a->coeffs[1912] >> 0);
  r[2630] = (a->coeffs[1912] >> 8);
#endif
  DBENCH_STOP(*tpack);
}

void polyt1_unpack(poly *r, const uint8_t *a) {
  unsigned int i;
  DBENCH_START();

#if N==1021
  for(i = 0; i < 510; ++i) {
    r->coeffs[2*i+0] = ((a[3*i+0] >> 0) | ((uint32_t)a[3*i+1] << 8)) & 0xFFF;
    r->coeffs[2*i+1] = ((a[3*i+1] >> 4) | ((uint32_t)a[3*i+2] << 4)) & 0xFFF;
  }

  r->coeffs[1020] = ((a[1530] >> 0) | ((uint32_t)a[1531] << 8)) & 0xFFF;
#elif N==1429 
  for(i = 0; i < 178; ++i) {
    r->coeffs[8*i+0] = ((a[11*i+0] >> 0) | ((uint32_t)a[11*i+ 1] << 8)) & 0x7FF;                               
    r->coeffs[8*i+1] = ((a[11*i+1] >> 3) | ((uint32_t)a[11*i+ 2] << 5)) & 0x7FF;                               
    r->coeffs[8*i+2] = ((a[11*i+2] >> 6) | ((uint32_t)a[11*i+ 3] << 2) | ((uint32_t)a[11*i+4] << 10 )) & 0x7FF;
    r->coeffs[8*i+3] = ((a[11*i+4] >> 1) | ((uint32_t)a[11*i+ 5] << 7)) & 0x7FF;                               
    r->coeffs[8*i+4] = ((a[11*i+5] >> 4) | ((uint32_t)a[11*i+ 6] << 4)) & 0x7FF;                               
    r->coeffs[8*i+5] = ((a[11*i+6] >> 7) | ((uint32_t)a[11*i+ 7] << 1) | ((uint32_t)a[11*i+8] <<  9)) & 0x7FF; 
    r->coeffs[8*i+6] = ((a[11*i+8] >> 2) | ((uint32_t)a[11*i+ 9] << 6)) & 0x7FF;                               
    r->coeffs[8*i+7] = ((a[11*i+9] >> 5) | ((uint32_t)a[11*i+10] << 3)) & 0x7FF;                               
  }

  r->coeffs[1424] = ((a[1958] >> 0) | ((uint32_t)a[1959] << 8)) & 0x7FF;                             
  r->coeffs[1425] = ((a[1959] >> 3) | ((uint32_t)a[1960] << 5)) & 0x7FF;                             
  r->coeffs[1426] = ((a[1960] >> 6) | ((uint32_t)a[1961] << 2) | ((uint32_t)a[1962] << 10 )) & 0x7FF;
  r->coeffs[1427] = ((a[1962] >> 1) | ((uint32_t)a[1963] << 7)) & 0x7FF;                             
  r->coeffs[1428] = ((a[1963] >> 4) | ((uint32_t)a[1964] << 4)) & 0x7FF;                             
#elif N==1913
  for(i = 0; i < 239; ++i) {
    r->coeffs[8*i+0] = ((a[11*i+0] >> 0) | ((uint32_t)a[11*i+ 1] << 8)) & 0x7FF;                              
    r->coeffs[8*i+1] = ((a[11*i+1] >> 3) | ((uint32_t)a[11*i+ 2] << 5)) & 0x7FF;                              
    r->coeffs[8*i+2] = ((a[11*i+2] >> 6) | ((uint32_t)a[11*i+ 3] << 2) | ((uint32_t)a[11*i+4] << 10)) & 0x7FF;
    r->coeffs[8*i+3] = ((a[11*i+4] >> 1) | ((uint32_t)a[11*i+ 5] << 7)) & 0x7FF;                              
    r->coeffs[8*i+4] = ((a[11*i+5] >> 4) | ((uint32_t)a[11*i+ 6] << 4)) & 0x7FF;                              
    r->coeffs[8*i+5] = ((a[11*i+6] >> 7) | ((uint32_t)a[11*i+ 7] << 1) | ((uint32_t)a[11*i+8] <<  9)) & 0x7FF;
    r->coeffs[8*i+6] = ((a[11*i+8] >> 2) | ((uint32_t)a[11*i+ 9] << 6)) & 0x7FF;                              
    r->coeffs[8*i+7] = ((a[11*i+9] >> 5) | ((uint32_t)a[11*i+10] << 3)) & 0x7FF;                              
  }

  r->coeffs[1912] = ((a[2629] >> 0) | ((uint32_t)a[2630] << 8)) & 0x7FF;
#endif
  DBENCH_STOP(*tpack);
}

void polyt0_pack(uint8_t *r, const poly *a) {
  unsigned int i;
#if N==1021
  uint32_t t[8];
#else
  uint32_t t[2];
#endif
  DBENCH_START();
#if N==1021
  for(i = 0; i < 127; ++i) {
    t[0] = (1 << 10) - a->coeffs[8*i+0];
    t[1] = (1 << 10) - a->coeffs[8*i+1];
    t[2] = (1 << 10) - a->coeffs[8*i+2];
    t[3] = (1 << 10) - a->coeffs[8*i+3];
    t[4] = (1 << 10) - a->coeffs[8*i+4];
    t[5] = (1 << 10) - a->coeffs[8*i+5];
    t[6] = (1 << 10) - a->coeffs[8*i+6];
    t[7] = (1 << 10) - a->coeffs[8*i+7];

    r[11*i+ 0] = (t[0] >>  0);              
    r[11*i+ 1] = (t[0] >>  8) | (t[1] << 3);
    r[11*i+ 2] = (t[1] >>  5) | (t[2] << 6);
    r[11*i+ 3] = (t[2] >>  2);              
    r[11*i+ 4] = (t[2] >> 10) | (t[3] << 1);
    r[11*i+ 5] = (t[3] >>  7) | (t[4] << 4);
    r[11*i+ 6] = (t[4] >>  4) | (t[5] << 7);
    r[11*i+ 7] = (t[5] >>  1);              
    r[11*i+ 8] = (t[5] >>  9) | (t[6] << 2);
    r[11*i+ 9] = (t[6] >>  6) | (t[7] << 5);
    r[11*i+10] = (t[7] >>  3);              
  }

  t[0] = (1 << 10) - a->coeffs[1016];
  t[1] = (1 << 10) - a->coeffs[1017];
  t[2] = (1 << 10) - a->coeffs[1018];
  t[3] = (1 << 10) - a->coeffs[1019];
  t[4] = (1 << 10) - a->coeffs[1020];

  r[1397] = (t[0] >>  0);              
  r[1398] = (t[0] >>  8) | (t[1] << 3);
  r[1399] = (t[1] >>  5) | (t[2] << 6);
  r[1400] = (t[2] >>  2);              
  r[1401] = (t[2] >> 10) | (t[3] << 1);
  r[1402] = (t[3] >>  7) | (t[4] << 4);
  r[1403] = (t[4] >>  4);              
#elif N==1429
  for(i = 0; i < 714; ++i) {
    t[0] = (1 << 11) - a->coeffs[2*i+0];
    t[1] = (1 << 11) - a->coeffs[2*i+1];

    r[3*i+0] = (t[0] >> 0);              
    r[3*i+1] = (t[0] >> 8) | (t[1] << 4);
    r[3*i+2] = (t[1] >> 4);              
  }

  t[0] = (1 << 11) - a->coeffs[1428];

  r[2142] = (t[0] >> 0);
  r[2143] = (t[0] >> 8);
#elif N==1913
  for(i = 0; i < 956; ++i) {
    t[0] = (1 << 11) - a->coeffs[2*i+0];
    t[1] = (1 << 11) - a->coeffs[2*i+1];

    r[3*i+0] = (t[0] >> 0);              
    r[3*i+1] = (t[0] >> 8) | (t[1] << 4);
    r[3*i+2] = (t[1] >> 4);              
  }

  t[0] = (1 << 11) - a->coeffs[1912];

  r[2868] = (t[0] >> 0);
  r[2869] = (t[0] >> 8);
#endif
  DBENCH_STOP(*tpack);
}

void polyt0_unpack(poly *r, const uint8_t *a) {
  unsigned int i;
  DBENCH_START();
#if N==1021
  for(i = 0; i < 127; ++i) {
    r->coeffs[8*i+0] = ((a[11*i+0] >> 0) | ((uint32_t)a[11*i+ 1] << 8)) & 0x7FF;                               
    r->coeffs[8*i+1] = ((a[11*i+1] >> 3) | ((uint32_t)a[11*i+ 2] << 5)) & 0x7FF;                               
    r->coeffs[8*i+2] =(((a[11*i+2] >> 6) | ((uint32_t)a[11*i+ 3] << 2)) | ((uint32_t)a[11*i+4] << 10)) & 0x7FF;
    r->coeffs[8*i+3] = ((a[11*i+4] >> 1) | ((uint32_t)a[11*i+ 5] << 7)) & 0x7FF;                               
    r->coeffs[8*i+4] = ((a[11*i+5] >> 4) | ((uint32_t)a[11*i+ 6] << 4)) & 0x7FF;                               
    r->coeffs[8*i+5] =(((a[11*i+6] >> 7) | ((uint32_t)a[11*i+ 7] << 1)) | ((uint32_t)a[11*i+8] <<  9)) & 0x7FF;
    r->coeffs[8*i+6] = ((a[11*i+8] >> 2) | ((uint32_t)a[11*i+ 9] << 6)) & 0x7FF;                               
    r->coeffs[8*i+7] = ((a[11*i+9] >> 5) | ((uint32_t)a[11*i+10] << 3)) & 0x7FF;                               

    r->coeffs[8*i+0] = (1 << 10) - r->coeffs[8*i+0];
    r->coeffs[8*i+1] = (1 << 10) - r->coeffs[8*i+1];
    r->coeffs[8*i+2] = (1 << 10) - r->coeffs[8*i+2];
    r->coeffs[8*i+3] = (1 << 10) - r->coeffs[8*i+3];
    r->coeffs[8*i+4] = (1 << 10) - r->coeffs[8*i+4];
    r->coeffs[8*i+5] = (1 << 10) - r->coeffs[8*i+5];
    r->coeffs[8*i+6] = (1 << 10) - r->coeffs[8*i+6];
    r->coeffs[8*i+7] = (1 << 10) - r->coeffs[8*i+7];
  }

  r->coeffs[1016] = ((a[1397] >> 0) | ((uint32_t)a[1398] << 8)) & 0x7FF;
  r->coeffs[1017] = ((a[1398] >> 3) | ((uint32_t)a[1399] << 5)) & 0x7FF;
  r->coeffs[1018] =(((a[1399] >> 6) | ((uint32_t)a[1400] << 2)) | ((uint32_t)a[1401] << 10 )) & 0x7FF;
  r->coeffs[1019] = ((a[1401] >> 1) | ((uint32_t)a[1402] << 7)) & 0x7FF;
  r->coeffs[1020] = ((a[1402] >> 4) | ((uint32_t)a[1403] << 4)) & 0x7FF;

  r->coeffs[1016] = (1 << 10) - r->coeffs[1016];
  r->coeffs[1017] = (1 << 10) - r->coeffs[1017];
  r->coeffs[1018] = (1 << 10) - r->coeffs[1018];
  r->coeffs[1019] = (1 << 10) - r->coeffs[1019];
  r->coeffs[1020] = (1 << 10) - r->coeffs[1020];
#elif N==1429
  for(i = 0; i < 714; ++i) {
    r->coeffs[2*i+0] = ((a[3*i+0] >> 0) | ((uint32_t)a[3*i+1] << 8)) & 0xFFF;
    r->coeffs[2*i+1] = ((a[3*i+1] >> 4) | ((uint32_t)a[3*i+2] << 4)) & 0xFFF;

    r->coeffs[2*i+0] = (1 << 11) - r->coeffs[2*i+0];
    r->coeffs[2*i+1] = (1 << 11) - r->coeffs[2*i+1];
  }

  r->coeffs[1428] = ((a[2142] >> 0) | ((uint32_t)a[2143] << 8)) & 0xFFF;

  r->coeffs[1428] = (1 << 11) - r->coeffs[1428];
#elif N==1913
  for(i = 0; i < 956; ++i) {
    r->coeffs[2*i+0] = ((a[3*i+0] >> 0) | ((uint32_t)a[3*i+1] << 8)) & 0xFFF;
    r->coeffs[2*i+1] = ((a[3*i+1] >> 4) | ((uint32_t)a[3*i+2] << 4)) & 0xFFF;
    
    r->coeffs[2*i+0] = (1 << 11) - r->coeffs[2*i+0];
    r->coeffs[2*i+1] = (1 << 11) - r->coeffs[2*i+1];
  }

  r->coeffs[1912] = ((a[2868] >> 0) | ((uint32_t)a[2869] << 8)) & 0xFFF;

  r->coeffs[1912] = (1 << 11) - r->coeffs[1912];
#endif
  DBENCH_STOP(*tpack);
}

void polyz_pack(uint8_t *r, const poly *a) {
	DBENCH_START();

#if GAMMA1 == (1<<17)
	unsigned int i;
	uint32_t t[4];

	for (i = 0; i < 255; ++i)
	{
		t[0] = GAMMA1 - a->coeffs[4 * i + 0];
		t[1] = GAMMA1 - a->coeffs[4 * i + 1];
		t[2] = GAMMA1 - a->coeffs[4 * i + 2];
		t[3] = GAMMA1 - a->coeffs[4 * i + 3];

		r[9 * i + 0] = (t[0] >> 0);
		r[9 * i + 1] = (t[0] >> 8);
		r[9 * i + 2] = (t[0] >> 16) | (t[1] << 2);
		r[9 * i + 3] = (t[1] >> 6);
		r[9 * i + 4] = (t[1] >> 14) | (t[2] << 4);
		r[9 * i + 5] = (t[2] >> 4);
		r[9 * i + 6] = (t[2] >> 12) | (t[3] << 6);
		r[9 * i + 7] = (t[3] >> 2);
		r[9 * i + 8] = (t[3] >> 10);
	}

	t[0] = GAMMA1 - a->coeffs[1020];

	r[2295] = t[0] >> 0;
	r[2296] = t[0] >> 8;
	r[2297] = t[0] >> 16;
#elif GAMMA1 == (1<<18)
	unsigned int i;
	uint32_t t[8];

	for (i = 0; i < 178; ++i)
	{
		t[0] = GAMMA1 - a->coeffs[8 * i + 0];
		t[1] = GAMMA1 - a->coeffs[8 * i + 1];
		t[2] = GAMMA1 - a->coeffs[8 * i + 2];
		t[3] = GAMMA1 - a->coeffs[8 * i + 3];
		t[4] = GAMMA1 - a->coeffs[8 * i + 4];
		t[5] = GAMMA1 - a->coeffs[8 * i + 5];
		t[6] = GAMMA1 - a->coeffs[8 * i + 6];
		t[7] = GAMMA1 - a->coeffs[8 * i + 7];

		r[19 * i + 0] = (t[0] >> 0);
		r[19 * i + 1] = (t[0] >> 8);
		r[19 * i + 2] = (t[0] >> 16) | (t[1] << 3);
		r[19 * i + 3] = (t[1] >> 5);
		r[19 * i + 4] = (t[1] >> 13) | (t[2] << 6);
		r[19 * i + 5] = (t[2] >> 2);
		r[19 * i + 6] = (t[2] >> 10);
		r[19 * i + 7] = (t[2] >> 18) | (t[3] << 1);
		r[19 * i + 8] = (t[3] >> 7);
		r[19 * i + 9] = (t[3] >> 15) | (t[4] << 4);
		r[19 * i + 10] = (t[4] >> 4);
		r[19 * i + 11] = (t[4] >> 12) | (t[5] << 7);
		r[19 * i + 12] = (t[5] >> 1);
		r[19 * i + 13] = (t[5] >> 9);
		r[19 * i + 14] = (t[5] >> 17) | (t[6] << 2);
		r[19 * i + 15] = (t[6] >> 6);
		r[19 * i + 16] = (t[6] >> 14) | (t[7] << 5);
		r[19 * i + 17] = (t[7] >> 3);
		r[19 * i + 18] = (t[7] >> 11);
	}

	t[0] = GAMMA1 - a->coeffs[1424];
	t[1] = GAMMA1 - a->coeffs[1425];
	t[2] = GAMMA1 - a->coeffs[1426];
	t[3] = GAMMA1 - a->coeffs[1427];
	t[4] = GAMMA1 - a->coeffs[1428];

	r[3382] = (t[0] >> 0);
	r[3383] = (t[0] >> 8);
	r[3384] = (t[0] >> 16) | (t[1] << 3);
	r[3385] = (t[1] >> 5);
	r[3386] = (t[1] >> 13) | (t[2] << 6);
	r[3387] = (t[2] >> 2);
	r[3388] = (t[2] >> 10);
	r[3389] = (t[2] >> 18) | (t[3] << 1);
	r[3390] = (t[3] >> 7);
	r[3391] = (t[3] >> 15) | (t[4] << 4);
	r[3392] = (t[4] >> 4);
	r[3393] = (t[4] >> 12);
#elif GAMMA1 == (1<<19)
	unsigned int i;
	uint32_t t[2];

	for (i = 0; i < 956; ++i)
	{
		t[0] = GAMMA1 - a->coeffs[2 * i + 0];
		t[1] = GAMMA1 - a->coeffs[2 * i + 1];

		r[5 * i + 0] = (t[0] >> 0);
		r[5 * i + 1] = (t[0] >> 8);
		r[5 * i + 2] = (t[0] >> 16) | (t[1] << 4);
		r[5 * i + 3] = (t[1] >> 4);
		r[5 * i + 4] = (t[1] >> 12);
	}

	t[0] = GAMMA1 - a->coeffs[1912];

	r[4780] = t[0] >> 0;
	r[4781] = t[0] >> 8;
	r[4782] = t[0] >> 16;
#endif
	DBENCH_STOP(*tpack);
}

void polyz_unpack(poly *r, const uint8_t *a) {
	unsigned int i;
	DBENCH_START();

#if (GAMMA1==(1<<17))
	for (i = 0; i < 255; ++i)
	{
		r->coeffs[4 * i + 0] = ((a[9 * i + 0] >> 0) | ((uint32_t)a[9 * i + 1] << 8) | ((uint32_t)a[9 * i + 2] << 16)) & 0x3FFFF;
		r->coeffs[4 * i + 1] = ((a[9 * i + 2] >> 2) | ((uint32_t)a[9 * i + 3] << 6) | ((uint32_t)a[9 * i + 4] << 14)) & 0x3FFFF;
		r->coeffs[4 * i + 2] = ((a[9 * i + 4] >> 4) | ((uint32_t)a[9 * i + 5] << 4) | ((uint32_t)a[9 * i + 6] << 12)) & 0x3FFFF;
		r->coeffs[4 * i + 3] = ((a[9 * i + 6] >> 6) | ((uint32_t)a[9 * i + 7] << 2) | ((uint32_t)a[9 * i + 8] << 10)) & 0x3FFFF;

		r->coeffs[4 * i + 0] = GAMMA1 - r->coeffs[4 * i + 0];
		r->coeffs[4 * i + 1] = GAMMA1 - r->coeffs[4 * i + 1];
		r->coeffs[4 * i + 2] = GAMMA1 - r->coeffs[4 * i + 2];
		r->coeffs[4 * i + 3] = GAMMA1 - r->coeffs[4 * i + 3];
	}
	r->coeffs[1020] = ((a[2295] >> 0) | ((uint32_t)a[2296] << 8) | ((uint32_t)a[2297] << 16)) & 0x3FFFF;

	r->coeffs[1020] = GAMMA1 - r->coeffs[1020];
#elif (GAMMA1==(1<<18))
	for (i = 0; i < 178; ++i)
	{
		r->coeffs[8 * i + 0] = ((a[19 * i +  0] >> 0) | ((uint32_t)a[19 * i +  1] << 8) | ((uint32_t)a[19 * i +  2] << 16)) & 0x7FFFF;
		r->coeffs[8 * i + 1] = ((a[19 * i +  2] >> 3) | ((uint32_t)a[19 * i +  3] << 5) | ((uint32_t)a[19 * i +  4] << 13)) & 0x7FFFF;
		r->coeffs[8 * i + 2] = ((a[19 * i +  4] >> 6) | ((uint32_t)a[19 * i +  5] << 2) | ((uint32_t)a[19 * i +  6] << 10) | ((uint32_t)a[19 * i +  7] << 18)) & 0x7FFFF;
		r->coeffs[8 * i + 3] = ((a[19 * i +  7] >> 1) | ((uint32_t)a[19 * i +  8] << 7) | ((uint32_t)a[19 * i +  9] << 15)) & 0x7FFFF;
		r->coeffs[8 * i + 4] = ((a[19 * i +  9] >> 4) | ((uint32_t)a[19 * i + 10] << 4) | ((uint32_t)a[19 * i + 11] << 12)) & 0x7FFFF;
		r->coeffs[8 * i + 5] = ((a[19 * i + 11] >> 7) | ((uint32_t)a[19 * i + 12] << 1) | ((uint32_t)a[19 * i + 13] <<  9) | ((uint32_t)a[19 * i + 14] << 17)) & 0x7FFFF;
		r->coeffs[8 * i + 6] = ((a[19 * i + 14] >> 2) | ((uint32_t)a[19 * i + 15] << 6) | ((uint32_t)a[19 * i + 16] << 14)) & 0x7FFFF;
		r->coeffs[8 * i + 7] = ((a[19 * i + 16] >> 5) | ((uint32_t)a[19 * i + 17] << 3) | ((uint32_t)a[19 * i + 18] << 11)) & 0x7FFFF;

		r->coeffs[8 * i + 0] = GAMMA1 - r->coeffs[8 * i + 0];
		r->coeffs[8 * i + 1] = GAMMA1 - r->coeffs[8 * i + 1];
		r->coeffs[8 * i + 2] = GAMMA1 - r->coeffs[8 * i + 2];
		r->coeffs[8 * i + 3] = GAMMA1 - r->coeffs[8 * i + 3];
		r->coeffs[8 * i + 4] = GAMMA1 - r->coeffs[8 * i + 4];
		r->coeffs[8 * i + 5] = GAMMA1 - r->coeffs[8 * i + 5];
		r->coeffs[8 * i + 6] = GAMMA1 - r->coeffs[8 * i + 6];
		r->coeffs[8 * i + 7] = GAMMA1 - r->coeffs[8 * i + 7];
	}
	r->coeffs[1424] = ((a[3382] >> 0) | ((uint32_t)a[3383] << 8) | ((uint32_t)a[3384] << 16)) & 0x7FFFF;
	r->coeffs[1425] = ((a[3384] >> 3) | ((uint32_t)a[3385] << 5) | ((uint32_t)a[3386] << 13)) & 0x7FFFF;
	r->coeffs[1426] = ((a[3386] >> 6) | ((uint32_t)a[3387] << 2) | ((uint32_t)a[3388] << 10) | ((uint32_t)a[3389] << 18)) & 0x7FFFF;
	r->coeffs[1427] = ((a[3389] >> 1) | ((uint32_t)a[3390] << 7) | ((uint32_t)a[3391] << 15)) & 0x7FFFF;
	r->coeffs[1428] = ((a[3391] >> 4) | ((uint32_t)a[3392] << 4) | ((uint32_t)a[3393] << 12)) & 0x7FFFF;

	r->coeffs[1424] = GAMMA1 - r->coeffs[1424];
	r->coeffs[1425] = GAMMA1 - r->coeffs[1425];
	r->coeffs[1426] = GAMMA1 - r->coeffs[1426];
	r->coeffs[1427] = GAMMA1 - r->coeffs[1427];
	r->coeffs[1428] = GAMMA1 - r->coeffs[1428];
#elif (GAMMA1==(1<<19))
	for (i = 0; i < 956; ++i)
	{
		r->coeffs[2 * i + 0] = ((a[5 * i + 0] >> 0) | ((uint32_t)a[5 * i + 1] << 8) | ((uint32_t)a[5 * i + 2] << 16)) & 0xFFFFF;
		r->coeffs[2 * i + 1] = ((a[5 * i + 2] >> 4) | ((uint32_t)a[5 * i + 3] << 4) | ((uint32_t)a[5 * i + 4] << 12)) & 0xFFFFF;

		r->coeffs[2 * i + 0] = GAMMA1 - r->coeffs[2 * i + 0];
		r->coeffs[2 * i + 1] = GAMMA1 - r->coeffs[2 * i + 1];
	}
	r->coeffs[1912] = ((a[4780] >> 0) | ((uint32_t)a[4781] << 8) | ((uint32_t)a[4782] << 16)) & 0xFFFFF;

	r->coeffs[1912] = GAMMA1 - r->coeffs[1912];
#endif
	DBENCH_STOP(*tpack);
}

void polyw1_pack(uint8_t *r, const poly *a) {
  unsigned int i;
  DBENCH_START();

#if N==1021
  for(i = 0; i < 255; ++i){
    r[3*i+0]  = (a->coeffs[4*i+0] >> 0) | (a->coeffs[4*i+1] << 6);
    r[3*i+1]  = (a->coeffs[4*i+1] >> 2) | (a->coeffs[4*i+2] << 4);
    r[3*i+2]  = (a->coeffs[4*i+2] >> 4) | (a->coeffs[4*i+3] << 2);
  }

  r[765] = (a->coeffs[1020]);
#elif N==1429
  for(i = 0; i < 178; ++i){
    r[5*i+0]  = (a->coeffs[8*i+0] >> 0) | (a->coeffs[8*i+1] << 5);                            
    r[5*i+1]  = (a->coeffs[8*i+1] >> 3) | (a->coeffs[8*i+2] << 2) | (a->coeffs[8*i+3] << 7);  
    r[5*i+2]  = (a->coeffs[8*i+3] >> 1) | (a->coeffs[8*i+4] << 4);                            
    r[5*i+3]  = (a->coeffs[8*i+4] >> 4) | (a->coeffs[8*i+5] << 1) | (a->coeffs[8*i+6] << 6);  
    r[5*i+4]  = (a->coeffs[8*i+6] >> 2) | (a->coeffs[8*i+7] << 3);                            
  }

  r[890]  = (a->coeffs[1424] >> 0) | (a->coeffs[1425] << 5);                            
  r[891]  = (a->coeffs[1425] >> 3) | (a->coeffs[1426] << 2) | (a->coeffs[1427] << 7);   
  r[892]  = (a->coeffs[1427] >> 1) | (a->coeffs[1428] << 4);                            
  r[893]  = (a->coeffs[1428] >> 4)
#elif N==1913
  for(i = 0; i < 239; ++i){
    r[5*i+0]  = (a->coeffs[8*i+0] >> 0) | (a->coeffs[8*i+1] << 5);                          
    r[5*i+1]  = (a->coeffs[8*i+1] >> 3) | (a->coeffs[8*i+2] << 2) | (a->coeffs[8*i+3] << 7);
    r[5*i+2]  = (a->coeffs[8*i+3] >> 1) | (a->coeffs[8*i+4] << 4);                          
    r[5*i+3]  = (a->coeffs[8*i+4] >> 4) | (a->coeffs[8*i+5] << 1) | (a->coeffs[8*i+6] << 6);
    r[5*i+4]  = (a->coeffs[8*i+6] >> 2) | (a->coeffs[8*i+7] << 3);                          
  }

  r[1195] = (a->coeffs[1912] >> 0);
#endif
  DBENCH_STOP(*tpack);
}

void pack_pk(uint8_t pk[CRYPTO_PUBLICKEYBYTES],
             const uint8_t zeta[SEEDBYTES],
             const poly *t1)
{
  unsigned int i;

  for(i = 0; i < SEEDBYTES; ++i)
    pk[i] = zeta[i];
  pk += SEEDBYTES;

  polyt1_pack(pk, t1);
}

void unpack_pk(uint8_t zeta[SEEDBYTES],
               poly *t1,
               const uint8_t pk[CRYPTO_PUBLICKEYBYTES])
{
  unsigned int i;

  for(i = 0; i < SEEDBYTES; ++i)
    zeta[i] = pk[i];
  pk += SEEDBYTES;

  polyt1_unpack(t1, pk);
}

void pack_sk(uint8_t sk[CRYPTO_SECRETKEYBYTES],
             const uint8_t zeta[SEEDBYTES],
             const uint8_t tr[SEEDBYTES],
             const uint8_t key[SEEDBYTES],
             const poly *t0,
             const poly *s1,
             const poly *s2)
{
  unsigned int i;

  for(i = 0; i < SEEDBYTES; ++i)
    sk[i] = zeta[i];
  sk += SEEDBYTES;

  for(i = 0; i < SEEDBYTES; ++i)
    sk[i] = key[i];
  sk += SEEDBYTES;

  for(i = 0; i < SEEDBYTES; ++i)
    sk[i] = tr[i];
  sk += SEEDBYTES;

  polyeta_pack(sk, s1);
  sk += POLYETA_PACKEDBYTES;

  polyeta_pack(sk, s2);
  sk += POLYETA_PACKEDBYTES;

  polyt0_pack(sk, t0);
}

void unpack_sk(uint8_t zeta[SEEDBYTES],
               uint8_t tr[SEEDBYTES],
               uint8_t key[SEEDBYTES],
               poly *t0,
               poly *s1,
               poly *s2,
               const uint8_t sk[CRYPTO_SECRETKEYBYTES])
{
  unsigned int i;

  for(i = 0; i < SEEDBYTES; ++i)
    zeta[i] = sk[i];
  sk += SEEDBYTES;

  for(i = 0; i < SEEDBYTES; ++i)
    key[i] = sk[i];
  sk += SEEDBYTES;

  for(i = 0; i < SEEDBYTES; ++i)
    tr[i] = sk[i];
  sk += SEEDBYTES;

  polyeta_unpack(s1, sk);
  sk += POLYETA_PACKEDBYTES;

  polyeta_unpack(s2, sk);
  sk += POLYETA_PACKEDBYTES;

  polyt0_unpack(t0, sk);
}

void pack_sig(uint8_t sig[CRYPTO_BYTES],
              const uint8_t c[SEEDBYTES],
              const poly *z,
              const poly *h)
{
  unsigned int i;

  for(i=0; i < SEEDBYTES; ++i)
    sig[i] = c[i];
  sig += SEEDBYTES;

  polyz_pack(sig, z);
  sig += POLYZ_PACKEDBYTES;

  /* Encode h */
  for (i = 0; i < POLYH_PACKEDBYTES-1; i++)
  {
	  sig[i]  = (h->coeffs[8 * i + 0] << 0);
	  sig[i] |= (h->coeffs[8 * i + 1] << 1);
	  sig[i] |= (h->coeffs[8 * i + 2] << 2);
	  sig[i] |= (h->coeffs[8 * i + 3] << 3);
	  sig[i] |= (h->coeffs[8 * i + 4] << 4);
	  sig[i] |= (h->coeffs[8 * i + 5] << 5);
	  sig[i] |= (h->coeffs[8 * i + 6] << 6);
	  sig[i] |= (h->coeffs[8 * i + 7] << 7);
  }
#if (N==1913)
  sig[POLYH_PACKEDBYTES - 1] = h->coeffs[8 * POLYH_PACKEDBYTES - 8];
#else
  sig[POLYH_PACKEDBYTES - 1]  = (h->coeffs[8 * POLYH_PACKEDBYTES - 8] << 0);
  sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 7] << 1);
  sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 6] << 2);
  sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 5] << 3);
  sig[POLYH_PACKEDBYTES - 1] |= (h->coeffs[8 * POLYH_PACKEDBYTES - 4] << 4);
#endif
}

int unpack_sig(uint8_t c[SEEDBYTES],
               poly *z,
               poly *h,
               const uint8_t sig[CRYPTO_BYTES])
{
  unsigned int i;

  for(i = 0; i < SEEDBYTES; ++i)
    c[i] = sig[i];
  sig += SEEDBYTES;

  polyz_unpack(z, sig);
  sig += POLYZ_PACKEDBYTES;

  /* Decode h */
  for (i = 0; i < POLYH_PACKEDBYTES - 1; i++)
  {
	  h->coeffs[8 * i + 0] = (sig[i] >> 0) & 0x1;
	  h->coeffs[8 * i + 1] = (sig[i] >> 1) & 0x1;
	  h->coeffs[8 * i + 2] = (sig[i] >> 2) & 0x1;
	  h->coeffs[8 * i + 3] = (sig[i] >> 3) & 0x1;
	  h->coeffs[8 * i + 4] = (sig[i] >> 4) & 0x1;
	  h->coeffs[8 * i + 5] = (sig[i] >> 5) & 0x1;
	  h->coeffs[8 * i + 6] = (sig[i] >> 6) & 0x1;
	  h->coeffs[8 * i + 7] = (sig[i] >> 7) & 0x1;
  }
#if (N==1913)
  h->coeffs[8 * POLYH_PACKEDBYTES - 8] = sig[POLYH_PACKEDBYTES - 1] & 0x1;
#else
  h->coeffs[8 * POLYH_PACKEDBYTES - 8] = (sig[POLYH_PACKEDBYTES - 1] >> 0) & 0x1;
  h->coeffs[8 * POLYH_PACKEDBYTES - 7] = (sig[POLYH_PACKEDBYTES - 1] >> 1) & 0x1;
  h->coeffs[8 * POLYH_PACKEDBYTES - 6] = (sig[POLYH_PACKEDBYTES - 1] >> 2) & 0x1;
  h->coeffs[8 * POLYH_PACKEDBYTES - 5] = (sig[POLYH_PACKEDBYTES - 1] >> 3) & 0x1;
  h->coeffs[8 * POLYH_PACKEDBYTES - 4] = (sig[POLYH_PACKEDBYTES - 1] >> 4) & 0x1;
#endif

  return 0;
}
