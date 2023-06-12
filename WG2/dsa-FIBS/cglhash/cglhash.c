/********************************************************************************************
* CGLHash Library
*
* Abstract: CGL Hash function
*********************************************************************************************/ 

#include "random/random.h"
#include <stdio.h>

static void init_basis(digit_t *gen, f2elm_t XP, f2elm_t XQ, f2elm_t XR)
{ // Initialization of basis points
    
    fpcopy(gen,                  XP[0]);
    fpcopy(gen +   NWORDS_FIELD, XP[1]);
    fpcopy(gen + 2*NWORDS_FIELD, XQ[0]);
    fpcopy(gen + 3*NWORDS_FIELD, XQ[1]);
    fpcopy(gen + 4*NWORDS_FIELD, XR[0]);
    fpcopy(gen + 5*NWORDS_FIELD, XR[1]);
}


void random_mod_order_A(unsigned char* random_digits)
{  
   // Outputs random value in [0, 2^eA - 1]

    randombytes(random_digits, MESSAGE_BYTES);
    random_digits[MESSAGE_BYTES-1] &= MASK_HASH;    // Masking last byte 
}




#define BENCH_LOOPS       100000



int64_t cpucycles_cgl(void)
{ // Access system counter for benchmarking
#if (OS_TARGET == OS_WIN) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
    return __rdtsc();
#elif (OS_TARGET == OS_WIN) && (TARGET == TARGET_ARM)
    return __rdpmccntr64();
#elif (OS_TARGET == OS_NIX) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
    unsigned int hi, lo;

    asm volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
    return ((int64_t)lo) | (((int64_t)hi) << 32);
#elif (OS_TARGET == OS_NIX) && (TARGET == TARGET_S390X)
    uint64_t tod;
    __asm__ volatile("stckf %0\n" : "=Q" (tod) : : "cc");
    return (tod);
#elif (OS_TARGET == OS_NIX) && (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
#else
    return 0;            
#endif
}


int ecisog_run()
{
    bool OK = true;
    int n;
    unsigned long long cycles, cycles1, cycles2;
    f2elm_t A24, C24, A4, A, C, coeff[5];
    point_proj_t P, Q;
        
    printf("\n--------------------------------------------------------------------------------------------------------\n\n"); 
    printf("Benchmarking elliptic curve and isogeny functions: \n\n"); 

    // Point doubling
    cycles = 0;
    for (n=0; n<BENCH_LOOPS; n++)
    {
        random_mod_order_A((digit_t*)A24); random_mod_order_A((digit_t*)C24);

        cycles1 = cpucycles_cgl(); 
        xDBL(P, Q, A24, C24);
        cycles2 = cpucycles_cgl();
        cycles = cycles+(cycles2-cycles1);
    }
    printf("  Point doubling runs in .......................................... %7lld ", cycles/BENCH_LOOPS);
    printf("\n");

    // 4-isogeny of a projective point
    cycles = 0;
    for (n=0; n<BENCH_LOOPS; n++)
    {
        random_mod_order_A((digit_t*)A); random_mod_order_A((digit_t*)coeff[0]); random_mod_order_A((digit_t*)coeff[1]); random_mod_order_A((digit_t*)coeff[2]);

        cycles1 = cpucycles_cgl(); 
        get_4_isog(P, A, C, coeff);
        cycles2 = cpucycles_cgl();
        cycles = cycles+(cycles2-cycles1);
    }
    printf("  4-isogeny of projective point runs in ........................... %7lld ", cycles/BENCH_LOOPS); 
    printf("\n");

    // 4-isogeny evaluation at projective point
    cycles = 0;
    for (n=0; n<BENCH_LOOPS; n++)
    {
        random_mod_order_A((digit_t*)A); random_mod_order_A((digit_t*)coeff[0]); random_mod_order_A((digit_t*)coeff[1]); random_mod_order_A((digit_t*)coeff[2]);

        cycles1 = cpucycles_cgl(); 
        eval_4_isog(P, coeff);
        cycles2 = cpucycles_cgl();
        cycles = cycles+(cycles2-cycles1);
    }
    printf("  4-isogeny evaluation at projective point runs in ................ %7lld ", cycles/BENCH_LOOPS); 
    printf("\n");


    
    return OK;
}



// 
int CGLhash(const unsigned char* MessageA, unsigned char* DigestA, int inlen)
{ // CGL-Hash function
  // Input:  message [0, 2^(eA/2)-1] 
  // Output: h(m) 
    point_proj_t R, Rtmp, pts[MAX_INT_POINTS_HASH];
    f2elm_t XPA, XQA, XRA, coeff[3], A24plus = {0}, C24 = {0}, A = {0}, t0, t1, jinv;
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_HASH], npts = 0, ii = 0;
    digit_t SecretKeyA[NWORDS_ORDER] = {0};

    // Initialize basis points
    init_basis((digit_t*)H_gen, XPA, XQA, XRA);


    // Initialize constants: A24plus = 2C, C24 = 4C, where A=0, C=1
    fpcopy((digit_t*)&Montgomery_one, A24plus[0]); //1
    mp2_add(A24plus, A24plus, A24plus); //2
    mp2_add(A24plus, A24plus, C24); //4
    

    // Retrieve kernel point
    decode_to_digits(MessageA, SecretKeyA, inlen, NWORDS_ORDER);
    
 //   SecretKeyA[0]=0x21C3E97D27DEE8B4;
 //   SecretKeyA[1]=0x520288E3E6DCF438;
  //  SecretKeyA[2]=0xE2D7ACE256FAD1F5;
  //  SecretKeyA[3]=0x10A1AE;
  //  SecretKeyA[4]=0x0;
    
    LADDER3PT(XPA, XQA, XRA, SecretKeyA, HASH, R, A);   
    

    index=0;
    int bitlen=inlen<<3; // inlen*3
    
   // printf("bitlen : %d ", bitlen);
    /*
    for(row=1; row<bitlen; row++)
    {
        m=bitlen-row;
        xDBLe(R, Rtmp, A24plus, C24, (int)(m));
        
        //get
        get_2_isog(Rtmp, A24plus, C24);
        eval_2_isog(R, Rtmp);
        
    }
    get_2_isog(R, A24plus, C24);
    */
    
    index = 0;   

    bitlen = bitlen >>1; //4-isogeny

    for (row = 1; row < bitlen; row++) {
        while (index < bitlen-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts] = index;
            npts+=1;
            m = strat_Hash[MAX_hash-index-row];
            xDBLe(R, R, A24plus, C24, (int)(2*m));
            index += m;
        }
        get_4_isog(R, A24plus, C24, coeff);      
   

        for (i = 0; i < npts; i++) {
            eval_4_isog(pts[i], coeff);
        }


        fp2copy(pts[npts-1]->X, R->X); 
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }

    get_4_isog(R, A24plus, C24, coeff);  // A+2c, 4C
  //  printf("cnt %d \n", cnt+1);
    
    
    
    
    mp2_add(A24plus, A24plus, t0); //t0=2A+4C
    fp2sub(t0, C24, A24plus); //A24plus = 2A
    mp2_add(A24plus, A24plus, A24plus); //A24plus=4A
    
    //test

    
    
    j_inv(A24plus, C24, jinv);


    // Format public key                   
    fp2_encode(jinv, DigestA);

    return 0;
}

