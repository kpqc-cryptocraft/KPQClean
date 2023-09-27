ifeq ($(RELEASE), 1)
DBG_FLAGS = -O2 -DNDEBUG
else
DBG_FLAGS = -g -O0 -DDEBUG
endif

LDFLAGS =  
CFLAGS = -I/usr/local/include
LIBFLAGS = -lm
CC = gcc
CFILES := $(shell find src -name '*.c' | sed -e 's/\.c/\.o/')

SOURCES = ./src/aes.c ./src/common.c ./src/fips202.c ./src/keypair.c ./src/matrix.c ./src/nearest_vector.c ./src/open.c ./src/rm.c ./src/rng.c ./src/sign.c

OBJS = ${CFILES}

sign: ${OBJS} ./PQCgenKAT_sign.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(DBG_FLAGS) -o $@ $^ $(LIBFLAGS)

runtime: ${OBJS} ./runtime.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(DBG_FLAGS) -o $@ $^ $(LIBFLAGS)

test: ${OBJS} ./test.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(DBG_FLAGS) -o $@ $^ $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

rng.o : rng.h
api.o : api.h

clean:
	cd src; rm -f *.o; cd ..
	rm -f *.o
	rm -f sign runtime



PQC_bench: ${OBJS} ./PQC_bench.c
	$(CC) $(LDFLAGS) $(CFLAGS) -O3 -DNDEBUG -o $@ PQC_bench.c $(SOURCES) $(LIBFLAGS)
	./PQC_bench

Module_bench: ${OBJS} ./Module_bench.c
	$(CC) $(LDFLAGS) $(CFLAGS) -O3 -DNDEBUG -o $@ Module_bench.c $(SOURCES) $(LIBFLAGS)
	./Module_bench

clean_bench:
	rm PQC_bench