CC = gcc
CFLAGS =-O3 
CFLAGS1 =-O3 -fomit-frame-pointer -msse2avx -mavx2 -std=c99
CFLAGS2=-O0

all :
	$(CC) $(CFLAGS) -c TiGER_CCAKEM.c TiGER_CPAPKE.c main.c fips202.c xe5.c rng.c aes.c util.c
	$(CC) $(CFLAGS) -o test_CCAKEM TiGER_CCAKEM.o TiGER_CPAPKE.o main.o fips202.o xe5.o rng.o aes.o util.o
	
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
	$(CC) -O3 -fomit-frame-pointer -mavx2 -std=c99 -o $@ ./PQC_bench.c TiGER_CCAKEM.c TiGER_CPAPKE.c fips202.c xe5.c rng.c aes.c util.c $(LDFLAGS)
	./PQC_bench

clean_bench:
	rm PQC_bench