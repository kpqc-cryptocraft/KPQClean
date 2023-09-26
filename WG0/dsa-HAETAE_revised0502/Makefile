INCDIR   := inc
SRCDIR   := src

CC=gcc

SOURCES= ./src/aes256ctr.c ./src/aes.c ./src/decompose.c ./src/encoding.c ./src/fft.c ./src/fips202.c ./src/fixpoint.c ./src/ntt.c ./src/packing.c ./src/poly.c ./src/polyfix.c ./src/polymat.c ./src/polyvec.c ./src/randombytes.c ./src/reduce.c ./src/sampler.c ./src/sign.c ./src/symmetric-shake.c
HEADERS= ./inc/aes256ctr.h ./inc/aes.h ./inc/decompose.h ./inc/encoding.h ./inc/fft.h ./inc/fips202.h ./inc/fixpoint.h ./inc/ntt.h ./inc/packing.h ./inc/poly.h ./inc/polyfix.h ./inc/polymat.h ./inc/polyvec.h ./inc/randombytes.h ./inc/rans_byte.h ./inc/reduce.h ./inc/sampler.h ./inc/sign.h ./inc/symmetric.h ./inc/config.h ./inc/params.h 

SOURCE = main.c ./src/*.c
HEADER = -I./$(INCDIR)

CFLAGS  = -march=native -mtune=native -O3 -fomit-frame-pointer
LDFLAGS = -lcrypto -lm
DFLAGS  = -g3 -DDEBUG
	
ALL:
	$(CC) $(CFLAGS) -c  $(SOURCE) $(HEADER)
	$(CC) $(CFLAGS) *.o -o main $(LDFLAGS)
	rm *.o
	./main	

.PHONY: clean

clean:
	rm main



PQC_bench: ./PQC_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./PQC_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQC_bench
	
Module_bench: ./Module_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./Module_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench
	rm *.su