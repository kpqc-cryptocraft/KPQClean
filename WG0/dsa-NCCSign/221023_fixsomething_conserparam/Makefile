CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wvla -Wpointer-arith -O3 -march=native -mtune=native -std=c99
NISTFLAGS += -Wno-unused-result -O3 -std=c99
SOURCES = sign.c packing.c poly.c reduce.c rounding.c aes.c
HEADERS = config.h params.h api.h sign.h packing.h poly.h \
  reduce.h rounding.h symmetric.h randombytes.h aes.h
KECCAK_SOURCES = $(SOURCES) fips202.c symmetric-shake.c
KECCAK_HEADERS = $(HEADERS) fips202.h
AES_SOURCES = $(SOURCES) fips202.c aes256ctr.c symmetric-aes.c 
AES_HEADERS = $(HEADERS) fips202.h aes256ctr.h 

.PHONY: all speed shared clean

all: PQCgenKAT_sign

fixtest: \
  test/test_NIMS2_fix \
  test/test_NIMS3_fix \
  test/test_NIMS5_fix \


test: \
  test/test_NIMS2 \
  test/test_NIMS3 \
  test/test_NIMS5 \
  

speed: \
  test/test_speed2 \
  test/test_speed3 \
  test/test_speed5 \
  test/test_speed2aes \
  test/test_speed3aes \
  test/test_speed5aes

shared: \
  libpqcrystals_NIMS2_ref.so \
  libpqcrystals_NIMS3_ref.so \
  libpqcrystals_NIMS5_ref.so \
  libpqcrystals_NIMS2aes_ref.so \
  libpqcrystals_NIMS3aes_ref.so \
  libpqcrystals_NIMS5aes_ref.so \
  libpqcrystals_fips202_ref.so \
  libpqcrystals_aes256ctr_ref.so

libpqcrystals_fips202_ref.so: fips202.c fips202.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

libpqcrystals_aes256ctr_ref.so: aes256ctr.c aes256ctr.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<

libpqcrystals_NIMS2_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=2 \
	  -o $@ $(SOURCES) symmetric-shake.c

libpqcrystals_NIMS3_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=3 \
	  -o $@ $(SOURCES) symmetric-shake.c

libpqcrystals_NIMS5_ref.so: $(SOURCES) $(HEADERS) symmetric-shake.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=5 \
	  -o $@ $(SOURCES) symmetric-shake.c

libpqcrystals_NIMS2aes_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=2 -DNIMS_USE_AES \
	   -o $@ $(SOURCES) symmetric-aes.c

libpqcrystals_NIMS3aes_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=3 -DNIMS_USE_AES \
	   -o $@ $(SOURCES) symmetric-aes.c

libpqcrystals_NIMS5aes_ref.so: $(SOURCES) $(HEADERS) symmetric-aes.c
	$(CC) -shared -fPIC $(CFLAGS) -DNIMS_MODE=5 -DNIMS_USE_AES \
	   -o $@ $(SOURCES) symmetric-aes.c

test/test_NIMS2: test/test_NIMS.c randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 \
	  -o $@ $< randombytes.c $(KECCAK_SOURCES)

test/test_NIMS3: test/test_NIMS.c randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 \
	  -o $@ $< randombytes.c $(KECCAK_SOURCES)

test/test_NIMS5: test/test_NIMS.c randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 \
	  -o $@ $< randombytes.c $(KECCAK_SOURCES)

test/test_NIMS2aes: test/test_NIMS.c randombytes.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 -DNIMS_USE_AES \
	  -o $@ $< randombytes.c $(AES_SOURCES)

test/test_NIMS3aes: test/test_NIMS.c randombytes.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 -DNIMS_USE_AES \
	  -o $@ $< randombytes.c $(AES_SOURCES)

test/test_NIMS5aes: test/test_NIMS.c randombytes.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 -DNIMS_USE_AES \
	  -o $@ $< randombytes.c $(AES_SOURCES)

test/test_vectors2: test/test_vectors.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 \
	  -o $@ $< $(KECCAK_SOURCES)

test/test_vectors3: test/test_vectors.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 \
	  -o $@ $< $(KECCAK_SOURCES)

test/test_vectors5: test/test_vectors.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 \
	  -o $@ $< $(KECCAK_SOURCES)

test/test_vectors2aes: test/test_vectors.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 -DNIMS_USE_AES \
	  -o $@ $< $(AES_SOURCES)

test/test_vectors3aes: test/test_vectors.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 -DNIMS_USE_AES \
	  -o $@ $< $(AES_SOURCES)

test/test_vectors5aes: test/test_vectors.c $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=5 -DNIMS_USE_AES \
	  -o $@ $< $(AES_SOURCES)

test/test_speed2: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(KECCAK_SOURCES)

test/test_speed3: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(KECCAK_SOURCES)

