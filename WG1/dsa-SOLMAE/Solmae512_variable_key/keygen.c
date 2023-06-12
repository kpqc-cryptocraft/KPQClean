#include "normaldist.h"
#include "api.h"
#include "randombytes.h"
#include <math.h>
#include <x86intrin.h>
#include <string.h>
#include <stdio.h>

static void compute_GSO(secret_key *sk)
{
    poly temp1, temp2, temp3;

    for (int i = 0; i < SOLMAE_D; ++i)
    {
        (sk->GSO_b10).coeffs[i].v = (sk->b10).coeffs[i].v;
        (sk->GSO_b11).coeffs[i].v = (sk->b11).coeffs[i].v;
        (sk->GSO_b20).coeffs[i].v = (sk->b20).coeffs[i].v;
        (sk->GSO_b21).coeffs[i].v = (sk->b21).coeffs[i].v;
    }

    set_poly(&temp1, &(sk->b20));
    set_poly(&temp2, &(sk->b21));

    FFT_mul_adj(&temp1, &(sk->b10));
    FFT_mul_adj(&temp2, &(sk->b11));
    poly_add(&temp1, &temp2); // temp1 = <b1, b2>

    set_poly(&temp2, &(sk->b10));
    set_poly(&temp3, &(sk->b11));

    FFT_mul_selfadj(&temp2);
    FFT_mul_selfadj(&temp3);
    poly_add(&temp2, &temp3); // temp2 = <b1, b1>

    poly_div_FFT(&temp1, &temp2);
    set_poly(&temp2, &temp1); // temp2 = temp1 = <b1, b2>/<b1, b1>

    pointwise_mul(&temp1, &(sk->GSO_b10));
    pointwise_mul(&temp2, &(sk->GSO_b11)); // [temp1,temp2] = (<b1, b2>/<b1, b1>) * ~b1

    poly_sub(&(sk->GSO_b20), &temp1);
    poly_sub(&(sk->GSO_b21), &temp2);
}

static void compute_sigma(secret_key *sk)
{
    poly temp1, temp2, poly_r_square;

    for (int i = 0; i < SOLMAE_D / 2; ++i)
    {
        (sk->sigma1).coeffs[i].v = SIGMA_SQUARE;
        (sk->sigma2).coeffs[i].v = SIGMA_SQUARE;
        poly_r_square.coeffs[i].v = R_SQUARE;
    }
    for (int i = SOLMAE_D / 2; i < SOLMAE_D; ++i)
    {
        poly_r_square.coeffs[i].v = 0;
        (sk->sigma1).coeffs[i].v = 0;
        (sk->sigma2).coeffs[i].v = 0;
    }

    set_poly(&temp1, &(sk->GSO_b10));
    set_poly(&temp2, &(sk->GSO_b11));

    FFT_mul_selfadj(&temp1);
    FFT_mul_selfadj(&temp2);
    poly_add(&temp1, &temp2);

    poly_div_FFT(&(sk->sigma1), &temp1);
    poly_sub(&(sk->sigma1), &poly_r_square);

    for (int i = 0; i < SOLMAE_D; ++i)
        (sk->sigma1).coeffs[i].v = sqrt((sk->sigma1).coeffs[i].v);

    set_poly(&temp1, &(sk->GSO_b20));
    set_poly(&temp2, &(sk->GSO_b21));

    FFT_mul_selfadj(&temp1);
    FFT_mul_selfadj(&temp2);
    poly_add(&temp1, &temp2);
    poly_div_FFT(&(sk->sigma2), &temp1);
    poly_sub(&(sk->sigma2), &poly_r_square);

    for (int i = 0; i < SOLMAE_D; ++i)
        (sk->sigma2).coeffs[i].v = sqrt((sk->sigma2).coeffs[i].v);
}

static void compute_beta_hat(secret_key *sk)
{
    poly temp1, temp2;
    // beta1
    set_poly(&temp1, &(sk->GSO_b10));
    set_poly(&temp2, &(sk->GSO_b11));

    FFT_mul_selfadj(&temp1);
    FFT_mul_selfadj(&temp2);
    poly_add(&temp1, &temp2);

    set_poly(&(sk->beta10), &(sk->GSO_b10));
    set_poly(&(sk->beta11), &(sk->GSO_b11));
    FFT_adj(&(sk->beta10));
    FFT_adj(&(sk->beta11));
    poly_div_FFT(&(sk->beta10), &temp1);
    poly_div_FFT(&(sk->beta11), &temp1);

    // beta2
    set_poly(&temp1, &(sk->GSO_b20));
    set_poly(&temp2, &(sk->GSO_b21));

    FFT_mul_selfadj(&temp1);
    FFT_mul_selfadj(&temp2);
    poly_add(&temp1, &temp2);

    set_poly(&(sk->beta20), &(sk->GSO_b20));
    set_poly(&(sk->beta21), &(sk->GSO_b21));
    FFT_adj(&(sk->beta20));
    FFT_adj(&(sk->beta21));
    poly_div_FFT(&(sk->beta20), &temp1);
    poly_div_FFT(&(sk->beta21), &temp1);
}

