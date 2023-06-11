# SPDX-License-Identifier: MIT

CC = gcc
CFLAGS += -I. -O3 -Wall -Wextra -march=native -fomit-frame-pointer
NISTFLAGS = -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-result
SHAKE_PATH = shake
SHAKE_LIB = libshake.a
LDFLAGS = $(SHAKE_PATH)/$(SHAKE_LIB)

EXECUTABLE_TESTTREE = tests/test_tree
EXECUTABLE_TESTAIM  = tests/test_aim
EXECUTABLE_TESTSIGN = tests/test_sign
EXECUTABLE_KAT      = PQCgenKAT_sign

EXECUTABLE_BENCH	= PQC_bench

.PHONY: all

all: $(SHAKE_LIB) $(EXECUTABLE_TESTTREE) $(EXECUTABLE_TESTAIM) $(EXECUTABLE_TESTSIGN) $(EXECUTABLE_KAT)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(SHAKE_LIB):
	$(MAKE) -C $(SHAKE_PATH)

$(EXECUTABLE_TESTTREE): $(EXECUTABLE_TESTTREE).c hash.c tree.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(EXECUTABLE_TESTAIM): $(EXECUTABLE_TESTAIM).c field/field128.c aim128.c hash.c aes.c
	$(CC) $(CFLAGS) -D_AIMER_L=1 $^ $(LDFLAGS) -o $@

$(EXECUTABLE_TESTSIGN): $(EXECUTABLE_TESTSIGN).c field/field128.c aim128.c rng.c hash.c tree.c aimer_internal.c aimer_instances.c aimer.c aes.c
	$(CC) $(CFLAGS) -D_AIMER_L=1 $^ $(LDFLAGS) -o $@

$(EXECUTABLE_KAT): $(EXECUTABLE_KAT).c api.c field/field128.c aim128.c rng.c hash.c tree.c aimer_internal.c aimer_instances.c aimer.c aes.c
	$(CC) $(CFLAGS) -D_AIMER_L=1 $^ $(LDFLAGS) -o $@


lib: $(SHAKE_LIB) 

PQC_bench: PQC_bench.c api.c field/field128.c aim128.c rng.c hash.c tree.c aimer_internal.c aimer_instances.c aimer.c aes.c
	$(CC) -I. -O2 -Wall -Wextra -march=native -fomit-frame-pointer -D_AIMER_L=1 $^ $(LDFLAGS) -o $@
	./PQC_bench

Module_bench: Module_bench.c api.c field/field128.c aim128.c rng.c hash.c tree.c aimer_internal.c aimer_instances.c aimer.c aes.c
	$(CC) -I. -O2 -Wall -Wextra -march=native -fomit-frame-pointer -D_AIMER_L=1 $^ $(LDFLAGS) -o $@
	./Module_bench

clean:
	rm -f $(wildcard *.o) $(EXECUTABLE_TESTTREE) $(EXECUTABLE_TESTAIM) $(EXECUTABLE_TESTSIGN) $(EXECUTABLE_KAT)
	$(MAKE) -C $(SHAKE_PATH) clean

clean_bench:
	rm PQC_bench
	rm *.su