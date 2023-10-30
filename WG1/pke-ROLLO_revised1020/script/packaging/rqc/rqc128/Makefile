
CC=gcc
C_FLAGS:=-O3 -flto -mavx2 -mpclmul -msse4.2 -maes -std=c99 -pedantic -Wall -Wextra

SHA2_SRC:=lib/sha2/sha2.c
SHA2_INCLUDE:=-I lib/sha2 -lcrypto

FIPS202_SRC:=lib/fips202/fips202.c
FIPS202_INCLUDE:=-I lib/fips202

RANDOMBYTES_SRC:=lib/randombytes/randombytes.c
RANDOMBYTES_INCLUDE:=-I lib/randombytes -lcrypto

SEEDEXPANDER_SRC:=lib/seedexpander/seedexpander.c
SEEDEXPANDER_INCLUDE:=-I lib/seedexpander -lcrypto

RBC_SRC:=src/rbc-127
RBC_INCLUDE:=-I src/rbc-127

SRC:=src
INCLUDE:=-I src $(RBC_INCLUDE)
LIB:=$(SHA2_INCLUDE) $(FIPS202_INCLUDE) $(RANDOMBYTES_INCLUDE) $(SEEDEXPANDER_INCLUDE)

MAIN_RQC:=src/main_rqc.c
MAIN_KAT:=src/main_kat.c

RQC_OBJS:=rbc_elt.o rbc_vec.o rbc_poly.o rbc_qre.o rbc_vspace.o parsing.o qpoly.o gabidulin.o rqc.o kem.o
RQC_OBJS_VERBOSE:=rbc_elt.o rbc_vec.o rbc_poly.o rbc_qre.o rbc_vspace.o parsing.o qpoly.o gabidulin-verbose.o rqc-verbose.o kem-verbose.o
LIB_OBJS:=sha2.o fips202.o randombytes.o seedexpander.o

BUILD:=bin/build
BIN:=bin



folders:
	@echo -e "\n### Creating build folders\n"
	mkdir -p $(BUILD)

sha2.o: folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $(SHA2_SRC) $(SHA2_INCLUDE) -o $(BUILD)/$@

fips202.o: folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $(FIPS202_SRC) $(FIPS202_INCLUDE) -o $(BUILD)/$@

randombytes.o: folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $(RANDOMBYTES_SRC) $(RANDOMBYTES_INCLUDE) -o $(BUILD)/$@

seedexpander.o: folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $(SEEDEXPANDER_SRC) $(SEEDEXPANDER_INCLUDE) -o $(BUILD)/$@



rbc_%.o: $(RBC_SRC)/rbc_%.c | folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $< $(RBC_INCLUDE) $(LIB) -o $(BUILD)/$@

%.o: $(SRC)/%.c | folders
	@echo -e "\n### Compiling $@\n"
	$(CC) $(C_FLAGS) -c $< $(INCLUDE) $(LIB) -o $(BUILD)/$@

%-verbose.o: $(SRC)/%.c | folders
	@echo -e "\n### Compiling $@ (verbose mode)\n"
	$(CC) $(C_FLAGS) -c $< $(INCLUDE) $(LIB) -D VERBOSE -o $(BUILD)/$@



rqc128: $(RQC_OBJS) $(LIB_OBJS) | folders
	@echo -e "\n### Compiling RQC-128\n"
	$(CC) $(C_FLAGS) $(MAIN_RQC) $(addprefix $(BUILD)/, $^) $(INCLUDE) $(LIB) -o $(BIN)/$@

rqc128-kat: $(RQC_OBJS) $(LIB_OBJS) | folders
	@echo -e "\n### Compiling RQC-128 KAT\n"
	$(CC) $(C_FLAGS) $(MAIN_KAT) $(addprefix $(BUILD)/, $^) $(INCLUDE) $(LIB) -o $(BIN)/$@

rqc128-verbose: $(RQC_OBJS_VERBOSE) $(LIB_OBJS) | folders
	@echo -e "\n### Compiling RQC-128 (verbose mode)\n"
	$(CC) $(C_FLAGS) $(MAIN_RQC) $(addprefix $(BUILD)/, $^) $(INCLUDE) $(LIB) -D VERBOSE -o $(BIN)/$@



clean:
	rm -f PQCkemKAT_*
	rm -f vgcore.*
	rm -rf ./bin

