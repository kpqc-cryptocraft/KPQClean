/*
 * Peregrine signature generation.
 * by Eun-Young Seo
 * Version: Nov. 16, 2022
 */

#include "inner.h"
#include <stdio.h>
#include <stdlib.h>

/* =================================================================== */
/*
 * Compute degree N from logarithm 'logn'.
 */
#define MKN(logn)   ((size_t)1 << (logn))

static void random_CBD13(inner_shake256_context *rng, int16_t *random_J, unsigned logn)//-13<=x<=13
{
    size_t n;
    n = MKN(logn - 1);
    uint64_t r, d;
    int i, j;
    uint32_t a[2], b[2];
  
    for(i = 0; i < n; i++)
    {
        r = get_rng_u64(rng);
            
        d = 0;
        for (j = 0; j < 13; j++)
            d += (r >> j) & 0x8004002001;

        a[0] = d & 0xf;
        b[0] = (d >> 13) & 0xf;
        a[1] = (d >> 26) & 0xf;
        b[1] = (d >> 39) & 0xf;
            
        random_J[2 * i] = (int8_t)(a[0] - b[0]);
        random_J[2 * i + 1] = (int8_t)(a[1] - b[1]);
    }
}
static void random_CBD3(inner_shake256_context *rng, int16_t *random_J, unsigned logn)//EYSEO CBD -3<=x<=3
{
    size_t n;
    n = MKN(logn - 2);
    uint64_t r;
    int i, j;
    uint32_t d, a[4], b[4];
    
    for(i = 0; i < n; i++)
    {
        r = get_rng_u64(rng);
        d = 0;
        for (j = 0; j < 3; j++)
            d += (r >> j) & 0x249249;

        a[0] = d & 0x3;
        b[0] = (d >> 3) & 0x3;
        a[1] = (d >> 6) & 0x3;
        b[1] = (d >> 9) & 0x3;
        a[2] = (d >> 12) & 0x3;
        b[2] = (d >> 15) & 0x3;
        a[3] = (d >> 18) & 0x3;
        b[3] = (d >> 21) & 0x3;

        random_J[4 * i + 0] = (int8_t)(a[0] - b[0]);
        random_J[4 * i + 1] = (int8_t)(a[1] - b[1]);
        random_J[4 * i + 2] = (int8_t)(a[2] - b[2]);
        random_J[4 * i + 3] = (int8_t)(a[3] - b[3]);
    }
}
static void random_CBD1(inner_shake256_context *rng, int16_t *random_J, unsigned logn)//EYSEO CBD -1<=x<=1
{
    size_t n;
    n = MKN(logn - 5);
    uint64_t r;
    int i, j;
    uint8_t a[32], b[32];
    
    for(i = 0; i < n; i++)
    {
        r = get_rng_u64(rng);
        for (j = 0; j < 32; j++)
        {
            a[j] = r & 0x1;
            b[j] = (r & 0x2)>>1;
            random_J[32 * i + j] = (int8_t)(a[j] - b[j]);
            r = r >> 2;
        }
    }
}
static void random_CBD9(inner_shake256_context *rng, int16_t *random_J, unsigned logn)//EYSEO CBD -9<=x<=9
{
    size_t n;
    n = MKN(logn - 2);
    uint64_t r;
    int i, j;
    uint32_t d, a[2], b[2];
    
    for(i = 0; i < n; i++)
    {
        r = get_rng_u64(rng);
        d = 0;
        for (j = 0; j < 9; j++)
            d += (r >> j) & 0x8040201;

        a[0] = d & 0xf;
        b[0] = (d >> 9) & 0xf;
        a[1] = (d >> 18) & 0xf;
        b[1] = (d >> 27) & 0xf;

        random_J[2 * i] = (int8_t)(a[0] - b[0]);
        random_J[2 * i + 1] = (int8_t)(a[1] - b[1]);
    }
}
static void random_uniform4(inner_shake256_context *rng, int16_t *random_J, unsigned logn)//EYSEO -3<=x<=4 uniform
{
    size_t n;
    n = MKN(logn - 2);
    uint64_t r, d;
    int i, j;
    uint32_t a[4], b[4];
    
    for(i = 0; i < n; i++)
    {
        r = get_rng_u64(rng);
           
        random_J[4 * i] = (int8_t)(r & 0xf)-7;
        random_J[4 * i + 1] = (int8_t)((r>>4) & 0xf)-7;
        random_J[4 * i + 2] = (int8_t)((r>>8) & 0xf)-7;
        random_J[4 * i + 3] = (int8_t)((r>>12) & 0xf)-7;
    }
}

