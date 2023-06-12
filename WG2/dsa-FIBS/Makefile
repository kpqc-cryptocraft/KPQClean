LDLIBS=
CC = gcc
OPT=-O2
CFLAGS = -Wall -Wextra -Wpedantic $(OPT) -std=c99

ifeq "$(CC)" "gcc"
	COMPILER=gcc
else ifeq "$(CC)" "clang"
	COMPILER=clang
endif

HASH = sha256
THASH = simple

ARCHITECTURE=_AMD64_
USE_OPT_LEVEL=_FAST_

ifeq "$(ARCH)" "x64"
	ARCHITECTURE=_AMD64_
	USE_OPT_LEVEL=_FAST_
else ifeq "$(ARCH)" "x86"
	ARCHITECTURE=_X86_
	USE_OPT_LEVEL=_GENERIC_
else ifeq "$(ARCH)" "s390x"
	ARCHITECTURE=_S390X_
	USE_OPT_LEVEL=_GENERIC_
else ifeq "$(ARCH)" "ARM"
	ARCHITECTURE=_ARM_
	USE_OPT_LEVEL=_GENERIC_
	ARM_TARGET=YES
else ifeq "$(ARCH)" "ARM64"
	ARCHITECTURE=_ARM64_
	USE_OPT_LEVEL=_FAST_
	ARM_TARGET=YES
endif

ifeq "$(OPT_LEVEL)" "GENERIC"
	USE_OPT_LEVEL=_GENERIC_
endif

ifeq "$(ARM_TARGET)" "YES"
	ARM_SETTING=-lrt
endif

ifeq "$(ARCHITECTURE)" "_AMD64_"
	ifeq "$(USE_OPT_LEVEL)" "_FAST_"
		MULX=-D _MULX_
		ifeq "$(USE_MULX)" "FALSE"
			MULX=
		else
			ADX=-D _ADX_
			ifeq "$(USE_ADX)" "FALSE"
				ADX=
			endif
		endif
	endif
endif

AR=ar rcs
RANLIB=ranlib
ADDITIONAL_SETTINGS=-march=native
ifeq "$(CC)" "clang"
ifeq "$(ARM_TARGET)" "YES"
	ADDITIONAL_SETTINGS=
endif
endif
ifeq "$(ARCHITECTURE)" "_S390X_"
	ADDITIONAL_SETTINGS=-march=z10
endif

SOURCES =          address.c rng.c wots.c utils.c fors.c sign.c hash_$(HASH).c thash_$(HASH)_$(THASH).c aes.c
HEADERS = params.h address.h rng.h wots.h utils.h fors.h api.h  hash.h thash.h aes.h

ifeq ($(HASH),shake256)
	SOURCES += fips202.c
	HEADERS += fips202.h
endif
ifeq ($(HASH),haraka)
	SOURCES += haraka.c
	HEADERS += haraka.h
endif
ifeq ($(HASH),sha256)
	SOURCES += sha256.c
	HEADERS += sha256.h
endif

DET_SOURCES = $(SOURCES:rng.%=rng.%)
DET_HEADERS = $(HEADERS:rng.%=rng.%)

TESTS = test/wots \
		test/fors \
		test/spx \

BENCHMARK = test/benchmark

CFLAGSCGL=$(OPT) -std=gnu11 $(ADDITIONAL_SETTINGS) -D $(ARCHITECTURE) -D __NIX__ -D $(USE_OPT_LEVEL) $(MULX) $(ADX)
LDFLAGS=-lm
ifeq "$(USE_OPT_LEVEL)" "_GENERIC_"
	EXTRA_OBJECTS_610=objs610/fp_generic.o
else ifeq "$(USE_OPT_LEVEL)" "_FAST_"
ifeq "$(ARCHITECTURE)" "_AMD64_"
	EXTRA_OBJECTS_610=objs610/fp_x64.o objs610/fp_x64_asm.o
endif
endif
OBJECTS_610=objs610/P610.o $(EXTRA_OBJECTS_610) objs/random.o objs/fips202.o

.PHONY: clean test benchmark


all: lib610 testcgl PQCgenKAT_sign tests benchmarks 

tests: $(TESTS)

test: $(TESTS:=.exec)

benchmarks: $(BENCHMARK)

benchmark: $(BENCHMARK:=.exec)

objs610/%.o: cglhash/P610/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGSCGL) $< -o $@
    
ifeq "$(USE_OPT_LEVEL)" "_GENERIC_"     
    objs610/fp_generic.o: cglhash/P610/generic/fp_generic.c
	    $(CC) -c $(CFLAGSCGL) cglhash/P610/generic/fp_generic.c -o objs610/fp_generic.o
else ifeq "$(USE_OPT_LEVEL)" "_FAST_"
ifeq "$(ARCHITECTURE)" "_AMD64_"        
    objs610/fp_x64.o: cglhash/P610/AMD64/fp_x64.c
	    $(CC) -c $(CFLAGSCGL) cglhash/P610/AMD64/fp_x64.c -o objs610/fp_x64.o

    objs610/fp_x64_asm.o: cglhash/P610/AMD64/fp_x64_asm.S
	    $(CC) -c $(CFLAGSCGL) cglhash/P610/AMD64/fp_x64_asm.S -o objs610/fp_x64_asm.o
endif
endif   

objs/random.o: cglhash/random/random.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGSCGL) cglhash/random/random.c -o objs/random.o

objs/fips202.o: cglhash/sha3/fips202.c
	$(CC) -c $(CFLAGSCGL) cglhash/sha3/fips202.c -o objs/fips202.o
    
lib610: $(OBJECTS_610)
	rm -rf lib610 cgl
	mkdir lib610 cgl
	$(AR) lib610/libcgl.a $^
	$(RANLIB) lib610/libcgl.a
    
testcgl: lib610
	$(CC) $(CFLAGSCGL) -L./lib610 cglhash/tests/test_cglp610.c cglhash/tests/test_extras.c -lcgl $(LDFLAGS) -o cgl/test_KEM $(ARM_SETTING)

    
PQCgenKAT_sign: PQCgenKAT_sign.c $(DET_SOURCES) $(DET_HEADERS) lib610
	$(CC) $(CFLAGS) -o $@ $(DET_SOURCES) $< -lcgl -lm -L./lib610

test/%: test/%.c $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $< $(LDLIBS)  -lcgl -L./lib610


test/%.exec: test/%
	@$<

clean:
	-$(RM) $(TESTS)
	-$(RM) $(BENCHMARK)
	-$(RM) PQCgenKAT_sign
	-$(RM) PQCsignKAT_*.rsp
	-$(RM) PQCsignKAT_*.req
	rm -rf *.req objs610 objs lib610 cgl
