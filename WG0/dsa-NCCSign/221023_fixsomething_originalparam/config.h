#ifndef CONFIG_H
#define CONFIG_H

#ifndef NIMS_MODE
#define NIMS_MODE 5
#if NIMS_MODE==2
#define CRYPTO_ALGNAME "NIMS_1"
#elif NIMS_MODE==3
#define CRYPTO_ALGNAME "NIMS_2"
#elif NIMS_MODE==5
#define CRYPTO_ALGNAME "NIMS_3"
#endif
#endif


#endif
