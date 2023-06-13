# KPQClean
Benchmark on Korean Post Quantum Cryptography!

## What is KPQClean?
This project was inspired by PQClean (<https://github.com/PQClean/PQClean>).

We removed the dependencies of KpqC algorithms and benchmarked them in the same environment (currently on Intel processors).

There are still a lot of works to be done, but the current version may provide basic performance comparisons between KpqC algorithms.


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
### Testing Environment
* OS: Ubuntu 22.04 
* CPU: Ryzen 7 4800H (2.90 GHz)
* RAM: 16GB
* Compiler: gcc 11.3.0
* Optimization Level: -O2
* Each algorithms 10,000 iterated, and **average** value of the operation cycle is used.

### PKE/KEM
| Algorithm     | Keygen		 | Encapsulation | Decapsulation | 
| ------------- | -------------: | -------------:| -------------:|
| IPCC-1  		| 15,006,320	 | 236,123,169	 | 2,859,824	 |
| IPCC-3 		| 15,411,372	 | 942,886		 | 2,829,830	 |
| IPCC-4		| 15,265,594	 | 1,137,671	 | 3,248,358	 |
| NTRUplus-576	| 286,684		 | 112,556		 | 128,076		 |
| NTRUplus-768	| 283,117		 | 149,447		 | 182,447		 |
| NTRUplus-864	| 314,850		 | 172,643		 | 216,461		 |
| NTRUplus-1152	| 791,357		 | 224,529		 | 278,881		 |
| PALOMA-128	| 124,028,368	 | 512,524 		 | 34,545		 |
| PALOMA-192	| 123,919,087	 | 511,028		 | 34,376		 |
| PALOMA-256	| 124,650,597	 | 511,443		 | 34,212		 |
| SMAUG-128		| 147,834 		 | 158,827		 | 172,337		 | 
| SMAUG-192		| 200,560		 | 225,867 		 | 245,631		 | 
| SMAUG-256		| 340,791 		 | 348,204		 | 367,505 		 |
| TiGER-128		| 174,181		 | 280,960		 | 375,857		 | 
| TiGER-192		| 195,760		 | 307,019		 | 399,939 		 | 
| TiGER-256		| 327,364		 | 646,894 		 | 879,634 		 | 

### Digital Signature
| Algorithm     | Keygen		 | Signature | Verification | 
| ------------- | -------------: | -------------:| -------------:|
| AIMer-I  				| 147,824		| 3,925,397		| 3,688,437		|
| AIMer-III				| 309,485 		| 8,173,125 	| 7,712,166 	|
| AIMer-V				| 712,579 		| 18,213,811 	| 17,515,554 	|
| GCKSign-II			| 191,048 		| 812,492 		| 175,823 		|
| GCKSign-III			| 198,099 		| 891,697 		| 180,034 		|
| GCKSign-V				| 432,874 		| 1,841,754 	| 342,830 		|
| HAETAE-II				| 1,986,112 	| 14,791,845 	| 2,427,387 	|
| HAETAE-III			| 4,413,154 	| 41,798,697 	| 5,089,998 	|
| HAETAE-V				| 6,810,562 	| 37,858,000 	| 7,803,629 	|
| MQSign-72/46			| 95,242,213 	| 521,368 		| 1,470,313 	|
| MQSign-112/72			| 490,303,519 	| 1,501,506 	| 5,203,085 	|
| MQSign-148/96			| 1,492,894,919 | 3,172,366 	| 12,054,509 	|
| NCCSign-IIo			| 1,891,678 	| 16,626,486 	| 3,723,436 	|
| NCCSign-IIIo			| 3,689,017 	| 32,444,017 	| 7,265,226 	|
| NCCSign-Vo			| 6,279,118 	| 43,207,193 	| 12,437,221 	|
| NCCSign-IIc			| 2,669,077 	| 33,881,336 	| 5,233,430 	|
| NCCSign-IIIc			| 4,482,423 	| 30,863,557 	| 8,851,645 	|
| NCCSign-Vc			| 7,246,585 	| 86,158,604 	| 14,356,581 	|
| Peregrine-512 		| 12,502,738 	| 330,561 		| 37,475 		|
| Peregrine-1024		| 41,931,477 	| 709,773 		| 80,480 		|
| Enhanced pqsigRM-612	| 6,175,797,126 | 14,737,341 	| 2,441,347 	|
| Enhanced pqsigRM-613	| 58,885,289,264| 2,145,199 	| 1,061,605 	|
| SOLMAE-512			| 29,080,153	| 387,621		| 43,589		|
| SOLMAE-1024			| 70,391,110	| 768,220		| 144,557		|

## Parameters
* Unit: Bytes
* All values come from each white paper.
* Some values are different from the value written on the source code.

### PKE/KEM
| Algorithm     | Pulbic Key     | Secret Key    | Ciphertext    | 
| ------------- | -------------: | -------------:| -------------:|
| IPCC-1  		| 3,600	     | 400	         | 322,000	 |
| IPCC-3 		| 3,600	     | 400	    	 | 322,000	 |
| IPCC-4		| 3,600	     | 400	         | 322,000	 |
| NTRUplus-576	| 864		 | 1,728		 | 864		 |
| NTRUplus-768	| 1,152		 | 2,304		 | 1,152	 |
| NTRUplus-864	| 1,296		 | 2,592		 | 1,296	 |
| NTRUplus-1152	| 1,728		 | 3,456		 | 1,728	 |
| PALOMA-128	| 319,488	 | 93,008 		 | 136		 |
| PALOMA-192	| 812,032	 | 355,400		 | 240		 |
| PALOMA-256	| 1,025,024	 | 357,064		 | 240		 |
| SMAUG-128		| 672 		 | 174		     | 768		 | 
| SMAUG-192		| 992		 | 185 		     | 1,024	 | 
| SMAUG-256		| 1,632 	 | 182		     | 1,536	 |
| TiGER-128		| 544		 | 528		     | 1,024	 | 
| TiGER-192		| 1,056		 | 1,056		 | 2,048	 | 
| TiGER-256		| 1,056		 | 1,056 		 | 2,048	 | 

### Digital Signature
| Algorithm     | Public Key    | Srcret Key    | Signature     | 
| ------------- | -------------: | -------------:| -------------:|
| AIMer-I  				| 33		| 49		| 5,904		|
| AIMer-III				| 49 		| 73 	    | 13,080 	|
| AIMer-V				| 65 		| 97	    | 25,152 	|
| GCKSign-II			| 1,760 	| 288 		| 1,952 	|
| GCKSign-III			| 1,952 	| 288 		| 2,080		|
| GCKSign-V				| 3,040 	| 5,444 	| 3,104 	|
| HAETAE-II				| 1,120 	| 1,536 	| 3,040 	|
| HAETAE-III			| 1,664 	| 2,272 	| 4,544 	|
| HAETAE-V				| 2,208 	| 2,944 	| 5,792 	|
| MQSign-72/46			| 328,411 	| 15,561 	| 134 	    |
| MQSign-112/72			| 1,238,761 | 37,729 	| 200 	    |
| MQSign-148/96			| 2,892,961 | 66,421 	| 260 	    |
| NCCSign-IIo			| 1,564 	| 2,266 	| 2,458 	|
| NCCSign-IIIo			| 1,997 	| 3,312 	| 3,605 	|
| NCCSign-Vo			| 2,663 	| 4,402	    | 5,055 	|
| NCCSign-IIc			| 1,984 	| 2,800 	| 3,186 	|
| NCCSign-IIIc			| 2,443 	| 3,914 	| 4,251 	|
| NCCSign-Vc			| 3,091 	| 4,940 	| 5,383 	|
| Peregrine-512 		| - 	    | - 		| - 		|
| Peregrine-1024		| - 	    | - 		| -		    |
| Enhanced pqsigRM-612	| 1,285,120 | 24,592 	| 528   	|
| Enhanced pqsigRM-613	| 4,194,304 | 331,074 	| 1,040 	|
| SOLMAE-512			| -     	| -		    | -		    |
| SOLMAE-1024			| -	        | -		    | -		    |
* Peregrine and SOLMAE do not specify parameters in the white paper

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
* (Ph.D. student) HyeokDong Kwon
* (Ph.D. student) MinJoo Sim
* (Ph.D. student) GyeongJu Song
* (Master student) MinWoo Lee
* (Associate professor) HwaJeong Seo

