CC		= gcc
LDFLAGS=
LDFLAGS+=
CPPFLAGS=

SOURCES= PQCgenKAT_pke.c pke.c Keygen.c Encryption.c MultiEnc.c Decryption.c rng.c verify.c fips202.c aes.c
HEADERS= api.h Keygen.h Encryption.h MultiEnc.h Decryption.h params.h rng.h verify.h fips202.h aes.h

SOURCES2= PQC_bench.c pke.c Keygen.c Encryption.c MultiEnc.c Decryption.c rng.c verify.c fips202.c aes.c
HEADERS2= api.h Keygen.h Encryption.h MultiEnc.h Decryption.h params.h rng.h verify.h fips202.h aes.h


PQCgenKAT_pke: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS) $(CPPFLAGS)

clean:
	@rm -rf. *.o PQCgenKAT_pke

PQC_bench: $(HEADERS2) $(SOURCES2)
	$(CC) -O3 -o $@ $(SOURCES2) $(LDFLAGS) $(CPPFLAGS)
	./PQC_bench

clean_bench:
	rm PQC_bench