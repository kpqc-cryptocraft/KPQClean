INCDIR := include
SRCDIR := src
BENDIR := benchmark
KATDIR := kat
MANDIR := test

CC = gcc

SOURCES= ./$(SRCDIR)/*.c
HEADERS= ./$(INCDIR)/*.h

SOURCE_MAIN = ./$(MANDIR)/main.c ./$(SRCDIR)/*.c
SOURCE_BENCH = ./$(BENDIR)/*.c ./$(SRCDIR)/*.c
SOURCE_KAT = ./$(KATDIR)/*.c ./$(SRCDIR)/*.c
HEADER = -I./$(INCDIR)

CFLAGS =  -mtune=native -O3 -fomit-frame-pointer -DSMAUG_MODE=1
LDFLAGS =
DFLAGS = -g3 -DDEBUG

ALL:
	$(CC) $(CFLAGS) -c $(SOURCE_MAIN) $(HEADER)
	$(CC) $(CFLAGS) *.o -o main

pqckat:
	$(CC) $(CFLAGS) -c $(SOURCE_KAT) $(HEADER) #-DCRYPTO_ALGNAME="SMAUG1_KEM"
	$(CC) $(CFLAGS) *.o -o pqckat

bench:
	$(CC) $(CFLAGS) -c $(SOURCE_BENCH) $(HEADER)
	$(CC) $(CFLAGS) *.o -o bench
	
PQC_bench: ./PQC_bench.c
	$(CC)  -mtune=native -O3 -fomit-frame-pointer -DSMAUG_MODE=5 -o  $@ ./PQC_bench.c $(HEADER) $(SOURCES) $(LDFLAGS)
	./PQC_bench
	
clean:
	rm *.o
	rm main

clean_pqckat:
	rm *.o *.req *.rsp
	rm pqckat

clean_banch:
	rm *.o
	rm bench
	
clean_bench:
	rm PQC_bench
