CC ?=/usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -O3 -fomit-frame-pointer
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer
RM = /bin/rm

LDFLAGS=

SOURCES= kem.c poly.c ntt.c aes256ctr.c sha256.c sha512.c symmetric.c reduce.c verify.c aes.c
HEADERS= params.h kem.h poly.h ntt.h sha2.h symmetric.h reduce.h verify.h aes.h

.PHONY: all PQCgenKAT_kem test clean

all: \
	test \
	PQCgenKAT_kem

PQCgenKAT_kem: $(HEADERS) rng.h $(SOURCES) rng.c
	$(CC) $(NISTFLAGS) -o $@ PQCgenKAT_kem.c rng.c $(SOURCES) $(LDFLAGS)

test: $(HEADERS) randombytes.h $(SOURCES) randombytes.c test.c cpucycles.h
	$(CC) $(CFLAGS) -o $@ randombytes.c test.c $(SOURCES)

PQC_bench: ./PQC_bench.c
	$(CC) -mtune=native -O3 -fomit-frame-pointer -o $@ ./PQC_bench.c rng.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQC_bench

clean:
	-$(RM) PQCgenKAT_kem
	-$(RM) test

clean_bench:
	rm PQC_bench