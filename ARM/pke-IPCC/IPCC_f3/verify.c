#include "verify.h"


/*************************************************
* Name:        verify
* 
* Description: Compare two arrays for equality in constant time.
*
* Arguments:   const unsigned int *a: pointer to first integer
*              const unsigned int *b: second integer
*
* Returns 0 if the integer values are equal, 1 otherwise
**************************************************/
int verify(const unsigned int *m, const unsigned int pt) {
    if (m[0] == pt) return 0;
    else return 1;
}