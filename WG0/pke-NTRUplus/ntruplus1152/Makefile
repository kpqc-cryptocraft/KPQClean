CC=/usr/bin/gcc
CFLAGS += -O3 -march=native -fomit-frame-pointer
LDFLAGS=

SOURCES= aes256ctr.c kem.c ntt.c poly.c reduce.c rng.c sha512.c symmetric.c util.c verify.c sha256.c aes.c
HEADERS= api.h crypto_stream.h kem.h ntt.h params.h poly.h reduce.h rng.h sha2.h symmetric.h util.h verify.h aes.h

.PHONY: all PQCgenKAT_kem test clean

all: test PQCgenKAT_kem

PQCgenKAT_kem: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -o $@ PQCgenKAT_kem.c $(SOURCES) $(LDFLAGS)

test: $(HEADERS) $(SOURCES) test.c
	$(CC) $(CFLAGS) -o $@ test.c $(SOURCES) $(LDFLAGS)

clean:
	-rm PQCgenKAT_kem
	-rm test


PQC_bench: ./PQC_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./PQC_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQC_bench

Module_bench: ./Module_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./Module_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./Module_bench


clean_bench:
	rm PQC_bench