test/test_speed5: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(KECCAK_SOURCES)

test/test_speed2aes: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(AES_SOURCES) $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 -DNIMS_USE_AES \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(AES_SOURCES)

test/test_speed3aes: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(AES_SOURCES) $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 -DNIMS_USE_AES \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(AES_SOURCES)

test/test_speed5aes: test/test_speed.c test/speed_print.c test/speed_print.h \
  test/cpucycles.c test/cpucycles.h randombytes.c $(AES_SOURCES) $(AES_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 -DNIMS_USE_AES \
	  -o $@ $< test/speed_print.c test/cpucycles.c randombytes.c \
	  $(AES_SOURCES)

test/test_NIMS2_fix: test/test_NIMS_fixinput.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=2 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

test/test_NIMS3_fix: test/test_NIMS_fixinput.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=3 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

test/test_NIMS5_fix: test/test_NIMS_fixinput.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(CFLAGS) -DNIMS_MODE=5 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

PQCgenKAT_sign: PQCgenKAT_sign.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(NISTFLAGS) -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

PQCgenKAT_sign2: PQCgenKAT_sign.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=2 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

PQCgenKAT_sign3: PQCgenKAT_sign.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=3 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

PQCgenKAT_sign5: PQCgenKAT_sign.c rng.c rng.h $(KECCAK_SOURCES) \
  $(KECCAK_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=5 \
	  -o $@ $< rng.c $(KECCAK_SOURCES) $(LDFLAGS)

PQCgenKAT_sign2aes: PQCgenKAT_sign.c rng.c rng.h $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=2 -DNIMS_USE_AES \
	  -o $@ $< rng.c $(AES_SOURCES) $(LDFLAGS)

PQCgenKAT_sign3aes: PQCgenKAT_sign.c rng.c rng.h $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=3 -DNIMS_USE_AES \
	  -o $@ $< rng.c $(AES_SOURCES) $(LDFLAGS)

PQCgenKAT_sign5aes: PQCgenKAT_sign.c rng.c rng.h $(AES_SOURCES) \
  $(AES_HEADERS)
	$(CC) $(NISTFLAGS) -DNIMS_MODE=5 -DNIMS_USE_AES \
	  -o $@ $< rng.c $(AES_SOURCES) $(LDFLAGS)

clean:
	rm -f *~ test/*~ *.gcno *.gcda *.lcov
	rm -f libpqcrystals_NIMS2_ref.so
	rm -f libpqcrystals_NIMS3_ref.so
	rm -f libpqcrystals_NIMS5_ref.so
	rm -f libpqcrystals_NIMS2aes_ref.so
	rm -f libpqcrystals_NIMS3aes_ref.so
	rm -f libpqcrystals_NIMS5aes_ref.so
	rm -f libpqcrystals_fips202_ref.so
	rm -f libpqcrystals_aes256ctr_ref.so
	rm -f test/test_NIMS2
	rm -f test/test_NIMS3
	rm -f test/test_NIMS5
	rm -f test/test_NIMS2aes
	rm -f test/test_NIMS3aes
	rm -f test/test_NIMS5aes
	rm -f test/test_vectors2
	rm -f test/test_vectors3
	rm -f test/test_vectors5
	rm -f test/test_vectors2aes
	rm -f test/test_vectors3aes
	rm -f test/test_vectors5aes
	rm -f test/test_speed2
	rm -f test/test_speed3
	rm -f test/test_speed5
	rm -f test/test_speed2aes
	rm -f test/test_speed3aes
	rm -f test/test_speed5aes
	rm -f PQCgenKAT_sign2
	rm -f PQCgenKAT_sign3
	rm -f PQCgenKAT_sign5
	rm -f PQCgenKAT_sign2aes
	rm -f PQCgenKAT_sign3aes
	rm -f PQCgenKAT_sign5aes
	rm -f PQCgenKAT_sign
	rm -f test/test_NIMS2_fix
	rm -f test/test_NIMS3_fix
	rm -f test/test_NIMS5_fix

PQC_bench2: ./PQC_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=2 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./PQC_bench2

PQC_bench3: ./PQC_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=3 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./PQC_bench3

PQC_bench5: ./PQC_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=5 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./PQC_bench5

Module_bench2: ./Module_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=2 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./Module_bench2

Module_bench3: ./Module_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=3 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./Module_bench3

Module_bench5: ./Module_bench.c randombytes.c $(KECCAK_SOURCES) $(KECCAK_HEADERS)
	$(CC) -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O2 -march=native -mtune=native -std=c99 \
	-DNIMS_MODE=5 -o $@ $< randombytes.c $(KECCAK_SOURCES)
	./Module_bench5

clean_bench2: 
	rm PQC_bench2

clean_bench3:
	rm PQC_bench3

clean_bench5:
	rm PQC_bench5