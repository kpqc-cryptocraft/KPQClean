CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

all :
	$(CC) $(CFLAGS) -c TiGER_CCAKEM.c TiGER_CPAPKE.c main.c fips202.c xef_ref.c rng.c aes.c util.c
	$(CC) $(CFLAGS) -o test_CCAKEM TiGER_CCAKEM.o TiGER_CPAPKE.o main.o fips202.o xef_ref.o rng.o aes.o util.o
	
run : all
	./test_CCAKEM

clean :
	rm -f *.o
	rm -f test_CCAKEM
	rm PQC_bench

new :
	make clean
	make all
	./test_CCAKEM

PQC_bench: ./PQC_bench.c
	$(CC) -O2 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99 -o $@ ./PQC_bench.c TiGER_CCAKEM.c TiGER_CPAPKE.c fips202.c xef_ref.c rng.c aes.c util.c $(LDFLAGS)
	./PQC_bench

Module_bench: ./Module_bench.c
	$(CC) -O2 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99 -o $@ ./Module_bench.c TiGER_CCAKEM.c TiGER_CPAPKE.c fips202.c xef_ref.c rng.c aes.c util.c $(LDFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench
