#ifndef CONFIG_H
#define CONFIG_H

#ifndef NIMS_MODE
//mode change
#define NIMS_MODE 5
#if NIMS_MODE==2
#define CRYPTO_ALGNAME "NIMS_1c"
#elif NIMS_MODE==3
#define CRYPTO_ALGNAME "NIMS_2c"
#elif NIMS_MODE==5
#define CRYPTO_ALGNAME "NIMS_3c"
#endif
#endif


#endif
