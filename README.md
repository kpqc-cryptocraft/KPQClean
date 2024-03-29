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
1. NTRUplus: Sep. 16th. (<https://groups.google.com/g/kpqc-bulletin/c/mQ3FGgxtu4c>)
2. PALOMA: Round 1 Submission.
3. SMAUG: May. 23rd. (<https://groups.google.com/g/kpqc-bulletin/c/8S0qef2mPSU>)
4. TiGER: Sept. 17th. (<https://groups.google.com/g/kpqc-bulletin/c/f_p6WCN7kdw>)
5. REDOG: Round 1 Submission.
6. Layered-ROLLO: Oct. 20th. (<https://groups.google.com/g/kpqc-bulletin/c/8nOd28f2K7k/m/YSs4YDF6AAAJ>)
7. IPCC: Round 1 Submission.

### Digital Signature
1. AIMer: June. 23rd. (<https://groups.google.com/g/kpqc-bulletin/c/mIjGH2fKMeE>)
2. GCKSign: Round 1 Submission.
3. HAETAE: May. 2nd. (<https://groups.google.com/g/kpqc-bulletin/c/Xtnsjrai83s>)
4. MQSign: Round 1 Submission.
5. NCCSign: Round 1 Submission.
6. Peregrine: Round 1 Submission.
7. SOLMAE: Round 1 Submission.
8. Enhanced pqsigRM: Round 1 Submission.
9. FIBS: Round 1 Submission.


## Benchmark result
### Testing Environment1
* OS: Ubuntu 22.04 
* CPU: Ryzen 7 4800H (2.90 GHz)
* RAM: 16GB
* Compiler: gcc 11.3.0
* Optimization Level: -O2. -O3

### Testing Environment2
* OS: Ubuntu 22.04 
* CPU: Intel i5-8259U (3.80 GHz)
* RAM: 16GB
* Compiler: gcc 11.3.0
* Optimization Level: -O2, -O3

### Testing Environment3
* OS: macOS Ventura 13.5.2
* CPU: Apple M1 Pro (3.228 GHz)
* RAM: 16GB
* Compiler: Apple clang 14.0.3
* Optimization Level: -O3

### Testing Environment4
* OS: Raspbian GNU/Linux 12 (bookworm) 32-bit
* SoC: Broadcom BCM2837 (armv7l, 1.4GHz)
* RAM: 1GB
* Compiler: gcc 12.2.0
* Optimization Level: -O3

### Benchmark method
* We used 'rdtsc' instruction to calculate time consumption.
* Each algorithms 10,000 iterated, and **average**, **median** value of the operation cycle is used.
* Enhanced pqsigRM was 100 iterated.
* On the Testing Environment3, each algorithms are 100 iterated, and the unit is nano second.
* On the Testing Environment4, each algorithms are 10 iterated, and the unit is nano second.

### PKE/KEM (Environment1, -O2)
<details>
<summary>PKE/KEM-Env1-O2 Table (Unit: clock cycles)</summary>
    
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 185,861 		 		| 108,315 				| 108,083 		 		| 262,723 				| 108,856 				| 108,597 				|
| Lattice		| NTRUplus-768			| 254,672 		 		| 122,688 				| 127,294 		 		| 287,516 				| 140,831 				| 146,458				|
| Lattice		| NTRUplus-864			| 274,978 		 		| 159,558 				| 171,999		 		| 286,033 				| 160,639 				| 172,679 				|
| Lattice		| NTRUplus-1152			| 391,732 		 		| 202,304				| 233,885		 		| 681,708 				| 203,716 				| 235,592 				|
| Lattice		| SMAUG-128				| 171,477 		 		| 154,483				| 178,205		 		| 181,007				| 156,512				| 181,950				|
| Lattice		| SMAUG-192				| 250,096		 		| 229,999				| 277,298		 		| 260,837				| 230,994				| 279,080				|
| Lattice		| SMAUG-256				| 479,138 		 		| 385,178		 		| 438,364 		 		| 490,702				| 387,420				| 439,345				|
| Lattice		| TiGER-128				| 144,362 		 		| 202,130				| 294,930		 		| 155,472 				| 205,374				| 296,766				|
| Lattice		| TiGER-192				| 266,046 		 		| 416,730		 		| 615,496 				| 267,393 				| 420,120 				| 619,737 				|
| Lattice		| TiGER-256				| 369,286 		 		| 630,663 		 		| 1,002,791 	 		| 373,687 				| 635,344				| 1,006,310 			|
| Code			| PALOMA-128			| 125,800,419	 		| 510,922 				| 35,496		 		| 125,630,139			| 513,098				| 36,061				|
| Code			| PALOMA-192			| 125,360,779	 		| 514,228				| 34,220		 		| 125,242,970			| 516,579				| 34,419				|
| Code			| PALOMA-256			| 125,294,065	 		| 510,284				| 34,713		 		| 125,174,502			| 512,685				| 34,978				|
| Code			| IPCC-1  				| 14,362,627	 		| ~~164,892,550~~		| 2,484,981				| 14,376,006			| ~~239,300,607~~		| 2,501,514				|
| Code			| IPCC-3		 		| 14,170,647	 		| 898,710		 		| 2,619,570				| 14,178,752			| 941,012				| 2,633,907				|
| Code			| IPCC-4				| 14,209,594	 		| 1,075,059	 			| 2,904,524				| 14,245,778			| 1,135,740				| 2,935,161				|
| Code			| Layered ROLLO I-128	| 379,871       		| 109,649 	     		| 784,697 	    		| 390,159          		| 110,771        		| 797,682 		   		|
| Code			| Layered ROLLO I-192	| 428,910       		| 149,988 	     		| 766,992         		| 445,280 	    		| 151,041       		| 773,379        		|
| Code			| Layered ROLLO I-256	| 661,722               | 248,037 	    		| 1,446,665       		| 669,091 				| 250,160 		 		| 1,460,123       		|
</details>

### PKE/KEM (Environment1, -O3)
<details>
<summary>PKE/KEM-Env1-O3 Table (Unit: clock cycles)</summary>
    
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 184,266 		 		| 106,691 				| 103,530		 		| 259,768 				| 107,141				| 103,949 				|
| Lattice		| NTRUplus-768			| 274,630 		 		| 134,328 				| 137,547 		 		| 285,107 				| 134,815 				| 138,043 				|
| Lattice		| NTRUplus-864			| 273,499 		 		| 157,557 				| 167,968 		 		| 274,676 				| 158,304 				| 168,695 				|
| Lattice		| NTRUplus-1152			| 390,543 		 		| 223,532				| 231,855 		 		| 686,244 				| 224,483 				| 233,006 				|
| Lattice		| SMAUG-128				| 72,790 		 		| 57,246				| 50,460		 		| 82,292				| 57,466				| 50,708				|
| Lattice		| SMAUG-192				| 105,966		 		| 82,940 				| 80,475		 		| 108,491				| 83,648				| 81,029				|
| Lattice		| SMAUG-256				| 158,021 		 		| 139,925		 		| 135,749 		 		| 161,110				| 141,573				| 138,010				|
| Lattice		| TiGER-128				| 60,146 		 		| 43,268 				| 44,341 		 		| 62,306 				| 43,425				| 44,527 				|
| Lattice		| TiGER-192				| 70,847 		 		| 66,758		 		| 62,031				| 81,879 				| 67,479 				| 64,205 				|
| Lattice		| TiGER-256				| 77,024 		 		| 83,723		 		| 82,041  		 		| 87,095 				| 84,910 				| 82,829 				|
| Code			| PALOMA-128			| 122,325,408	 		| 498,365 				| 34,307		 		| 122,253,994			| 500,446				| 34,484				|
| Code			| PALOMA-192			| 122,290,738	 		| 503,266				| 34,278		 		| 122,173,457			| 506,366				| 34,468				|
| Code			| PALOMA-256			| 122,321,957	 		| 497,959				| 34,249		 		| 122,254,172			| 500,026				| 34,420				|
| Code			| IPCC-1  				| 13,940,097	 		| ~~160,111,204~~		| ~~16,360,164~~		| 13,969,607			| ~~232,561,407~~		| 2,408,173				|
| Code			| IPCC-3		 		| 13,996,024	 		| 926,492		 		| 2,512,836				| 14,036,005			| 967,543				| 2,532,438				|
| Code			| IPCC-4				| 13,989,832	 		| 1,106,031	 			| 2,714,531				| 14,007,544			| 1,165,274				| 2,732,139				|
| Code			| Layered ROLLO I-128	| 269,381 				| 81,142 				| 568,168 				| 280,894 				| 82,170 				| 571,343 				|
| Code			| Layered ROLLO I-192	| 318,362 				| 111,592 				| 641,654 				| 330,740 				| 112,510 				| 644,127 				|
| Code			| Layered ROLLO I-256	| 542,010 				| 195,431 				| 1,263,907 			| 556,151 				| 196,644 				| 1,269,793 			|
</details>

### PKE/KEM (Environment2, -O2)
<details>
<summary>PKE/KEM-Env2-O2 Table (Unit: clock cycles)</summary>
    
| Type			| Algorithm          	| Keygen(Med.)         	| Encapsulation(Med.)   | Decapsulation(Med.)   |  Keygen(Avr.)      | Encapsulation(Avr.)	| Decapsulation(Avr.)	|
| -------------:| -------------:      	| -------------:       	| -------------:      	| -------------:      	| -------------:     | -------------:      	| -------------:      	|
| Lattice		| NTRUplus-576       	| 175,573             	| 115,344             	| 96,728              	| 263,837            | 147,083             	| 113,826              	|
| Lattice		| NTRUplus-768       	| 254,672             	| 122,688            	| 127,294              	| 287,516            | 140,831            	| 146,458              	|
| Lattice		| NTRUplus-864       	| 243,236             	| 141,784              	| 153,160            	| 286,642       	 | 170,594           	| 177,139            	|
| Lattice		| NTRUplus-1152      	| 599,381             	| 180,240              	| 212,660              	| 645,544            | 200,420              | 233,308              	|
| Lattice		| SMAUG-128         	| 158,149            	| 164,598             	| 196,470             	| 165,414            | 169,648             	| 203,288            	|
| Lattice		| SMAUG-192         	| 244,736             	| 225,490            	| 272,132             	| 265,142            | 236,227            	| 285,366            	|
| Lattice		| SMAUG-256         	| 435,790             	| 411,917             	| 465,572              	| 448,654            | 422,602            	| 486,263            	|
| Lattice		| TiGER-128         	| 148,122             	| 214,737             	| 306,852            	| 174,658            | 254,508             	| 352,721             	|
| Lattice		| TiGER-192         	| 284,636              	| 564,686              	| 828,367             	| 328,857            | 665,299 	        	| 903,561             	|
| Lattice		| TiGER-256         	| 404,288             	| 639,568           	| 1,432,020        		| 453,633            | 686,114         		| 1,421,420      		|
| Code			| PALOMA-128			| 118,204,341           | 499,914             	| 39,724              	| 118,365,137        | 511,837            	| 41,693             	|
| Code			| PALOMA-192        	| 118,310,371           | 499,302             	| 38,846              	| 118,490,933        | 514,299             	| 41,016             	|
| Code			| PALOMA-256        	| 118,366,206         	| 503,814            	| 43,174             	| 118,507,160        | 518,903             	| 45,385             	|
| Code			| IPCC-1            	| 13,792,887          	| ~~159,126,951~~       | 1,196,157            	| 13,896,694         | ~~231,010,613~~      | 1,259,215             |
| Code			| IPCC-3           		| 13,754,219          	| 870,059              	| 1,235,991            	| 13,864,988         | 922,755            	| 1,307,538             |
| Code			| IPCC-4            	| 13,754,687          	| 1,050,451             | 1,318,173             | 13,851,205         | 1,151,306            | 1,380,740             |
| Code			| Layered ROLLO I-128   | 436,486 	            | 142,110               | 933,571               | 478,855            | 151,564 		        | 984,667 	            |
| Code			| Layered ROLLO I-192   | 515,084 	            | 190,365 	            | 960,546               | 542,838            | 202,468 	            | 972,614 	            |
| Code			| Layered ROLLO I-256   | 780,800               | 290,565 	            | 1,548,044 		    | 837,400        	 | 315,783              | 1,663,367             |
</details>

### PKE/KEM (Environment2, -O3)
<details>
<summary>PKE/KEM-Env2-O3 Table (Unit: clock cycles)</summary>
    
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 162,956 		 		| 94,981 				| 93,298 		 		| 241,793 				| 101,155 				| 99,479 				|
| Lattice		| NTRUplus-768			| 250,592 		 		| 119,006 				| 122,437 		 		| 271,454 				| 127,308 				| 132,202 				|
| Lattice		| NTRUplus-864			| 240,904 		 		| 138,104 				| 149,058		 		| 261,313 				| 147,497 				| 160,356 				|
| Lattice		| NTRUplus-1152			| 517,578 		 		| 172,660 				| 198,334 		 		| 617,132 				| 184,218				| 212,901 				|
| Lattice		| SMAUG-128				| 63,020 		 		| 49,324				| 39,196		 		| 65,919				| 55,873				| 42,528				|
| Lattice		| SMAUG-192				| 92,658		 		| 69,739				| 67,691		 		| 95,436 				| 74,836				| 70,950				|
| Lattice		| SMAUG-256				| 135,202 		 		| 122,766		 		| 115,096 		 		| 142,842				| 128,734				| 118,789				|
| Lattice		| TiGER-128				| 55,092 		 		| 39,850 				| 45,977		 		| 60,904 				| 44,451 				| 51,154	 			|
| Lattice		| TiGER-192				| 65,530 		 		| 58,926 		 		| 55,800 				| 73,066  				| 66,244 				| 66,563 				|
| Lattice		| TiGER-256				| 73,902  		 		| 74,756		 		| 73,548 		 		| 80,087  				| 88,262 				| 84,296 				|
| Code			| PALOMA-128			| 108,402,198	 		| 459,846 				| 40,838		 		| 108,597,537			| 473,532				| 42,840				|
| Code			| PALOMA-192			| 108,206,652	 		| 460,374				| 40,688		 		| 108,344,570			| 472,432				| 42,798				|
| Code			| PALOMA-256			| 108,216,713	 		| 459,880				| 40,886		 		| 108,461,853			| 465,766				| 41,780				|
| Code			| IPCC-1  				| 12,643,392	 		| ~~145,233,220~~		| 1,159,273				| 12,712,124			| ~~210,977,105~~		| 1,185,580				|
| Code			| IPCC-3		 		| 12,795,377	 		| 874,663		 		| 1,206,585				| 12,874,291			| 922,533				| 1,267,783				|
| Code			| IPCC-4				| 13,078,917	 		| 1,037,485	 			| 1,310,503				| 13,250,237			| 1,107,017				| 1,368,035				|
| Code			| Layered ROLLO I-128	| 267,360 				| 88,882 				| 577,276 				| 299,840 				| 95,552 				| 665,704 				|
| Code			| Layered ROLLO I-192	| 321,910 				| 120,718 				| 616,007 				| 378,729 				| 130,088 				| 646,335 				|
| Code			| Layered ROLLO I-256	| 536,373 				| 197,644 				| 1,204,694 			| 605,193				| 210,251 				| 1,300,744 			|
</details>

### PKE/KEM (Environment3, -O3)
<details>
<summary>PKE/KEM-Env3-O3 Table (Unit: nsec)</summary>
    
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 119,040  		 		| 46,848				| 36,864 		 		| 124,979 				| 47,508				| 36,682 				|
| Lattice		| NTRUplus-768			| 87,040  		 		| 46,080				| 38,912  		 		| 76,460 				| 46,359  				| 38,684 				|
| Lattice		| NTRUplus-864			| 59,136 		 		| 37,888 				| 33,024  		 		| 59,668  				| 38,484 				| 32,640 				|
| Lattice		| NTRUplus-1152			| 273,024 		 		| 73,984 				| 61,952 		 		| 297,897  				| 71,583				| 61,501  				|
| Lattice		| SMAUG-128				| 51,200 		 		| 46,080				| 38,912 		 		| 61,448				| 49,454				| 38,013 				|
| Lattice		| SMAUG-192				| 82,944 		 		| 72,960 				| 46,848		 		| 91,505 				| 73,039 				| 47,158 				|
| Lattice		| SMAUG-256				| 64,000 		 		| 70,912 		 		| 79,104 		 		| 76,370 				| 69,309 				| 77,965 				|
| Lattice		| TiGER-128				| 54,016  		 		| 56,064  				| 44,032  		 		| 54,369  				| 56,271 				| 44,413				|
| Lattice		| TiGER-192				| 64,000		 		| 88,960 		 		| 75,008 				| 62,359 				| 94,866 				| 94,733				|
| Lattice		| TiGER-256				| 70,912 		 		| 125,952 		 		| 99,968   		 		| 68,004  				| 128,740 				| 103,688  				|
| Code			| PALOMA-128			| 34,776,064 	 		| 155,008  				| 8,960 		 		| 34,915,917 			| 153,480 				| 9,236					|
| Code			| PALOMA-192			| 34,742,912 	 		| 146,176 				| 8,960 		 		| 35,071,649 			| 147,387				| 9,359 				|
| Code			| PALOMA-256			| 34,691,968 	 		| 155,008				| 8,960			 		| 34,935,721 			| 154,355 				| 9,062					|
| Code			| IPCC-1  				| 4,171,904 	 		| 7,326,464 			| 230,144 				| 4,445,222 			| 11,629,652 			| 243,666 				|
| Code			| IPCC-3		 		| 4,205,952 	 		| 191,207,040 	 		| 246,400 				| 4,293,573				| 277,514,522 			| 277,967 				|
| Code			| IPCC-4				| 4,153,984 	 		| 233,392,384 			| 234,624 				| 4,281,823 			| 317,245,691 			| 255,708 				|
</details>

### PKE/KEM (Environment4, -O3)
<details>
<summary>PKE/KEM-Env4-O3 Table (Unit: nsec)</summary>
    
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| SMAUG-128				| 577,280  		 		| 597,888 				| 675,328  		 		| 593,766 				| 600,192 				| 675,789 				|
| Lattice		| SMAUG-192				| 993,152 		 		| 919,808  				| 1,072,128 		 	| 1,004,902 			| 925,722 				| 1,081,370 			|
| Lattice		| SMAUG-256				| 1,513,984  		 	| 1,566,464 	 		| 1,759,488 	 		| 1,536,512 			| 1,570,304 			| 1,792,051 			|
| Lattice		| TiGER-128				| 1,082,752  		 	| 1,207,808   			| 1,416,448   		 	| 1,098,650 			| 1,223,501 			| 1,454,182 			|
| Lattice		| TiGER-192				| ~~663,296~~ 		 		| ~~917,760~~		 		| ~~1,122,176~~ 				| ~~672,051~~ 				| ~~927,437~~ 			| ~~926,834~~ 			|
| Lattice		| TiGER-256				| 845,952 		 		| 1,295,616		 		| 1,678,336	 		 	| 854,886   			| 1,300,634 			| 1,689,344 			|
| Code			| PALOMA-128			| 612,124,288  	 		| 3,070,592   			| 775,680  		 		| 613,487,718  			| 3,077,555  			| 851,226 				|
| Code			| PALOMA-192			| 718,675,840  	 		| 3,081,216  			| 807,936  		 		| 950,287,744  			| 3,089,613 			| 822,221  				|
| Code			| PALOMA-256			| 612,304,128  	 		| 3,188,992 			| 588,288 			 	| 614,168,090  			| 4,019,738  			| 643,200 				|
| Code			| IPCC-1  				| 74,656,000  	 		| 451,377,408  			| 13,834,752  			| 74,756,173  			| 738,162,432  			| 16,646,272  			|
| Code			| IPCC-3		 		| 74,141,312  	 		| 3,920,640  	 		| 12,512,896  			| 74,213,683 			| 33,329,254  			| 13,876,275  			|
| Code			| IPCC-4				| 74,782,080  	 		| 4,969,216  			| 12,011,648  			| 74,940,877  			| 21,339,136  			| 13,409,920  			|
</details>

### Digital Signature (Environment1, -O2)
<details>
<summary>Digital Signature-Env1-O2 Table (Unit: clock cycles)</summary>
    
| Type					| Algorithm     		| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 		| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 64,844  			| 3,214,955  		| 2,592,455 			| 68,995 			| 3,257,540  		| 2,683,305  		|
| Zero-knowledge		| AIMer-I-param2    	| 64,206   			| 6,844,783   		| 6,397,821  			| 64,845 			| 6,852,149 		| 6,402,617 		|
| Zero-knowledge		| AIMer-I-param3    	| 196,968  			| 21,907,209   		| 21,271,413 			| 180,191 			| 22,746,073  		| 21,333,772 		|
| Zero-knowledge		| AIMer-I-param4    	| 197,084  			| 112,105,141  		| 110,507,502  			| 209,231 			| 112,773,321 		| 110,496,781 		|
| Zero-knowledge		| AIMer-III-param1    	| 327,990  			| 7,423,203 		| 6,260,694  			| 313,110 			| 7,994,873 		| 6,270,172 		|
| Zero-knowledge		| AIMer-III-param2    	| 261,638  			| 17,831,955   		| 17,154,936 			| 266,723 			| 18,682,928 		| 17,595,524 		|
| Zero-knowledge		| AIMer-III-param3    	| 261,754  			| 53,637,298  		| 52,191,257  			| 297,101 			| 54,790,721 		| 52,882,465 		|
| Zero-knowledge		| AIMer-III-param4    	| 356,860  			| 258,710,378   	| 253,439,135  			| 340,142 			| 262,793,446 		| 253,809,391 		|
| Zero-knowledge		| AIMer-V-param1    	| 738,355 			| 14,246,874 		| 12,776,516 			| 811,474 			| 14,722,314  		| 12,826,943  		|
| Zero-knowledge		| AIMer-V-param2    	| 737,572  			| 34,973,565 		| 33,936,612 			| 691,135 			| 36,019,115 		| 33,942,184 		|
| Zero-knowledge		| AIMer-V-param3    	| 240,251  			| 104,590,385 		| 101,833,457   		| 256,019  			| 105,383,550  		| 102,296,025 		|
| Zero-knowledge		| AIMer-V-param4    	| 240,352 			| 501,582,289 		| 491,070,862 			| 245,309 			| 504,155,885 		| 492,179,959 		|
| Multivariate Quadratic| MQSign-72/46			| 94,788,559 		| 516,954 			| 1,461,281 			| 94,829,257		| 518,651			| 1,465,923			|
| Multivariate Quadratic| MQSign-112/72			| 488,913,828 		| 1,493,703 		| 5,211,909 			| 490,448,324		| 1,513,132			| 5,258,218			|
| Multivariate Quadratic| MQSign-148/96			| 1,488,480,956 	| 3,162,943 		| 12,036,827 			| 1,488,377,972		| 3,164,654			| 12,041,118		|
| Lattice				| GCKSign-II			| 179,771 			| 601,707 			| 176,987 				| 181,822			| 848,504			| 178,229			|
| Lattice				| GCKSign-III			| 186,673 			| 649,049 			| 183,367 				| 198,852			| 899,646			| 185,793			|
| Lattice				| GCKSign-V				| 252,822 			| 917,415 			| 277,733 				| 255,206			| 1,099,271			| 284,217			|
| Lattice				| HAETAE-II				| 798,312 			| 4,605,461 		| 147,494 				| 1,091,637			| 5,704,780			| 148,078			|
| Lattice				| HAETAE-III			| ~~1,533,941~~ 	| ~~11,474,155~~ 	| 257,926 				| ~~2,127,683~~		| ~~12,068,749~~	| 259,846			|
| Lattice				| HAETAE-V				| 846,713 			| 3,902,298 		| 305,428 				| 1,104,472			| 5,214,861			| 306,973			|
| Lattice				| NCCSign-I(original)	| 1,869,079 		| 23,762,252 		| 3,681,057 			| 1,882,892 		| 23,763,293		| 3,684,640			|
| Lattice				| NCCSign-III(original)	| 3,655,334 		| 39,587,190 		| 7,241,808 			| 3,675,996 		| 39,635,337		| 7,246,465			|
| Lattice				| NCCSign-V(original)	| 6,263,739 		| 179,281,596 		| 12,418,902 			| 6,268,503 		| 179,337,534		| 12,422,702		|
| Lattice				| NCCSign-I(conserparam)| 2,650,542 		| 10,404,301 		| 5,232,079 			| 2,670,083 		| 10,419,012		| 5,244,741			|
| Lattice				| NCCSign-III(conserparam)| 4,477,513 		| 17,657,839 		| 8,867,243 			| 4,497,436 		| 17,666,605		| 8,869,094			|
| Lattice				| NCCSign-V(conserparam)| 7,240,343 		| 64,377,767 		| 14,358,074 			| 7,257,655 		| 64,387,183		| 14,375,040		|
| Lattice				| Peregrine-512 		| 12,401,256 		| 329,933 			| 37,294 				| 12,609,569		| 332,600			| 37,505			|
| Lattice				| Peregrine-1024		| 39,405,505 		| 709,848 			| 80,243 				| 42,160,344		| 722,426			| 81,200			|
| Lattice				| SOLMAE-512			| 23,848,774		| 378,392			| 43,935				| 29,181,985 		| 385,719			| 44,109			|
| Lattice				| SOLMAE-1024			| 55,350,546		| 760,380			| 141,375				| 70,141,847 		| 764,304			| 142,357			|
| Code					| Enhanced pqsigRM-612	| 6,013,112,315 	| 7,210,560 		| 2,223,401 			| 5,970,970,554		| 9,823,994			| 2,303,399			|
| Code					| Enhanced pqsigRM-613	| 58,238,108,879	| 1,864,512 		| 1,053,034 			| 58,669,322,672	| 2,650,133			| 1,064,763			|
</details>

### Digital Signature (Environment1, -O3)
<details>
<summary>Digital Signature-Env1-O3 Table (Unit: clock cycles)</summary>
    
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 147,059  			| 3,213,041  		| 2,648,860 			| 163,284 			| 3,935,749  		| 2,786,447  		|
| Zero-knowledge		| AIMer-I-param2    	| 115,870  			| 6,846,958   		| 6,512,008  			| 115,683 			| 7,396,703 		| 6,687,261 		|
| Zero-knowledge		| AIMer-I-param3    	| 74,284   			| 22,706,812   		| 21,993,890 			| 76,777 			| 23,169,641 		| 22,295,683 		|
| Zero-knowledge		| AIMer-I-param4    	| 64,453  			| 110,355,455  		| 107,146,199  			| 65,526 			| 110,667,942  		| 107,713,292 		|
| Zero-knowledge		| AIMer-III-param1    	| 116,116  			| 7,694,773 		| 6,410,653  			| 120,704 			| 7,780,833 		| 6,488,682 		|
| Zero-knowledge		| AIMer-III-param2    	| 116,145 			| 18,202,285    	| 17,165,796  			| 119,301  			| 18,388,655 		| 17,252,906 		|
| Zero-knowledge		| AIMer-III-param3    	| 116,551  			| 53,836,586  		| 52,181,817  			| 122,702 			| 54,514,121  		| 53,041,857 		|
| Zero-knowledge		| AIMer-III-param4    	| 356,860   		| 258,266,315    	| 253,500,513  			| 359,030  			| 261,570,537 		| 253,557,101 		|
| Zero-knowledge		| AIMer-V-param1    	| 610,871  			| 13,023,538 		| 11,387,575 			| 639,510 			| 13,639,884  		| 11,622,084  		|
| Zero-knowledge		| AIMer-V-param2    	| 238,728 			| 31,846,684 		| 30,674,402 			| 241,070 			| 32,085,621 		| 30,810,554 		|
| Zero-knowledge		| AIMer-V-param3    	| 655,096   		| 95,848,132		| 92,529,097   			| 650,057  			| 96,079,292  		| 92,629,544 		|
| Zero-knowledge		| AIMer-V-param4    	| 733,294 			| 460,721,275 		| 450,173,931 			| 689,891 			| 461,929,400 		| 450,233,261 		|
| Multivariate Quadratic| MQSign-72/46 				| 39,040,917 		| 311,112 			| 512,227 				| 39,057,616 		| 312,293 			| 514,042			|
| Multivariate Quadratic| MQSign-112/72 			| 115,942,827 		| 669,465 			| 1,143,296 			| 116,040,569 		| 672,499 			| 1,147,066			|
| Multivariate Quadratic| MQSign-148/96 			| 235,289,035 		| 1,186,622 		| 1,943,667 			| 235,425,321 		| 1,190,984 		| 1,952,355			|
| Lattice				| GCKSign-II 				| 164,836 			| 537,675 			| 159,674 				| 175,216 			| 765,476 			| 160,447			|
| Lattice				| GCKSign-III 				| 166,199 			| 581,189 			| 161,646 				| 180,908 			| 806,260 			| 162,452			|
| Lattice				| GCKSign-V 				| 231,797 			| 895,549 			| 279,009 				| 242,850 			| 1,068,798 		| 280,118			|
| Lattice				| HAETAE-II 				| 688,083 			| 3,429,265 		| 131,805 				| 957,268 			| 4,247,185 		| 132,462			|
| Lattice				| HAETAE-III 				| ~~1,329,157~~		| ~~8,734,670~~ 	| 228,578 				| ~~1,843,459~~		| ~~9,183,604~~		| 229,703			|
| Lattice				| HAETAE-V 					| 723,318 			| 2,790,612 		| 272,542 				| 946,202 			| 3,700,449 		| 273,840			|
| Lattice				| NCCSign-II(original) 		| 1,843,356 		| 50,520,712 		| 3,636,803 			| 1,860,128 		| 50,540,655 		| 3,643,639			|
| Lattice				| NCCSign-III(original) 	| 3,618,997 		| ~~21,416,384~~	| 7,170,903 			| 3,646,841 		| ~~21,437,009~~	| 7,169,406			|
| Lattice				| NCCSign-V(original) 		| 6,149,059 		| 151,973,282 		| 12,196,791 			| 6,162,746 		| 152,011,122 		| 12,213,326		|
| Lattice				| NCCSign-II(conserparam)	| 2,619,295 		| 10,301,902 		| 5,171,686 			| 2,639,164 		| 10,308,375 		| 5,175,958			|
| Lattice				| NCCSign-III(conserparam)	| 4,379,261 		| ~~86,475,941~~	| 8,685,877 			| 4,405,049 		| ~~86,515,726~~	| 8,685,125			|
| Lattice				| NCCSign-V(conserparam) 	| 7,178,921 		| 42,637,366 		| 14,245,148 			| 7,194,274 		| 42,681,718 		| 14,247,358		|
| Lattice				| Peregrine-512 			| 11,953,307 		| 253,402 			| 25,462 				| 12,146,320 		| 254,228 			| 25,634			|
| Lattice				| Peregrine-1024 			| 38,366,232 		| 535,920 			| 53,621 				| 41,014,591 		| 538,260 			| 53,946			|
| Lattice				| SOLMAE-512 				| 23,053,028 		| 349,566 			| 40,513 				| 28,233,370 		| 355,950 			| 40,812			|
| Lattice				| SOLMAE-1024 				| 53,966,332 		| 698,581 			| 135,256 				| 68,603,714 		| 702,006 			| 136,193			|
| Code					| Enhanced pqsigRM-613 		| 6,139,551,981 	| 4,610,319 		| 2,278,806 			| 6,144,274,759 	| 6,276,554 		| 2,376,095			|
| Code					| Enhanced pqsigRM-612 		| 54,994,439,928 	| 714,647 			| 225,577 				| 55,073,661,751 	| 967,439 			| 234,553			|
</details>

### Digital Signature (Environment2, -O2)
<details>
<summary>Digital Signature-Env2-O2 Table (Unit: clock cycles)</summary>
    
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 63,933   			| 3,600,037  		| 2,793,252  			| 72,446  			| 3,888,113  		| 3,056,785  		|
| Zero-knowledge		| AIMer-I-param2    	| 63,375  			| 6,728,396   		| 6,558,491   			| 74,279 			| 7,104,050 		| 6,950,607 		|
| Zero-knowledge		| AIMer-I-param3    	| 120,915  			| 22,519,566   		| 25,863,759 			| 119,094  			| 24,752,954 		| 26,935,070 		|
| Zero-knowledge		| AIMer-I-param4    	| 84,224   			| 123,016,593   	| 108,085,442  			| 86,537 			| 126,913,831  		| 108,341,338 		|
| Zero-knowledge		| AIMer-III-param1    	| 114,714  			| 7,621,547 		| 6,673,509 			| 140,583 			| 7,988,598 		| 7,168,685 		|
| Zero-knowledge		| AIMer-III-param2    	| 128,547  			| 22,253,030    	| 21,664,782   			| 148,248  			| 22,709,547  		| 23,870,182 		|
| Zero-knowledge		| AIMer-III-param3    	| 113,296 			| 66,283,625  		| 61,548,453  			| 139,246			| 69,010,295 		| 63,563,735  		|
| Zero-knowledge		| AIMer-III-param4    	| 149,336   		| 266,763,260    	| 258,358,771 			| 168,673  			| 286,982,536  		| 257,978,901  		|
| Zero-knowledge		| AIMer-V-param1    	| 238,347   		| 15,905,602 		| 16,312,363 			| 274,861 			| 17,713,665  		| 17,977,624 		|
| Zero-knowledge		| AIMer-V-param2    	| 241,737 			| 44,685,871 		| 44,863,206  			| 275,707  			| 43,701,933 		| 45,656,757 		|
| Zero-knowledge		| AIMer-V-param3    	| 299,868    		| 108,834,414 		| 106,217,782   		| 331,971 			| 113,095,802  		| 118,492,622 		|
| Zero-knowledge		| AIMer-V-param4    	| 283,645  			| 543,695,192 		| 513,685,715 			| 299,150 			| 556,632,059 		| 514,419,680 		|
| Multivariate Quadratic| MQSign-72/46 				| 87,038,447 		| 509,630 			| 1,377,392 			| 87,156,508 		| 527,234 			| 1,411,202			|
| Multivariate Quadratic| MQSign-112/72 			| 448,271,119 		| 1,472,032 		| 4,808,216 			| 448,141,266 		| 1,500,297 		| 4,875,532			|
| Multivariate Quadratic| MQSign-148/96 			| 1,326,638,494 	| 3,128,536 		| 11,091,036 			| 1,328,649,536 	| 3,150,219 		| 11,143,601		|
| Lattice				| GCKSign-II 				| 171,176 			| 640,093 			| 167,116 				| 190,739 			| 845,502 			| 173,676			|
| Lattice				| GCKSign-III 				| 173,252 			| 698,964 			| 168,824 				| 185,265 			| 943,376 			| 176,768			|
| Lattice				| GCKSign-V 				| 248,629 			| 945,815 			| 273,631 				| 264,811 			| 1,151,316 		| 282,979			|
| Lattice				| HAETAE-II 				| 700,875 			| 4,173,002 		| 142,584 				| 979,130 			| 5,274,158 		| 150,759			|
| Lattice				| HAETAE-III 				| ~~1,352,577~~		| ~~10,615,663~~	| 250,534 				| ~~1,940,364~~		| ~~11,445,286~~	| 262,470			|
| Lattice				| HAETAE-V 					| 752,413 			| 3,418,728 		| 311,986 				| 983,382 			| 4,622,966 		| 328,866			|
| Lattice				| NCCSign-II(original) 		| 1,704,190 		| 27,083,021 		| 3,344,228 			| 1,799,742 		| 27,354,886 		| 3,460,388			|
| Lattice				| NCCSign-III(original) 	| 3,271,119 		| ~~65,455,745~~	| 6,533,931 			| 3,402,118 		| ~~65,582,525~~	| 6,586,857			|
| Lattice				| NCCSign-V(original) 		| 5,723,169 		| 39,565,842 		| 11,290,884 			| 6,088,747 		| 39,658,546 		| 11,384,040		|
| Lattice				| NCCSign-II(conserparam) 	| 2,296,351 		| 15,914,954 		| 4,519,308 			| 2,412,156 		| 16,002,740 		| 4,622,316			|
| Lattice				| NCCSign-III(conserparam) 	| 4,009,717 		| 16,015,734 		| 7,996,462 			| 4,169,703 		| 16,116,734 		| 8,085,000			|
| Lattice				| NCCSign-V(conserparam) 	| 6,561,582 		| 26,019,063 		| 13,005,536 			| 6,639,348 		| 26,080,187 		| 13,084,234		|
| Lattice				| Peregrine-512 			| 12,073,005 		| 295,128 			| 33,114 				| 12,299,755		| 305,264 			| 35,943			|
| Lattice				| Peregrine-1024 			| 38,493,479 		| 640,132 			| 71,246 				| 41,112,188		| 652,620 			| 74,891			|
| Lattice				| SOLMAE-512 				| 22,494,902 		| 351,311 			| 64,526 				| 27,556,843 		| 366,508 			| 68,880			|
| Lattice				| SOLMAE-1024 				| 52,388,360 		| 706,028 			| 152,984 				| 65,688,581 		| 729,400 			| 158,540			|
| Code					| Enhanced pqsigRM-613 		| 4,961,556,899 	| 7,505,040 		| 2,125,125 			| 4,973,260,518 	| 10,823,438 		| 2,645,728			|
| Code					| Enhanced pqsigRM-612 		| 74,021,054,015 	| 2,113,913 		| 1,126,131 			| 73,941,690,821 	| 2,765,068 		| 1,295,161			|
</details>

### Digital Signature (Environment2, -O3)
<details>
<summary>Digital Signature-Env2-O3 Table (Unit: clock cycles)</summary>
    
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 78,212   			| 3,799,913 		| 3,125,105  			| 94,251  			| 4,000,548 		| 3,318,324   		|
| Zero-knowledge		| AIMer-I-param2    	| 62,013   			| 8,202,991   		| 7,719,642 			| 67,545 			| 8,623,032  		| 8,299,636 		|
| Zero-knowledge		| AIMer-I-param3    	| 59,949   			| 28,891,133   		| 26,007,825  			| 73,077  			| 29,338,289 		| 25,865,036  		|
| Zero-knowledge		| AIMer-I-param4    	| 59,132   			| 125,595,827   	| 111,966,098  			| 73,953  			| 127,933,918 		| 118,860,248 		|
| Zero-knowledge		| AIMer-III-param1    	| 150,830 			| 9,550,533 		| 8,247,989  			| 167,334 			| 9,563,875			| 8,456,415  		|
| Zero-knowledge		| AIMer-III-param2    	| 111,755   		| 22,535,406    	| 22,149,494    		| 131,077  			| 22,879,148   		| 22,107,888 		|
| Zero-knowledge		| AIMer-III-param3    	| 109,995  			| 67,033,442 		| 54,668,451  			| 136,612 			| 66,512,279 		| 57,154,915  		|
| Zero-knowledge		| AIMer-III-param4    	| 141,443   		| 271,749,297     	| 266,189,006 			| 165,637  			| 284,501,178 		| 267,296,086  		|
| Zero-knowledge		| AIMer-V-param1    	| 255,129    		| 14,687,409 		| 13,724,601 			| 295,481 			| 16,357,037 		| 15,402,041  		|
| Zero-knowledge		| AIMer-V-param2    	| 285,212  			| 43,671,802 		| 34,428,290 			| 294,113  			| 43,211,961		| 35,975,358  		|
| Zero-knowledge		| AIMer-V-param3    	| 290,019    		| 115,523,707 		| 109,326,832   		| 292,918 			| 122,646,177  		| 109,994,189  		|
| Zero-knowledge		| AIMer-V-param4    	| 243,628   		| 557,052,072  		| 521,735,231 			| 313,602  			| 581,221,074 		| 526,630,666  		|	
| Multivariate Quadratic| MQSign-72/46 				| 38,474,591 		| 298,952 			| 533,676 				| 38,612,360 		| 308,203 			| 547,680			|
| Multivariate Quadratic| MQSign-112/72 			| 117,049,542 		| 650,928 			| 1,120,124 			| 117,234,338 		| 667,681 			| 1,147,333			|
| Multivariate Quadratic| MQSign-148/96 			| 236,124,011 		| 1,165,706 		| 1,897,664 			| 236,332,422 		| 1,173,558 		| 1,908,458			|
| Lattice				| GCKSign-II 				| 175,993 			| 597,712 			| 172,893 				| 188,999 			| 869,677 			| 182,127			|
| Lattice				| GCKSign-III 				| 183,987 			| 698,941 			| 179,608 				| 223,689 			| 976,483 			| 186,837			|
| Lattice				| GCKSign-V 				| 238,884 			| 928,251 			| 262,868 				| 259,401 			| 1,228,167 		| 293,133			|
| Lattice				| HAETAE-II 				| 672,901 			| 3,334,242 		| 126,972 				| 944,910 			| 4,200,552 		| 132,300			|
| Lattice				| HAETAE-III  				| ~~1,291,292~~		| ~~8,261,232~~    	| 227,780 				| ~~1,828,235~~		| ~~8,769,910~~		| 238,478			|
| Lattice				| HAETAE-V 					| 719,708 			| 2,627,334 		| 270,600 				| 973,865 			| 3,546,813 		| 280,493			|
| Lattice				| NCCSign-II(original) 		| 1,666,543 		| 16,352,341 		| 3,248,162 			| 1,846,947 		| 16,530,887 		| 3,321,373			|
| Lattice				| NCCSign-III(original) 	| 3,141,974 		| 34,454,252 		| 6,234,249 			| 3,227,617 		| 34,523,301 		| 6,288,505			|
| Lattice				| NCCSign-V(original) 		| 5,613,303 		| 167,158,023 		| 11,155,020 			| 5,851,360 		| 167,337,719 		| 11,307,818		|
| Lattice				| NCCSign-II(conserparam) 	| 2,317,555 		| 13,776,448 		| 4,568,006 			| 2,393,641 		| 13,868,809 		| 4,647,302			|
| Lattice				| NCCSign-III(conserparam) 	| 3,981,551 		| ~~83,521,123~~	| 7,935,382 			| 4,209,101 		| ~~83,634,184~~	| 8,001,129			|
| Lattice				| NCCSign-V(conserparam) 	| 6,333,006 		| 25,183,392 		| 12,555,623 			| 6,470,472 		| 25,269,299 		| 12,680,799		|
| Lattice				| Peregrine-512 			| 11,783,005 		| 260,328 			| 26,262 				| 12,032,320 		| 269,678 			| 28,484			|
| Lattice				| Peregrine-1024 			| 37,875,534 		| 551,168 			| 55,654 				| 40,364,494 		| 569,794 			| 58,474			|
| Lattice				| SOLMAE-512 				| 22,627,042 		| 332,848 			| 64,838 				| 27,866,035 		| 348,841 			| 67,662			|
| Lattice				| SOLMAE-1024 				| 53,245,753 		| 668,103 			| 149,168 				| 67,369,725 		| 686,523		 	| 154,073			|
| Code					| Enhanced pqsigRM-613 		| 4,702,612,115 	| 4,732,706 		| 2,064,731 			| 4,703,836,987 	| 6,667,564 		| 2,458,625			|
| Code					| Enhanced pqsigRM-612 		| 71,111,088,778 	| 923,513 			| 417,658 				| 71,168,430,985 	| 1,166,665 		| 502,448			|
</details>

### Digital Signature (Environment3, -O3)
<details>
<summary>Digital Signature-Env3-O3 Table (Unit: nsec)</summary>
    
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 44,032    			| 899,968 		| 647,040 				| 48,655  			| 1,047,603 		| 651,395   		|
| Zero-knowledge		| AIMer-I-param2    	| 52,992   			| 1,698,048   		| 1,593,472 			| 54,157 			| 1,909,775   		| 1,605,245  		|
| Zero-knowledge		| AIMer-I-param3    	| 52,992   			| 5,171,584    		| 5,123,072 			| 54,638 			| 5,427,658 		| 5,137,475 		|
| Zero-knowledge		| AIMer-I-param4    	| 52,992    		| 25,836,800    	| 25,750,528   			| 54,533  			| 26,125,240 		| 25,846,420  		|
| Zero-knowledge		| AIMer-III-param1    	| 94,976			| 1,921,536  		| 1,597,952  			| 90,442 			| 2,120,712 		| 1,611,597  		|
| Zero-knowledge		| AIMer-III-param2    	| 94,976   			| 4,444,160     	| 4,254,080     		| 91,876 			| 4,682,583    		| 4,271,025			|
| Zero-knowledge		| AIMer-III-param3    	| 90,496 			| 12,902,016 		| 12,858,624   			| 88,392			| 13,133,614  		| 12,907,110  		|
| Zero-knowledge		| AIMer-III-param4    	| 77,056 	  		| 63,625,472      	| 63,475,584 			| 85,435   			| 63,923,018 		| 63,566,275  		|
| Zero-knowledge		| AIMer-V-param1    	| 150,016     		| 3,278,976 		| 2,849,408  			| 156,132 			| 3,461,350 		| 2,870,072   		|
| Zero-knowledge		| AIMer-V-param2    	| 151,040  			| 7,632,128 		| 7,357,952 			| 165,619   		| 7,812,086 		| 7,382,221  		|
| Zero-knowledge		| AIMer-V-param3    	| 80,128 	  		| 22,604,416 		| 22,692,992   			| 87,229			| 22,887,040   		| 22,764,360  		|
| Zero-knowledge		| AIMer-V-param4    	| 150,016 			| 110,281,984  		| 110,957,440 			| 168,602   		| 110,695,219  		| 111,235,878   	|
| Multivariate Quadratic| MQSign-72/46 				| 25,404,416 		| 140,032 			| 431,104 				| 25,694,354 		| 140,605 			| 436,836 			|
| Multivariate Quadratic| MQSign-112/72 			| 135,705,472 		| 380,160			| 1,565,952 			| 136,245,245 		| 385,275 			| 1,575,908 		|
| Multivariate Quadratic| MQSign-148/96 			| 411,995,904  		| 795,136 			| 3,660,416 			| 412,772,841  		| 802,437  			| 3,674,153 		|
| Lattice				| GCKSign-II 				| 102,912 			| 254,976  			| 50,944 				| 108,541			| 320,015  			| 51,244 			|
| Lattice				| GCKSign-III 				| 108,032  			| 238,080  			| 54,016 				| 118,446 			| 315,105 			| 54,879			|
| Lattice				| GCKSign-V 				| 142,080  			| 264,576 			| 75,008 				| 144,274  			| 352,781 	 		| 75,077 			|
| Lattice				| HAETAE-II 				| 357,120  			| 208,384 			| 40,960 				| 485,315 			| 367,772 			| 43,341 			|
| Lattice				| HAETAE-III  				| 428,544 			| 450,432     		| 72,960				| 718,948 			| 656,292			| 75,410 			|
| Lattice				| HAETAE-V 					| 403,968  			| 432,000  			| 91,136  				| 486,986 			| 618,040			| 94,843			|
| Lattice				| Peregrine-512 			| 3,891,456 		| 45,056  			| 5,120					| 4,175,012 		| 45,507 			| 5,179 			|
| Lattice				| Peregrine-1024 			| 13,422,464  		| 93,952 			| 9,984 				| 14,907,274 		| 95,708 			| 10,150 			|
| Code					| Enhanced pqsigRM-613 		| 793,679,488  		| 2,152,960  		| 379,392				| 793,098,163 		| 2,555,494 		| 405,683 			|
| Code					| Enhanced pqsigRM-612 		| 10,549,565,568 	| 329,600 			| 72,064  				| 10,564,255,590 	| 271,821 	 		| 76,672 			|
</details>

### Digital Signature (Environment4, -O3)
<details>
<summary>Digital Signature-Env4-O3 Table (Unit: nsec)</summary>
    
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I-param1    	| 1,358,080    		| 67,787,904 		| 58,651,648 			| 1,399,091  		| 69,619,482 		| 58,922,189   		|
| Zero-knowledge		| AIMer-I-param2    	| 581,120  			| 66,993,152   		| 149,871,872  			| 606,259 			| 67,316,250   		| 145,416,013  		|
| Zero-knowledge		| AIMer-I-param3    	| 580,736   		| 316,050,816     	| 209,649,536			| 610,048 			| 345,565,875 		| 262,197,939 		|
| Zero-knowledge		| AIMer-I-param4    	| 587,904 	 		| 1,163,266,688    	| 1,706,329,600 		| 608,538 			| 1,441,277,440 	| 1,631,551,386 	|
| Zero-knowledge		| AIMer-III-param1    	| 1,310,848 		| 71,054,080 		| 64,191,744 			| 1,334,861 		| 71,649,562  		| 64,446,259 		|
| Zero-knowledge		| AIMer-III-param2    	| 1,292,672 		| 174,505,600     	| 393,859,584     		| 1,335,168 		| 191,038,259  		| 338,752,896 		|
| Zero-knowledge		| AIMer-III-param3    	| 1,293,696 		| 519,604,736 		| 513,189,632    		| 1,318,144 		| 701,901,005 		| 698,305,229  		|
| Zero-knowledge		| AIMer-III-param4    	| 1,315,200  		| 3,450,891,392    	| 2,548,476,928 		| 1,352,781   		| 3,470,096,998 	| 2,548,539,571 	|
| Zero-knowledge		| AIMer-V-param1    	| 3,120,512   		| 133,866,112  		| 123,565,568 			| 3,136,128 		| 137,323,827  		| 128,562,278  		|
| Zero-knowledge		| AIMer-V-param2    	| 7,403,904			| 322,445,696 		| 315,369,728 			| 8,194,048 		| 483,219,456 		| 316,895,130  		|
| Zero-knowledge		| AIMer-V-param3    	| 3,066,240   		| 1,052,067,712 	| 956,340,096    		| 3,117,184 		| 1,335,187,994 	| 1,327,519,309 	|
| Zero-knowledge		| AIMer-V-param4    	| 3,068,288 		| 5,893,699,712 	| 6,630,984,448 		| 3,108,198 		| 5,809,457,280		| 6,613,736,576 	|
| Multivariate Quadratic| MQSign-72/46 				| 201,938,688  		| 1,364,224 		| 3,089,152 			| 202,309,734 		| 1,387,392 		| 3,108,685 		|
| Multivariate Quadratic| MQSign-112/72 			| 985,279,488 		| 3,718,912 		| 10,791,296 			| 987,524,557 		| 3,754,573 		| 10,818,022  		|
| Multivariate Quadratic| MQSign-148/96 			| 4,625,720,064   	| 17,148,032 		| 55,782,656 			| 4,062,789,222 	| 17,195,520 		| 55,945,190 		|
| Lattice				| HAETAE-II 				| 3,960,448   		| 9,362,432  		| 904,576  				| 6,145,664 		| 17,819,392 		| 973,363 			|
| Lattice				| HAETAE-III  				| ~~10,779,648~~ 		| ~~78,606,976~~  		| ~~3,268,096~~				| ~~10,816,486~~ 		| ~~62,431,360~~ 		| ~~3,280,742~~ 		|
| Lattice				| HAETAE-V 					| 4,497,920			| 29,362,432 		| 1,539,072 			| 5,275,443 		| 30,645,555 		| 1,619,354 		|
| Lattice				| Peregrine-512 			| 32,932,864  		| 1,204,992 		| 120,576 				| 33,412,736 		| 1,261,184 		| 126,310 			|
| Lattice				| Peregrine-1024 			| 102,701,440  		| 2,625,920 		| 260,352 				| 118,987,059 		| 2,656,102 		| 270,925 			|
| Code					| Enhanced pqsigRM-613 		| 60,258,585,088  	| 107,284,480  		| 18,758,144 			| 60,246,444,203  	| 110,248,619 		| 17,735,595 		|
| Code					| Enhanced pqsigRM-612 		| N/A 				| N/A 				| N/A 					| N/A 				| N/A 		 		| N/A 				|
</details>

## Memory consumption
We checked for memory consumption using the [Valgrind](https://valgrind.org/) tool.

The experiment used Valgrind's [massif](https://valgrind.org/docs/manual/ms-manual.html) tool.

* Command used for testing: valgrind --tool=massif --stacks=yes ./PQC_bench
* Our records were based on comparing the largest heap usage and the largest stack usage in the Valgrind logs, choosing the larger of the two values as the foundation.
  
<details>
<summary>Show memory consumption tables (Unit: bytes) </summary>

### PKE/KEM
| Algorithm         | Stack+Heap+Extra_Heap | Stack         | Heap          | Extra_Heap    |  
| --------------    | ---------------------:| -------------:| -------------:| -------------:|
| IPCC-f1		    | 735168                | 	      733160|	       1968 |            40 |
| IPCC-f3		    | 791168                | 	      789160|	       1968 |            40 |
| IPCC-f4		    | 805208                | 	     803200 |	       1968 |            40 |
| NTRUplus-576	    | 18152                 | 	      16632 |	       1496 |            24 |
| NTRUplus-768	    | 23272                 | 	      21752 |	       1496 |            24 |
| NTRUplus-864	    | 25832  	            |         24312 |	       1496 |            24 |
| NTRUplus-1152	    | 33480                 | 	      31960 |	       1496 |            24 |		
| PALOMA-128		| 16644096              | 	   16641600 |	       2440 |            56 |
| PALOMA-192		| 16644096              | 	   16641600 |	       2440 |            56 |
| PALOMA-256		| 16644096              | 	   16641600 |	       2440 |            56 |		
| Layered ROLLO-128 | 27344                 | 	       9160 |	      17900 |           284 |
| Layered ROLLO-192 | 31160                 | 	      10368 |	      20516 |           276 |
| Layered ROLLO-256 | 45696                 |         17128 |	      28292 |           276 |		
| SMAUG-128		    | 12128                 | 	      10608 |	       1496 |            24 |
| SMAUG-192		    | 19184                 | 	      17664 |          1496 |	         24 |
| SMAUG-256		    | 37768                 | 	      36248 |	       1496 |            24 |	
| TiGER-128		    | 12488                 | 	      10968 |	       1496 |            24 |
| TiGER-192		    | 20776                 | 	      19256 |	       1496 |            24 |
| TiGER-256		    | 21432                 | 	      19912 |          1496 |	         24 |

### Digital Signature
| Algorithm                 |  Stack+Heap+Extra_Heap | Stack         | Heap          | Extra_Heap    |  
| -----------------------   | ---------------------: | -------------:| -------------:|--------------:|
|AIMer-l1-param1            | 201968	             |7936	         | 191048        | 2984          |
|AIMer-l1-param2            | 451888		         |7920	         | 441606        | 2362          |
|AIMer-l1-param3            | 1426072		         |7840           | 1416710       | 1522          |
|AIMer-l1-param4            | 6896664		         |7840           | 6887572       | 1522          |
|AIMer-l3-param1            | 438496		         |15248        	 | 417768        | 5480          |
|AIMer-l3-param2            | 1066392		         |15248	         | 1047512       | 3632          |
|AIMer-l3-param3            | 3135808		         |15568	         | 3117478       | 2762          |
|AIMer-l3-param4            | 15036584		         |15168	         | 15018816      | 2600          |
|AIMer-l5-param1            | 877208		         |27408	         | 844272        | 5528          |
|AIMer-l5-param2            | 2117488		         |27408	         | 2086088       | 3992          |
|AIMer-l5-param3            | 6315904		         |27328	         | 6285870       | 2706          |
|AIMer-l5-param4            | 30403744		         |27328	         | 30373596      | 2820          |	
|GCKSign-II	                | 47544	                 |44568        	 |2928           |             48|
|GCKSign-III                | 48056	                 |45080	         |2928           |             48|
|GCKSign-V	                | 72136	                 |69160	         |2928           |             48|		
|HAETAE-II	                | 90568	                 |88560	         |1968           |             40|
|HAETAE-III	                | 136904                 |134896	     | 1968          |             40|
|HAETAE-V	                | 172392	             |170384	     | 1968          |             40|
|MQSign-72_46               | 1248392	             |1245896	     | 2440          |             56|
|MQSign-112_72	            | 4677064	             |4674568	     | 2440          |             56|
|MQSign-148_96	            | 10908232	             |10905736	     | 2440          |             56|
|NCCSign-II (conserparam)	| 224352                 |222344	     | 1968          |             40|
|NCCSign-III (conserparam)	| 296816	             |294808	     | 1968          |             40|
|NCCSign-V (conserparam)	| 374992	             |372984	     | 1968          |             40|
|NCCSign-II (original)	    | 189168	             |187160	     | 1968          |             40|
|NCCSign-III (original)	    | 264384	             |262376	     | 1968          |             40|
|NCCSign-V (original)	    | 351360	             |349352	     | 1968          |             40|
|Peregrine-512	            | 119528	             |3008	         | 116356        |            164|
|Peregrine-1024	            | 230632	             |3008	         | 227470        |            154|
|Enhanced pqsigRM-612	    | 6624984	             |1617888	     | 5006784       |            312|
|Enhanced pqsigRM-613	    | 29426240               |4247296	     | 25167600      |          11344|
|SOLMAE-512		            | 129336	             |126840	     | 2440          |             56|
|SOLMAE-1024	            | 255240	             |252744	     | 2440          |             56|


* REDOG: Valgrind only targets programs written in C language. REDOG could not be tested because it is only provided in python.
* FIBS: The algorithm calculation has not been completed. So we couldn't test.
</details>
  



## Memory leak detection
We checked for memory leaks using the Valgrind tool.

The experiment method followed the [memory leak detection method](https://valgrind.org/docs/manual/quick-start.html) provided by Valgrind.

The experiment was conducted in Environment 2, and the number of algorithm repetitions was fixed to 1 when testing for memory leaks.

As a result of the inspection, memory leaks were found in **three** algorithms.
More details can be found in the valgrind report.
* Command used for testing: valgrind --leak-check=full --track-origins=yes --log-file=_valgrind_report.txt ./PQC_bench
* REDOG: Valgrind only targets programs written in C language. REDOG could not be tested because it is only provided in python.
* FIBS: The algorithm calculation has not been completed. So we couldn't test.


## Constant time implementation
We verified the constant time implementation by analyzing the logs generated by Valgrind.

Valgrind checks whether the flag value forms a [conditional statement](https://neuromancer.sk/article/29).

We specified the secret key array as the flag.

Valgrind only checks whether a conditional statement is created, but does not determine the meaning of the statement.

We analyzed the logs to see if time information was actually leaking.

The results of this analysis are largely divided into three categories.

* High risk(L1): There is a high possibility that actual side channel information will be leaked.
* Moderate risk(L2): Side channel information may be leaked, but has little correlation with the secret key.
* Low risk or safe(L3): A conditional statement that is always ignored during execution or a statement that cannot be executed under normal circumstances.

<details>
<summary>Learn more</summary>

The following algorithms could not be tested:
* FIBS: Computation does not end when using Valgrind.
* REDOG: Written in Python, so Valgrind cannot be applied.
* Peregrine, SOLMAE: Difficult to specify flag because it does not use general secret key array.

### AIMer
<details>
<summary>Learn more</summary>
    
#### L1
None
#### L2
None
#### L3
* aimer_instance_get:	 This is the part that checks the parameters of AIMer. It is ignored during normal execution and has no effect on terminating the operation if it is judged to be abnormal.
* h_2_expand:	This function utilizes the secret key to extract hash values. There doesn't seem to be a significant risk related to hash value leakage, and it does not handle the entire secret key.
* reveal_all_but:	This function is used in the verification tree and deals with a portion of hash values generated using the secret key. There doesn't appear to be a significant risk associated with hash value leakage. It returns a list of tree nodes excluding the leaf index.
* get_sibling:	This function is used in the verification tree and deals with a portion of hash values (masked) generated using the secret key. There doesn't seem to be a significant risk related to hash value leakage. It finds the sibling node of the given index in the tree.
* reconstruct_seed_tree:	This function is used in the verification tree and performs branching based on the number of nodes (some hash values). There doesn't seem to be a significant risk associated with revealing the number of nodes.
* has_sibling:	This function is used in the verification tree and performs branching to determine the presence of sibling nodes. There doesn't appear to be a significant risk associated with revealing the presence or absence of sibling nodes.
* _aimer_verify:	This function is used for signature verification. It contains conditional statements for comparing some tree values, but it only deals with hash values. There are no direct branches that handle the secret key.  
</details>

### GCKSign
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
* poly_reject_z: The "check" variable, which determines the number of iterations of the while loop in the "crypto_sign" function, is determined based on the result of the "poly_reject_z" function. In this process, the "z" value is a variable that depends on the value of "sk" (the secret key). Therefore, the number of iterations of this while loop can be influenced by the value of "sk."
#### L3
* sample_y:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* sample_c:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* crypto_sign_open:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* rej_uniform:	"rej_uniform()" is a function that operates within "sample_a()." The values associated with "rej_uniform()" are not related to the leakage of the secret key; even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
</details>

### HAETAE
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* cryptolab_haetae2_rej_uniform:	rej_uniform() is a function that operates within "poly_uniform()." The values associated with "rej_uniform()" are not related to the leakage of the secret key; even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae3_rej_uniform:	rej_uniform() is a function that operates within "poly_uniform()." The values associated with "rej_uniform()" are not related to the leakage of the secret key; even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae5_rej_uniform:	rej_uniform() is a function that operates within "poly_uniform()." The values associated with "rej_uniform()" are not related to the leakage of the secret key; even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae2_polyfixveclk_sample_hyperball:	This is a function used within crypto_sign_signature(). The values associated with rej_uniform() are not related to the leakage of the secret key. Even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae3_polyfixveclk_sample_hyperball:	This is a function used within crypto_sign_signature(). The values associated with rej_uniform() are not related to the leakage of the secret key. Even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae5_polyfixveclk_sample_hyperball:	This is a function used within crypto_sign_signature(). The values associated with rej_uniform() are not related to the leakage of the secret key. Even if SHAKE hash values are exposed, only the hash values are leaked, and there is no connection to the leakage of the secret key.
* sample_gauss:	sample_gauss() is a function that operates within "sample_gauss_N()." "sample_gauss_N()" is used in "polyfixveclk_sample_hyperball()." Therefore, even if there is a leakage, only hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae2_sample_gauss_N:	sample_gauss_N() is used within "polyfixveclk_sample_hyperball()." Therefore, even if there is a leakage, only hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae3_sample_gauss_N:	sample_gauss_N() is used within "polyfixveclk_sample_hyperball()." Therefore, even if there is a leakage, only hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae5_sample_gauss_N:	sample_gauss_N() is used within "polyfixveclk_sample_hyperball()." Therefore, even if there is a leakage, only hash values are leaked, and there is no connection to the leakage of the secret key.
* cryptolab_haetae2_poly_challenge:	poly_challenge() is used in "crypto_sign_signature()." "poly_challenge()" operates on the result of a SHAKE hash, and as a result, any leakage only pertains to the hash values.
* cryptolab_haetae3_poly_challenge:	poly_challenge() is used in "crypto_sign_signature()." "poly_challenge()" operates on the result of a SHAKE hash, and as a result, any leakage only pertains to the hash values.
* cryptolab_haetae2_signature:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* cryptolab_haetae3_signature:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* cryptolab_haetae5_signature:	It was detected in the logs due to the lack of initialization; there is no association with constant time* .
* cryptolab_haetae2_verify:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* cryptolab_haetae3_verify:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* cryptolab_haetae5_verify:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
</details>

### MQSign
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* mqss_sign: 	It was detected, but didn't see any indication of a secret key forming a branch.

</details>

### NCCSign
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* poly_decompose:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* decompose:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* poly_challenge:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* poly_chknorm:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* poly_make_hint:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* make_hint:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* poly_use_hint:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* use_hint:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* poly_uniform:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
* rej_uniform:	It was detected in the logs due to the lack of initialization; there is no association with constant time.
</details>

### Enhanced pqsigRM
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* vector_mtx_product:	Although the number of loops depends on the matrix size, it appears to be independent of leakage of confidential information.
* vec_mat_prod:	Although the number of loops depends on the matrix size, it appears to be independent of leakage of confidential information.
* mindist_decoding:	Although the number of loops depends on the matrix size, it appears to be independent of leakage of confidential information.
</details>

### NTRUplus
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
None
</details>

### IPCC
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* PDSdigitValue:	There is a branching statement within this function that uses the secret key. However, it only checks whether the secret key is empty, so there is no risk of leaking the actual secret key content. (An error will occur if the secret key value is 0)
* CipherDigitValue:	This function converts ciphertext into numbers, but it does not contain branches that handle the secret key. Therefore, there is no risk of secret key leakage.
* NewDecryption:	This function is used for decrypting ciphertext. There is a branching statement that deals with the index of the secret key, but there is no risk of exposing the key value.
</details>

### Layered ROLLO-I
<details>
<summary>Learn more</summary>

Analyzing is still ongoing with updated version.
#### L1
None
#### L2
* ~~biix_secret_key_from_string:	In rbc_vspace_set_random_full_rank -> rbc_vec_set_random_full_rank, it seems that the size of the rank generated due to the secret key influences the number of times the while loop is executed.~~
#### L3
* ~~biix_decaps:	Through the for loop, the secret key is stored at the same index position in the skseed_st array up to size SEEDEXPANDER_SEED_BYTES(40). The skseed_st array is then used in biix_secret_key_from_string.~~
</details>

### SMAUG
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
* malloc:	Array size value not initialized.
* cmov:	Array value not initialized.
* poly_mult_add:	Array value not initialized.
* indcpa_dec:	Array value not initialized.
</details>

### TiGER
<details>
<summary>Learn more</summary>

#### L1
None
#### L2
None
#### L3
None
</details>

### PALOMA
<details>
<summary>Learn more</summary>

The amount of logs is very large, but in all cases are confirmed that references to uninitialized variables.
However, additional analysis is still ongoing. 
</details>

</details>

## Parameters
<details>
<summary>Learn more </summary>
    
* Unit: Bytes
* All values come from each white paper or source code.
* Some values are different from the value written on the source code. In this case, we wrote values of source code.

    
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
| TiGER-128		| 480		 | 134		     | 640    	 | 
| TiGER-192		| 928    	 | 178        	 | 1,280	 | 
| TiGER-256		| 928    	 | 263    		 | 1,408	 | 

### Digital Signature
| Algorithm     | Public Key    | Srcret Key    | Signature     | 
| ------------- | -------------: | -------------:| -------------:|
| AIMer-I (all parameters)  				| 33		| 49		| 5,904		|
| AIMer-III (all parameters)				| 49 		| 73 	    | 13,080 	|
| AIMer-V (all parameters)				| 65 		| 97	    | 25,152 	|
| GCKSign-II			| 1,760 	| 288 		| 1,952 	|
| GCKSign-III			| 1,952 	| 288 		| 2,080		|
| GCKSign-V				| 3,040 	| 5,444 	| 3,104 	|
| HAETAE-II				| 1,120 	| 1,536 	| 3,040 	|
| HAETAE-III			| 1,664 	| 2,272 	| 4,544 	|
| HAETAE-V				| 2,208 	| 2,944 	| 5,792 	|
| MQSign-72/46			| 328,411 	| 15,561 	| 134 	    |
| MQSign-112/72			| 1,238,761 | 37,729 	| 200 	    |
| MQSign-148/96			| 2,892,961 | 66,421 	| 260 	    |
| NCCSign-I(original)	| 1,564 	| 2,266 	| 2,458 	|
| NCCSign-III(original)	| 1,997 	| 3,312 	| 3,605 	|
| NCCSign-V(original)	| 2,663 	| 4,402	    | 5,055 	|
| NCCSign-I(conserparam)| 1,984 	| 2,800 	| 3,186 	|
| NCCSign-III(conserparam)| 2,443 	| 3,914 	| 4,251 	|
| NCCSign-V(conserparam)| 3,091 	| 4,940 	| 5,383 	|
| Peregrine-512 		| - 	    | - 		| - 		|
| Peregrine-1024		| - 	    | - 		| -		    |
| Enhanced pqsigRM-612	| 1,285,120 | 24,592 	| 528   	|
| Enhanced pqsigRM-613	| 4,194,304 | 331,074 	| 1,040 	|
| SOLMAE-512			| -     	| -		    | -		    |
| SOLMAE-1024			| -	        | -		    | -		    |
* Peregrine and SOLMAE did not specify parameters in the white paper
</details>

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
To change SMAUG scheme, change -DSMAUG_MODE(=1, 3, 5) value at Makefile.
#### HAETAE
To change HAETAE scheme, modify #define HAETAE_MODE(=2, 3, 5) macro in config.h before compile.
#### MQ-Sign
To change MQ-Sign scheme, comment out any other define macros except for the desired MQSign at mqc_config.h file.
#### Peregrine
To change Peregrine scheme, change the logn(=9, 10) value at PQC_bench.c file.
#### NCCSign
Compile with the following command.

    make PQC_bench2
    make clean_bench2 (NCCSign-II)
    make PQC_bench3
    make clean_bench3 (NCCSign-III)
    make PQC_bench5
    make clean_bench5 (NCCSign-V)
    
#### SOLMAE
Compile with the following command.

    make
    make clean

#### Layered-ROLLO
Compile through python file. Before try to compile codes, you must first configure python3 to be executable with python.

    (optional) alias python=python3
    python rbc-lib.py

## Contact
* HyeokDong Kwon: <korlethean@gmail.com>
* HwaJeong Seo: <hwajeong84@gmail.com>

### Contributers
* (Ph.D. student) HyeokDong Kwon
* (Ph.D. student) MinJoo Sim
* (Ph.D. student) GyeongJu Song
* (Master student) MinWoo Lee
* (Master student) SangWong Kim
* (Associate professor) HwaJeong Seo

