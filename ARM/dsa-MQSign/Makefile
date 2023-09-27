CC=    gcc
CXX=   g++
LD=    gcc

PROJ = MQSALG
PROJ_DIR = ./$(PROJ)

CFLAGS= -O3 -std=c11 -Wall -Wextra $(BUILT_OPT)
INCPATH= -I$(PROJ_DIR)

SRCS = $(wildcard $(PROJ_DIR)/*.c)
SRCS_O = $(SRCS:.c=.o)
SRCS_O_ND = $(subst $(PROJ_DIR)/,,$(SRCS_O))

OBJ = $(SRCS_O_ND) utils.o

EXE = bench

.PHONY: all tests tables clean

all: $(OBJ) $(EXE)

PQCgenKAT_sign: $(OBJ) PQCgenKAT_sign.o
	$(LD) -o $@ $^ $(LIBS)

bench: $(OBJ) bench.o
	@$(LD) -o $@ $^ $(LIBS)

PQC_bench: ./PQC_bench.c
	$(CC)  -mtune=native -O3 -fomit-frame-pointer -o $@ ./PQC_bench.c $(INCPATH) $(SRCS)
	./PQC_bench


%.o: %.c
	@$(CC) $(CFLAGS) $(INCPATH) -c $<

%.o: $(PROJ_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCPATH) -c $<

%.o: common/%.c
	@$(CC) $(CFLAGS) $(INCPATH) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $<

tests:
	cd tests; make

tables:
	cd supplement; make

clean:
	rm *.o bench

clean_bench:
	rm PQC_bench