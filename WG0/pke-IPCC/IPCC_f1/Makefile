CC		= gcc
LDFLAGS=
LDFLAGS+=
CPPFLAGS=

SOURCES= PQCgenKAT_pke.c pke.c Keygen.c Encryption.c MultiEnc.c Decryption.c rng.c verify.c fips202.c aes.c
HEADERS= api.h Keygen.h Encryption.h MultiEnc.h Decryption.h params.h rng.h verify.h fips202.h aes.h


PQCgenKAT_pke: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS) $(CPPFLAGS)

clean:
	@rm -rf *.o PQCgenKAT_pke

PQC_bench: ./PQC_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./PQC_bench.c $(HEADERS) pke.c Keygen.c Encryption.c MultiEnc.c Decryption.c rng.c verify.c fips202.c aes.c $(LDFLAGS)
	./PQC_bench

Module_bench: ./Module_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./Module_bench.c $(HEADERS) pke.c Keygen.c Encryption.c MultiEnc.c Decryption.c rng.c verify.c fips202.c aes.c $(LDFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench