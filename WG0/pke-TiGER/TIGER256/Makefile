CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99
CFLAGS2=-O0

all :
	$(CC) $(CFLAGS) -c TIGER_CCAKEM.c TIGER_CPAPKE.c main.c fips202.c xe5.c rng.c aes.c
	$(CC) $(CFLAGS) -o test_CCAKEM TIGER_CCAKEM.o TIGER_CPAPKE.o main.o fips202.o xe5.o rng.o aes.o
	
run : all
	./test_CCAKEM

clean :
	rm -f *.o
	rm -f test_CCAKEM

new :
	make clean
	make all
	./test_CCAKEM

PQC_bench: ./PQC_bench.c
	$(CC) -O2 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99 -o $@ ./PQC_bench.c TIGER_CCAKEM.c TIGER_CPAPKE.c fips202.c xe5.c rng.c aes.c $(LDFLAGS)
	./PQC_bench

Module_bench: ./Module_bench.c
	$(CC) -O2 -fomit-frame-pointer -msse2avx -mavx2 -march=native -std=c99 -o $@ ./Module_bench.c TIGER_CCAKEM.c TIGER_CPAPKE.c fips202.c xe5.c rng.c aes.c $(LDFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench