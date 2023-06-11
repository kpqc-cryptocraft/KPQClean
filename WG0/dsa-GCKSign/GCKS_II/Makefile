INCDIR   := inc
SRCDIR   := src

CC=gcc

SOURCES= ./src/fips202.c ./src/NIST/rng.c ./src/pack.c  ./src/symmetric.c ./src/aes256ctr.c ./src/poly.c ./src/reduce.c ./src/sample.c ./src/sign.c ./src/NIST/aes.c ./src/sha2.c
HEADERS= ./inc/api.h ./inc/fips202.h ./inc/NIST/rng.h ./inc/pack.h ./inc/symmetric.h ./inc/aes256ctr.h ./inc/poly.h ./inc/reduce.h ./inc/sample.h ./inc/sign.h ./inc/params.h ./inc/NIST/aes.h ./inc/sha2.h

SOURCE = main.c ./src/*.c  ./$(SRCDIR)/NIST/*.c 
HEADER = -I./$(INCDIR) -I./$(INCDIR)/NIST

CFLAGS  = -march=native -mtune=native -O3 -fomit-frame-pointer
LDFLAGS = 
DFLAGS  = -g3 -DDEBUG



	
ALL:
	$(CC) $(CFLAGS) -c  $(SOURCE) $(HEADER)
	$(CC) $(CFLAGS) *.o -o main $(LDFLAGS)
	#rm *.o
	



PQCgenKAT_sign: ./PQCgenKAT_sign.c
	$(CC) $(CFLAGS) -o $@ ./PQCgenKAT_sign.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQCgenKAT_sign

PQC_bench: ./PQC_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./PQC_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQC_bench

Module_bench: ./Module_bench.c
	$(CC) -march=native -mtune=native -O2 -fomit-frame-pointer -o $@ ./Module_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./Module_bench

debug:
	$(CC) $(DCFLAGS) -c  $(SOURCE) $(HEADER)
	$(CC) $(DCFLAGS) *.o -o debug_main $(LDFLAGS)
	#rm *.o
	./debug_main

.PHONY: clean

clean:
	rm *.o
	rm main
	rm PQCgenKAT_sign

clean_bench:
	rm PQC_bench
	rm *.su