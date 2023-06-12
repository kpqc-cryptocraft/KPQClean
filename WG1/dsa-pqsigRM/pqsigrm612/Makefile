LDFLAGS =  
CFLAGS = 
LIBFLAGS = -lm
CC = gcc
CFILES := $(shell find . -name '*.c' | sed -e 's/\.c/\.o/')

SOURCES = ./src/aes.c ./src/common.c ./src/fips202.c ./src/keypair.c ./src/matrix.c ./src/nearest_vector.c ./src/open.c ./src/rm.c ./src/rng.c ./src/sha2.c ./src/sign.c

OBJS = ${CFILES}

sign: ${OBJS}
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^  $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

rng.o : rng.h
api.o : api.h

clean:
	cd src; rm -f *.o; cd ..
	rm -f *.o
	rm -f sign runtime


PQC_bench: ./src/*.c ./PQC_bench.c
	$(CC) $(LDFLAGS) $(CFLAGS) -O2 -o $@ PQC_bench.c $(SOURCES) $(LIBFLAGS)
	./PQC_bench

clean_bench:
	rm PQC_bench