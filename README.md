# KPQClean
Benchmark on Korean Post Quantum Cryptography!

## What is KPQClean?
This project was inspired by PQClean (<https://github.com/PQClean/PQClean>).

We removed the dependencies of KpqC algorithms and benchmarked them in the same environment (currently on Intel processors).

There are still a lot of works to be done, but current version may provide basic performance comparisons between KpqC algorithms.


## Source code version
'Round 1 Submission' means the code published in KpqC competition Round 1(not updated after submission).

When updated source code was used, indicate the date the source code was updated.

### PKE/KEM
1. IPCC: Round 1 Submission.
2. NTRUplus: Round 1 Submission.
3. PALOMA: Round 1 Submission.
4. SMAUG: May. 23rd. (<https://groups.google.com/g/kpqc-bulletin/c/8S0qef2mPSU>)
5. TiGER: Round 1 Submission.
6. REDOG: Round 1 Submission.
7. Layered-ROLLO: Round 1 Submission.
### Digital Signature
1. AIMer: Round 1 Submission.
2. GCKSign: Round 1 Submission.
3. HAETAE: May. 2nd. (<https://groups.google.com/g/kpqc-bulletin/c/Xtnsjrai83s>)
4. MQSign: Round 1 Submission.
5. NCCSign: Round 1 Submission.
6. Peregrine: Round 1 Submission.
7. SOLMAE: Round 1 Submission.
8. Enhanced pqsigRM: Round 1 Submission.
9. FIBS: Round 1 Submission.


## Benchmark result
### PKE/KEM
### Digital Signature

KEM
Cycle
PK
SK
Ciphertext

| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |


## Parameters
### PKE/KEM
### Digital Signature

Sign
Cycle
PK
SK
Signature

WG0 algorithm + descprtion...
WG1
WG2

## How to use
### Compile command
Use the following command for KpqC benchmarking compile.

    make PQC_bench
    make clean_bench
    
If another command is not specified, compile with this.
### Specific case
#### PALOMA
Change stack limit before compile.

    ulimit -s 51200

#### SMAUG
To change SMAUG scheme, change -DSMAUG_MODE value at Makefile.
#### HAETAE
To change HAETAE scheme, modify #define HAETAE_MODE macro in config.h before compile.
#### SOLMAE
Compile with the following command.

    make
    clean

## Contact
* HyeokDong Kwon: <korlethean@gmail.com>
* HwaJeong Seo: <hwajeong84@gmail.com>

### Contributers
* HyeokDong Kwon
* MinJoo Sim
* GyeongJu Song
* MinWoo Lee
* HwaJeong Seo