static void simple_frand(double *r, uint64_t *buf, size_t n)
{
    static const double pow2m64 = pow(2, -64);
    randombytes((uint8_t *)buf, n * sizeof(uint64_t));
    for (size_t i = 0; i < n; i++)
    {
        r[i] = ((double)buf[i]) * pow2m64;
    }
}

int keygen_fg(secret_key *sk)
{
    double z[SOLMAE_D / 2], af[SOLMAE_D / 2], ag[SOLMAE_D / 2],
        f[SOLMAE_D], g[SOLMAE_D];
    const double alow = 1 / SOLMAE_ALPHA + SOLMAE_ALPHAEPS,
                 ahigh = SOLMAE_ALPHA - SOLMAE_ALPHAEPS,
                 qlow = ((double)SOLMAE_Q) * alow * alow,
                 qhigh = ((double)SOLMAE_Q) * ahigh * ahigh,
                 qlow2 = ((double)SOLMAE_Q) / (SOLMAE_ALPHA * SOLMAE_ALPHA),
                 qhigh2 = ((double)SOLMAE_Q) * SOLMAE_ALPHA * SOLMAE_ALPHA;

    double r[2 * SOLMAE_D];
    uint64_t rint[2 * SOLMAE_D];

    int trials = 0, check;

    do
    {
        trials++;
        simple_frand(r, rint, 2 * SOLMAE_D);

        for (size_t i = 0; i < SOLMAE_D / 2; i++)
        {
            z[i] = sqrt(qlow + (qhigh - qlow) * r[i]);

            af[i] = z[i] * cos(M_PI / 2 * r[i + SOLMAE_D / 2]);
            ag[i] = z[i] * sin(M_PI / 2 * r[i + SOLMAE_D / 2]);
            f[i] = af[i] * cos(2 * M_PI * r[i + 2 * SOLMAE_D / 2]);
            f[i + SOLMAE_D / 2] = af[i] * sin(2 * M_PI * r[i + 2 * SOLMAE_D / 2]);
            g[i] = ag[i] * cos(2 * M_PI * r[i + 3 * SOLMAE_D / 2]);
            g[i + SOLMAE_D / 2] = ag[i] * sin(2 * M_PI * r[i + 3 * SOLMAE_D / 2]);
        }
        for (size_t i = 0; i < SOLMAE_D; i++)
        {
            sk->b10.coeffs[i].v = f[i];
            sk->b11.coeffs[i].v = g[i];
        }
        invFFT(&sk->b10);
        invFFT(&sk->b11);
        for (size_t i = 0; i < SOLMAE_D; i++)
        {
            sk->f[i] = lrint(sk->b10.coeffs[i].v);
            sk->g[i] = lrint(sk->b11.coeffs[i].v);
            sk->b10.coeffs[i].v = (double)sk->f[i];
            sk->b11.coeffs[i].v = (double)sk->g[i];
        }
        /*
        printf("\n\n*** f = ");
        print_poly(b10);
        printf("*** g = ");
        print_poly(b11);
        printf("\n\n");
        */
        FFT(&sk->b10);
        FFT(&sk->b11);
        check = 0;
        // double zmin = INFINITY, zmax = 0.;
        for (size_t i = 0; i < SOLMAE_D / 2; i++)
        {
            double zi =
                fpr_sqr(sk->b10.coeffs[i]).v +
                fpr_sqr(sk->b10.coeffs[i + SOLMAE_D / 2]).v +
                fpr_sqr(sk->b11.coeffs[i]).v +
                fpr_sqr(sk->b11.coeffs[i + SOLMAE_D / 2]).v;
            /*
            if(zi < zmin)
            zmin = zi;
            if(zi > zmax)
            zmax = zi;
            */
            if (zi < qlow2 || zi > qhigh2)
            {
                check = 1;
                break;
            }
        }
        // printf("zmin = %f (qlow2  = %f)\n", zmin, qlow2);
        // printf("zmax = %f (qhigh2 = %f)\n", zmax, qhigh2);
    } while (check);
    return trials;
}

int keygen_full(secret_key *sk, public_key *pk)
{
    int trials = 0, lim;
    uint8_t tmp[32 * SOLMAE_D];

    lim = (1 << (Zf(max_FG_bits)[SOLMAE_LOG_D] - 1)) - 1;
    while (1)
    {
        trials += keygen_fg(sk);
        if (!Zf(compute_public)(pk->hint, sk->f, sk->g, SOLMAE_LOG_D, tmp))
            continue;

        if (!Zf(solve_NTRU)(SOLMAE_LOG_D, sk->F, sk->G, sk->f, sk->g,
                            lim, (uint32_t *)tmp))
            continue;

        break;
    }

    for (int i = 0; i < SOLMAE_D; i++)
    {
        sk->b20.coeffs[i].v = (double)sk->F[i];
        sk->b21.coeffs[i].v = (double)sk->G[i];
    }
    FFT(&sk->b20);
    FFT(&sk->b21);

    for (int i = 0; i < SOLMAE_D; i++)
    {
        pk->h.coeffs[i].v = (double)pk->hint[i];
    }
    FFT(&pk->h);

    compute_GSO(sk);
    compute_sigma(sk);
    compute_beta_hat(sk);

    return trials;
}
