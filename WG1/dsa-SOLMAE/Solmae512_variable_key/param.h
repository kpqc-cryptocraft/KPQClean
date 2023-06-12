#ifndef PARAM_H
#define PARAM_H

#define SOLMAE_D 512
#define SOLMAE_K 320
#define SOLMAE_Q 12289
#define MSG_BYTES 32
#define R 1.32
#define R_SQUARE 1.7424

/* sigma = r * alpha * sqrt(q) */
/* gamma = slack * sigma * sqrt(2d) */

#if SOLMAE_D == 512
  #define SOLMAE_LOG_D 9
  #define SIGMA_SQUARE 30116.1316
  #define GAMMA_SQUARE 33870790
  #define SOLMAE_ALPHA 1.17
  #define SOLMAE_ALPHAEPS 0.065
#elif SOLMAE_D == 1024
  #define SOLMAE_LOG_D 10
  #define SIGMA_SQUARE 60329.1844
  #define GAMMA_SQUARE 134150669
  #define SOLMAE_ALPHA 1.64
  #define SOLMAE_ALPHAEPS 0.3
#endif


#endif
