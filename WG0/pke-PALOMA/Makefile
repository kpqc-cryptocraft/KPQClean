INCDIR := include

CC = gcc

SOURCES= gf_table_gen.c gf.c gfpoly.c goppa_instance.c keygen.c mat_mul.c common.c lsh.c lsh512.c encap.c decap.c PALOMA.c rng.c aes.c 
HEADERS= ./$(INCDIR)/*.h

SOURCE_KAT = KAT.c $(SOURCES)
SOURCE_BENCH = bench.c $(SOURCES)
HEADER = -I./$(INCDIR)

CFLAGS = -march=native -mtune=native -O3 -fomit-frame-pointer
DFLAGS = -g3 -DDEBUG

ALL:
	ulimit -s 51200
	$(CC) $(CFLAGS) -c $(SOURCE_KAT) $(HEADER)
	$(CC) $(CFLAGS) *.o -o PQCgenKAT_kem

bench:
	ulimit -s 51200
	$(CC) $(CFLAGS) -c $(SOURCE_BENCH) $(HEADER)
	$(CC) $(CFLAGS) *.o -o bench

clean:
	rm *.o
	rm PQCgenKAT_kem
	rm *.req *.rsp

clean_banch:
	rm *.o
	rm bench

PQC_bench:
	ulimit -s 51200
	#./PQC_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./PQC_bench.c $(SOURCES) $(HEADER) $(LDFLAGS)
	./PQC_bench

Module_bench:
	ulimit -s 51200
	#./Module_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./Module_bench.c $(SOURCES) $(HEADER) $(LDFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench