#include <stdlib.h>
#include <string.h>

#include "mqs_config.h"
#include "mqs_keypair.h"
#include "mqs.h"

#include "api.h"

#include "utils_hash.h"

#include <rng.h>

#if defined(_SUPERCOP_)
#include "crypto_sign.h"
#endif


int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk, unsigned char *sk_seed)
{
	int r = generate_keypair_mqss((pk_mqs*)pk, (sk_mqss*)sk, sk_seed);

	return r;
}


int
crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk, const uint8_t* sk_seed, uint8_t depth)
{
	unsigned char digest[_HASH_LEN];

	hash_msg( digest , _HASH_LEN , m , mlen );

	memcpy( sm , m , mlen );
	smlen[0] = mlen + _SIGNATURE_BYTE;

	return mqss_sign(sm + mlen, (sk_mqss*)sk, sk_seed, digest, depth);
}



int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,const unsigned char *sm, unsigned long long smlen,const unsigned char *pk)
{
	if( _SIGNATURE_BYTE > smlen ) return -1;

	unsigned char digest[_HASH_LEN];
	hash_msg( digest , _HASH_LEN , sm , smlen-_SIGNATURE_BYTE );

	memcpy( m , sm , smlen-_SIGNATURE_BYTE );
	mlen[0] = smlen-_SIGNATURE_BYTE;
	
	return mqsc_verify(digest, sm + smlen - _SIGNATURE_BYTE, pk);
}

