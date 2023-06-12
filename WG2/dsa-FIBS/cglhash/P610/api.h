/********************************************************************************************
* CGLHash Library
*
* Abstract: API header file for P610
*********************************************************************************************/  

#ifndef P610_API_H
#define P610_API_H
    

// Algorithm name
#define CRYPTO_ALGNAME "CGLp610"  


/*********************** CGL Hash function API ***********************/

#define CGL_MESSAGEBYTES    39
#define CGL_BYTES     462
#define CGL_HASHBYTES         154 


void random_mod_order_A(unsigned char* random_digits);
int ecisog_run();
int CGLhash(const unsigned char* MessageA, unsigned char* DigestA, int inlen);

#endif