/*
 * Compute a signature: the signature contains two vectors, s1 and s2.
 * The s1 vector is not returned. The squared norm of (s1,s2) is
 * computed, and if it is short enough, then s2 is returned into the
 * s2[] buffer, and 1 is returned; otherwise, s2[] is untouched and 0 is
 * returned; the caller should then try again.
 *
 * tmp[] must have room for at least nine polynomials.
 */
static void
do_sign_dyn_peregrine(int16_t *s2, inner_shake256_context *rng,
    const int8_t *restrict f, const int8_t *restrict g,
    const int8_t *restrict F, const int8_t *restrict G,
    const uint16_t *hm, const uint16_t *h_src, unsigned logn, uint8_t *restrict tmp)
{
    //size_t n, u;
    int n, i, check;
    int16_t *int16_s1, *int16_s2, *int16_J1, *int16_J2;
    uint16_t *uint16_s1, *uint16_s2, *uint16_I1, *uint16_I2, *uint16_J1, *uint16_J2, *uint16_i1, *uint16_i2, *uint16_tmp1, *uint16_tmp2;
    uint32_t *uint32_i1, *uint32_i2, *uint32_gm, *uint32_igm;
    uint32_t p1, p01, R21, hq, inv_q_p;
    
    hq = 6144;
    inv_q_p = 185756712;

    const small_prime *primes;
    primes = PRIMES;
    n = MKN(logn);
    check = 1;
    
    //hm 은 0에서 12288 까지의 양의 정수라고 가정하고 계산
    
    uint32_i1 = (uint32_t *)tmp;
    uint32_i2 = uint32_i1 + n;
    uint32_gm = uint32_i2 + n;
    uint32_igm = uint32_gm + n;
    int16_J1 = (int16_t *)(uint32_igm + n);
    int16_J2 = int16_J1 + n;
    int16_s1 = int16_J2 + n;
    int16_s2 = int16_s1 + n;
    uint16_s1 = (uint16_t *)(int16_s2 + n);
    uint16_s2 = uint16_s1 + n;
    uint16_I1 = uint16_s2 + n;
    uint16_I2 = uint16_I1 + n;
    uint16_J1 = uint16_I2 + n;
    uint16_J2 = uint16_J1 + n;
    uint16_i1 = uint16_J2 + n;
    uint16_i2 = uint16_i1 + n;
    uint16_tmp1 = uint16_i2 + n;
    uint16_tmp2 = uint16_tmp1 + n;
    
    p1 = primes[0].p;
    p01 = modp_ninv31(p1);
    R21 = modp_R2(p1, p01);
    modp_mkgm2(uint32_gm, uint32_igm, logn, primes[0].g, p1, p01);
    inv_q_p = modp_montymul(inv_q_p, R21, p1, p01);
    
    //prepare hm and -F
    for (i = 0; i < n; i ++) {
        uint32_i1[i] = (uint32_t)hm[i];
        uint16_i1[i] = hm[i];
        uint32_i2[i] = modp_set(-((uint32_t)F[i]), p1);//input이 -p/2<x<p/2 이기 때문에 modp_set을 통해서 반드시 0<x<p안에 들어오게 해주어야 NTT/INTT가 정상 동작함
        uint16_i2[i] = mq_conv_to_positive((int16_t)(-F[i]));
    }

    //calculate NTT of hm and -F
    modp_NTT2_ext(uint32_i1, 1, uint32_gm, logn, p1, p01);
    mq_NTT(uint16_i1, logn);
    modp_NTT2_ext(uint32_i2, 1, uint32_gm, logn, p1, p01);
    mq_NTT(uint16_i2, logn);
    
    //transform NTT of hm to the montgomery form and multiply the NTT of -F ->Not montgomery expression
    for (i = 0; i < n; i ++) {
        uint32_i1[i] = modp_montymul(uint32_i1[i], R21, p1, p01);
        uint16_i1[i] = mq_montymul_16(uint16_i1[i], QR2);
        uint32_i2[i] = modp_montymul(uint32_i1[i], uint32_i2[i], p1, p01);
        uint16_i2[i] = mq_montymul_16(uint16_i1[i], uint16_i2[i]);
    }
    
    //calculate hm*(-F) from INTT
    modp_iNTT2_ext(uint32_i2, 1, uint32_igm, logn, p1, p01);
    mq_iNTT(uint16_i2, logn);
    
    for(i = 0; i < n; i++)
    {
        uint32_i2[i] = modp_add(uint32_i2[i], hq, p1);
        uint16_i2[i] = mq_add_16(uint16_i2[i], (uint16_t)hq);
        uint32_i2[i] = modp_sub(uint32_i2[i], (uint32_t)uint16_i2[i], p1);
        uint32_i2[i] = modp_montymul(uint32_i2[i], inv_q_p, p1, p01);
        uint16_I1[i] = mq_conv_to_positive((uint16_t)modp_norm(uint32_i2[i], p1));
    }
    //-----------------until here calculation of I1 not NTT form----------------------------
    
    //prepare f
    for (i = 0; i < n; i ++) {
        uint32_i2[i] = modp_set((uint32_t)f[i], p1);//input이 -p/2<x<p/2 이기 때문에 modp_set을 통해서 반드시 0<x<p안에 들어오게 해주어야 NTT/INTT가 정상 동작함
        uint16_i2[i] = mq_conv_to_positive((int16_t)f[i]);
    }

    //save the NTT of f
    modp_NTT2_ext(uint32_i2, 1, uint32_gm, logn, p1, p01);
    mq_NTT(uint16_i2, logn);
    
    //multifly the NTT of f to the montgomery NTT form of hm
    for (i = 0; i < n; i ++) {
        uint32_i2[i] = modp_montymul(uint32_i1[i], uint32_i2[i], p1, p01);
        uint16_i2[i] = mq_montymul_16(uint16_i1[i], uint16_i2[i]);
    }
    
    //calculate hm*f from INTT
    modp_iNTT2_ext(uint32_i2, 1, uint32_igm, logn, p1, p01);
    mq_iNTT(uint16_i2, logn);
    
    for(i = 0; i < n; i++)
    {
        uint32_i2[i] = modp_add(uint32_i2[i], hq, p1);
        uint16_i2[i] = mq_add_16(uint16_i2[i], (uint16_t)hq);
        uint32_i2[i] = modp_sub(uint32_i2[i], (uint32_t)uint16_i2[i], p1);
        uint32_i2[i] = modp_montymul(uint32_i2[i], inv_q_p, p1, p01);
        uint16_I2[i] = mq_conv_to_positive((uint16_t)modp_norm(uint32_i2[i], p1));
        
        uint16_i1[i] = mq_montymul_16(uint16_i1[i], 1);//For generation of s1 later
    }
    //------------------until here calculation of I2 not NTT form--------------------------
    
    //for(int j = 0; j < 1; j++)
    while(check)
    {
        random_CBD3(rng, int16_J1, logn);

        for(i = 0; i < n; i++)
        {
            int16_J2[i] = 0;
        }
        
        for (i = 0; i < n; i ++) {
            uint16_J1[i] = mq_conv_to_positive(int16_J1[i]);
            uint16_J2[i] = mq_conv_to_positive(int16_J2[i]);
            uint16_J1[i] = mq_add_16(uint16_I1[i], uint16_J1[i]);
            uint16_J2[i] = mq_add_16(uint16_I2[i], uint16_J2[i]);
            uint16_tmp1[i] = mq_conv_to_positive((int16_t)f[i]);
            uint16_tmp2[i] = mq_conv_to_positive((int16_t)F[i]);
        }
        mq_NTT(uint16_J1, logn);
        mq_NTT(uint16_J2, logn);
        mq_NTT(uint16_tmp1, logn);
        mq_NTT(uint16_tmp2, logn);

        for(i = 0; i < n; i++)
        {
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], QR2);
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], uint16_J1[i]);
            uint16_tmp2[i] = mq_montymul_16(uint16_tmp2[i], QR2);
            uint16_tmp2[i] = mq_montymul_16(uint16_tmp2[i], uint16_J2[i]);
            uint16_s2[i] = mq_add_16(uint16_tmp1[i], uint16_tmp2[i]);
            
            uint16_i2[i] = h_src[i];
        }
        
        mq_NTT(uint16_i2, logn);

        for(i=0; i < n; i++)
        {
            uint16_i2[i] = mq_montymul_16(uint16_i2[i], QR2);
            uint16_i2[i] = mq_montymul_16(uint16_i2[i], uint16_s2[i]);
            uint16_s1[i] = mq_sub_16(uint16_i1[i], uint16_i2[i]);
        }

        mq_iNTT(uint16_s2, logn);
        mq_iNTT(uint16_s1, logn);

        for(i = 0; i < n; i++)
        {
            int16_s2[i] = mq_conv_to_center(uint16_s2[i]);
            int16_s1[i] = mq_conv_to_center(uint16_s1[i]);
        }

        //Until here, calculation of s2 and s1----------------------------------------------------
    
        /*for (i = 0; i < n; i ++) {
            uint16_s1[i] = hm[i];
            uint16_tmp1[i] = mq_conv_to_positive((int16_t)g[i]);
            uint16_tmp2[i] = mq_conv_to_positive((int16_t)G[i]);
        }
    
        mq_NTT(uint16_s1, logn);
        mq_NTT(uint16_tmp1, logn);//g
        mq_NTT(uint16_tmp2, logn);//G
    
        
    
        for(i = 0; i < n; i++)
        {
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], QR2);
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], uint16_I1[i]);
            uint16_tmp2[i] = mq_montymul_16(uint16_tmp2[i], QR2);
            uint16_tmp2[i] = mq_montymul_16(uint16_tmp2[i], uint16_I2[i]);
            uint16_tmp1[i] = mq_add_16(uint16_tmp1[i], uint16_tmp2[i]);
            uint16_s1[i] = mq_sub_16(uint16_s1[i], uint16_tmp1[i]);
        }

        mq_iNTT(uint16_s1, logn);

        for(i = 0; i < n; i++)
        {
            int16_s1[i] = mq_conv_to_center(uint16_s1[i]);
        }*/

        //Until here, another method of calculation of s1--------------------------------s--------
        
        //Test to verify that s1+h*s2=c
        //Use uint16_s1 and uint16_s2
        /*mq_NTT(uint16_s1, logn);
        mq_NTT(uint16_s2, logn);
        for(i = 0; i < n; i++)
        {
            uint16_tmp1[i] = h_src[i];
        }
        mq_NTT(uint16_tmp1, logn);
    
        for(i=0; i < n; i++)
        {
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], QR2);
            uint16_tmp1[i] = mq_montymul_16(uint16_tmp1[i], uint16_s2[i]);
            uint16_tmp1[i] = mq_add_16(uint16_tmp1[i], uint16_s1[i]);
            uint16_tmp2[i] = hm[i];
        }
        mq_iNTT(uint16_tmp1,logn);
        for(i = 0; i < n; i++)
        {
            if(uint16_tmp1[i] != uint16_tmp2[i]) printf("different\n");
        }*/
        //Until here, verification of s1+h*s2=c
        
        //When signature is short enough, the return value of Zf(is_short) is one.
        //check = Zf(is_short)(int16_s1, int16_s2, logn);
        //printf("%d : check[%d]\n", j, check);
        if(Zf(is_short)(int16_s1, int16_s2, logn))
        {
            //s2 = int16_s2;
            for(i = 0; i < n; i++)
            {
                s2[i] = int16_s2[i];
            }
            check = 0;
            //printf("Signature is generated\n");
            //printf("Signature is generated[%d]\n", j);
            //break;
        }
    }
}

void
Zf(sign_dyn_peregrine)(int16_t *sig, inner_shake256_context *rng,
    const int8_t *restrict f, const int8_t *restrict g,
    const int8_t *restrict F, const int8_t *restrict G,
    const uint16_t *hm, const uint16_t *h_src, unsigned logn, uint8_t *tmp)
{
    do_sign_dyn_peregrine(sig, rng, f, g, F, G, hm, h_src, logn, tmp);
}
//---------------------------------------------------------------------


