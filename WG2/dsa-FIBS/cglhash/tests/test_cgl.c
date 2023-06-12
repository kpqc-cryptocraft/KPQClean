/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
* Abstract: benchmarking/testing isogeny-based key encapsulation mechanism
*********************************************************************************************/ 


// Benchmark and test parameters  
#if defined(OPTIMIZED_GENERIC_IMPLEMENTATION) || (TARGET == TARGET_ARM) 
    #define BENCH_LOOPS        100      // Number of iterations per bench 
    #define TEST_LOOPS         100      // Number of iterations per test
#else
    #define BENCH_LOOPS       100
    #define TEST_LOOPS        100      
#endif




int cryptorun_hash()
{ // Benchmarking key exchange
    unsigned int n;
    unsigned char PrivateKeyA[CGL_MESSAGEBYTES];
    unsigned char PublicKeyA[CGL_BYTES];
    unsigned char SharedSecretA[CGL_HASHBYTES];
    unsigned long long cycles, cycles1, cycles2;

   
    printf("--------------------------------------------------------------------------------------------------------\n\n");

    // Benchmarking Alice's key generation
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();
        CGLhash(PrivateKeyA, PublicKeyA, CGL_MESSAGEBYTES);
        cycles2 = cpucycles();
        cycles = cycles+(cycles2-cycles1);
    }
    printf("  Isogeny Hash runs in in ............................... %10lld ", cycles/BENCH_LOOPS); print_unit;
    printf("\n");


    return PASSED;
}



int main()
{
    int Status = PASSED;
    ecisog_run();
    cryptorun_hash();

    /*
    Status = cryptotest_kem();             // Test key encapsulation mechanism
    if (Status != PASSED) {
        printf("\n\n   Error detected: KEM_ERROR_SHARED_KEY \n\n");
        return FAILED;
    }

    Status = cryptorun_kem();              // Benchmark key encapsulation mechanism
    if (Status != PASSED) {
        printf("\n\n   Error detected: KEM_ERROR_SHARED_KEY \n\n");
        return FAILED;
    }
*/
    return Status;
}