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
1. NTRUplus: Round 1 Submission.
2. PALOMA: Round 1 Submission.
3. SMAUG: May. 23rd. (<https://groups.google.com/g/kpqc-bulletin/c/8S0qef2mPSU>)
4. TiGER: Round 1 Submission.
5. REDOG: Round 1 Submission.
6. Layered-ROLLO: Round 1 Submission.
7. IPCC: Round 1 Submission.

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

### Benchmark method
* We used 'rdtsc' instruction to calculate time consumption.
* Each algorithms 10,000 iterated, and **average**, **median** value of the operation cycle is used.
* Enhanced pqsigRM was 100 iterated.

### PKE/KEM (Environment1, -O2)
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 208,742		 		| 111,998				| 128,093		 		| 286,443				| 112,614				| 128,670				|
| Lattice		| NTRUplus-768			| 279,386		 		| 148,480				| 181,250		 		| 298,193				| 154,346				| 185,298				|
| Lattice		| NTRUplus-864			| 304,819		 		| 179,858				| 224,953		 		| 306,436				| 180,793				| 225,978				|
| Lattice		| NTRUplus-1152			| 444,744		 		| 223,619				| 278,690		 		| 801,975				| 224,602				| 279,606				|
| Lattice		| SMAUG-128				| 171,477 		 		| 154,483				| 178,205		 		| 181,007				| 156,512				| 181,950				|
| Lattice		| SMAUG-192				| 250,096		 		| 229,999				| 277,298		 		| 260,837				| 230,994				| 279,080				|
| Lattice		| SMAUG-256				| 479,138 		 		| 385,178		 		| 438,364 		 		| 490,702				| 387,420				| 439,345				|
| Lattice		| TiGER-128				| 273,470		 		| 466,755				| 628,778		 		| 286,082				| 471,567				| 632,066				|
| Lattice		| TiGER-192				| 288,550		 		| 518,491		 		| 674,192 				| 293,000				| 524,467				| 691,838				|
| Lattice		| TiGER-256				| 536,152		 		| ~~1,088,747~~	 		| ~~1,477,318~~	 		| 541,935				| ~~1,092,191~~			| ~~1,276,848~~			|
| Code			| PALOMA-128			| 125,800,419	 		| 510,922 				| 35,496		 		| 125,630,139			| 513,098				| 36,061				|
| Code			| PALOMA-192			| 125,360,779	 		| 514,228				| 34,220		 		| 125,242,970			| 516,579				| 34,419				|
| Code			| PALOMA-256			| 125,294,065	 		| 510,284				| 34,713		 		| 125,174,502			| 512,685				| 34,978				|
| Code			| IPCC-1  				| 14,362,627	 		| ~~164,892,550~~		| 2,484,981				| 14,376,006			| ~~239,300,607~~		| 2,501,514				|
| Code			| IPCC-3		 		| 14,170,647	 		| 898,710		 		| 2,619,570				| 14,178,752			| 941,012				| 2,633,907				|
| Code			| IPCC-4				| 14,209,594	 		| 1,075,059	 			| 2,904,524				| 14,245,778			| 1,135,740				| 2,935,161				|
| Code			| Layered ROLLO I-128	| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		|
| Code			| Layered ROLLO I-192	| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		|
| Code			| Layered ROLLO I-256	| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		| *Not yet tested*		|

### PKE/KEM (Environment1, -O3)
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 202,652		 		| 110,026				| 121,742		 		| 287,810				| 110,910				| 123,929				|
| Lattice		| NTRUplus-768			| 270,512		 		| 146,566				| 174,435		 		| 281,685				| 147,174				| 175,018				|
| Lattice		| NTRUplus-864			| 297,192		 		| 168,113				| 204,537		 		| 302,876				| 168,857				| 206,046				|
| Lattice		| NTRUplus-1152			| 435,305		 		| 222,459				| 266,626		 		| 772,442				| 223,429				| 268,110				|
| Lattice		| SMAUG-128				| 72,790 		 		| 57,246				| 50,460		 		| 82,292				| 57,466				| 50,708				|
| Lattice		| SMAUG-192				| 105,966		 		| 82,940 				| 80,475		 		| 108,491				| 83,648				| 81,029				|
| Lattice		| SMAUG-256				| 158,021 		 		| 139,925		 		| 135,749 		 		| 161,110				| 141,573				| 138,010				|
| Lattice		| TiGER-128				| 65,482		 		| 48,749				| 51,214		 		| 68,866				| 49,105				| 51,589				|
| Lattice		| TiGER-192				| 69,426		 		| 63,510		 		| 57,739 				| 79,105				| 63,805				| 59,383				|
| Lattice		| TiGER-256				| 81,316		 		| 87,551 		 		| 93,090 		 		| 90,989				| 88,218				| 93,436				|
| Code			| PALOMA-128			| 122,325,408	 		| 498,365 				| 34,307		 		| 122,253,994			| 500,446				| 34,484				|
| Code			| PALOMA-192			| 122,290,738	 		| 503,266				| 34,278		 		| 122,173,457			| 506,366				| 34,468				|
| Code			| PALOMA-256			| 122,321,957	 		| 497,959				| 34,249		 		| 122,254,172			| 500,026				| 34,420				|
| Code			| IPCC-1  				| 13,940,097	 		| ~~160,111,204~~		| ~~16,360,164~~		| 13,969,607			| ~~232,561,407~~		| 2,408,173				|
| Code			| IPCC-3		 		| 13,996,024	 		| 926,492		 		| 2,512,836				| 14,036,005			| 967,543				| 2,532,438				|
| Code			| IPCC-4				| 13,989,832	 		| 1,106,031	 			| 2,714,531				| 14,007,544			| 1,165,274				| 2,732,139				|
| Code			| Layered ROLLO I-128	| 285,940				| 83,346				| 788,104				| 296,880				| 84,198				| 805,790				|
| Code			| Layered ROLLO I-192	| 320,958				| 136,503				| 1,014,203				| 345,689				| 149,843				| 1,110,378				|
| Code			| Layered ROLLO I-256	| 687,721				| 201,913				| 1,945,871				| 700,284				| 207,030				| 1,948,662				|

### PKE/KEM (Environment2, -O2)
| Type			| Algorithm          	| Keygen(Med.)         	| Encapsulation(Med.)   | Decapsulation(Med.)   |  Keygen(Avr.)      | Encapsulation(Avr.)	| Decapsulation(Avr.)	|
| -------------:| -------------:      	| -------------:       	| -------------:      	| -------------:      	| -------------:     | -------------:      	| -------------:      	|
| Lattice		| NTRUplus-576       	| 186,944             	| 105,686             	| 120,194             	| 271,460            | 121,722             	| 132,428             	|
| Lattice		| NTRUplus-768       	| 246,616             	| 139,310             	| 166,938             	| 265,516            | 154,868             	| 174,788             	|
| Lattice		| NTRUplus-864       	| 270,494             	| 160,789             	| 200,702              	| 288,025            | 180,014            	| 206,856            	|
| Lattice		| NTRUplus-1152      	| 698,490             	| 202,678             	| 257,114              	| 744,381            | 212,073             	| 267,046             	|
| Lattice		| SMAUG-128         	| 158,149            	| 164,598             	| 196,470             	| 165,414            | 169,648             	| 203,288            	|
| Lattice		| SMAUG-192         	| 244,736             	| 225,490            	| 272,132             	| 265,142            | 236,227            	| 285,366            	|
| Lattice		| SMAUG-256         	| 435,790             	| 411,917             	| 465,572              	| 448,654            | 422,602            	| 486,263            	|
| Lattice		| TiGER-128         	| 163,856             	| 209,168            	| 311,924             	| 182,794            | 217,723            	| 325,532            	|
| Lattice		| TiGER-192         	| 171,578             	| 214,126             	| 312,702             	| 181,774            | 221,613            	| 324,412            	|
| Lattice		| TiGER-256         	| 444,558             	| 433,462          		| 673,105         		| 461,623            | 448,364         		| 714,429         		|
| Code			| PALOMA-128			| 118,204,341           | 499,914             	| 39,724              	| 118,365,137        | 511,837            	| 41,693             	|
| Code			| PALOMA-192        	| 118,310,371           | 499,302             	| 38,846              	| 118,490,933        | 514,299             	| 41,016             	|
| Code			| PALOMA-256        	| 118,366,206         	| 503,814            	| 43,174             	| 118,507,160        | 518,903             	| 45,385             	|
| Code			| IPCC-1            	| 13,792,887          	| ~~159,126,951~~       | 1,196,157            	| 13,896,694         | ~~231,010,613~~      | 1,259,215             |
| Code			| IPCC-3           		| 13,754,219          	| 870,059              	| 1,235,991            	| 13,864,988         | 922,755            	| 1,307,538             |
| Code			| IPCC-4            	| 13,754,687          	| 1,050,451             | 1,318,173             | 13,851,205         | 1,151,306            | 1,380,740             |
| Code			| Layered ROLLO I-128   | *Not yet tested*      | *Not yet tested*      | *Not yet tested*      | *Not yet tested*   | *Not yet tested*     | *Not yet tested*      |
| Code			| Layered ROLLO I-192   | *Not yet tested*      | *Not yet tested*      | *Not yet tested*      | *Not yet tested*   | *Not yet tested*     | *Not yet tested*      |
| Code			| Layered ROLLO I-256   | *Not yet tested*      | *Not yet tested*      | *Not yet tested*      | *Not yet tested*   | *Not yet tested*     | *Not yet tested*      |

### PKE/KEM (Environment2, -O3)
| Type			| Algorithm     		| Keygen(Med.)	 		| Encapsulation(Med.) 	| Decapsulation(Med.)	|  Keygen(Avr.)			| Encapsulation(Avr.) 	| Decapsulation(Avr.)	|
| -------------:| -------------: 		| -------------: 		| -------------:		| -------------:		| -------------: 		| -------------:		| -------------:		|
| Lattice		| NTRUplus-576			| 177,748 		 		| 102,296				| 111,820		 		| 258,761				| 117,949				| 124,783				|
| Lattice		| NTRUplus-768			| 239,546		 		| 137,135				| 161,970		 		| 257,560				| 165,057				| 177,077				|
| Lattice		| NTRUplus-864			| 260,672		 		| 153,481				| 186,386		 		| 272,001				| 163,794				| 197,686				|
| Lattice		| NTRUplus-1152			| 568,556		 		| 201,226				| 246,050		 		| 698,764				| 209,569				| 256,800				|
| Lattice		| SMAUG-128				| 63,020 		 		| 49,324				| 39,196		 		| 65,919				| 55,873				| 42,528				|
| Lattice		| SMAUG-192				| 92,658		 		| 69,739				| 67,691		 		| 95,436 				| 74,836				| 70,950				|
| Lattice		| SMAUG-256				| 135,202 		 		| 122,766		 		| 115,096 		 		| 142,842				| 128,734				| 118,789				|
| Lattice		| TiGER-128				| 62,490 		 		| 45,398				| 53,248		 		| 66,987 				| 48,285				| 56,591				|
| Lattice		| TiGER-192				| 66,512 		 		| 60,238		 		| 58,572 				| 71,626 				| 71,973				| 71,967				|
| Lattice		| TiGER-256				| 78,772 		 		| 82,776		 		| 89,902		 		| 83,770 				| 90,129				| 98,287				|
| Code			| PALOMA-128			| 108,402,198	 		| 459,846 				| 40,838		 		| 108,597,537			| 473,532				| 42,840				|
| Code			| PALOMA-192			| 108,206,652	 		| 460,374				| 40,688		 		| 108,344,570			| 472,432				| 42,798				|
| Code			| PALOMA-256			| 108,216,713	 		| 459,880				| 40,886		 		| 108,461,853			| 465,766				| 41,780				|
| Code			| IPCC-1  				| 12,643,392	 		| ~~145,233,220~~		| 1,159,273				| 12,712,124			| ~~210,977,105~~		| 1,185,580				|
| Code			| IPCC-3		 		| 12,795,377	 		| 874,663		 		| 1,206,585				| 12,874,291			| 922,533				| 1,267,783				|
| Code			| IPCC-4				| 13,078,917	 		| 1,037,485	 			| 1,310,503				| 13,250,237			| 1,107,017				| 1,368,035				|
| Code			| Layered ROLLO I-128	| 203,181				| 66,529				| 558,503				| 231,523				| 77,774				| 602,966				|
| Code			| Layered ROLLO I-192	| 227,813				| 102,758				| 671,605				| 255,243				| 125,567				| 761,739				|
| Code			| Layered ROLLO I-256	| 375,056				| 136,052 				| 1,245,346				| 455,911				| 146,919				| 1,337,504				|

### Digital Signature (Environment1, -O2)
| Type					| Algorithm     		| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 		| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMer-I  				| 145,058			| 3,912,361			| 3,669,834				| 156,204			| 3,966,517			| 3,701,498			|
| Zero-knowledge		| AIMer-III				| 296,496 			| 8,001,274 		| 7,550,063 			| 315,810			| 8,033,590			| 7,548,322			|
| Zero-knowledge		| AIMer-V				| 710,442 			| 18,068,276 		| 17,415,022 			| 730,960			| 18,077,211		| 17,421,527		|
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

### Digital Signature (Environment1, -O3)
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMER-I 					| 145,986 			| 3,878,272 		| 3,672,923 			| 156,213 			| 4,077,840 		| 4,384,331			|
| Zero-knowledge		| AIMER-III 				| 296,032 			| 8,087,462 		| 7,678,098 			| 307,498 			| 8,364,809 		| 7,740,701			|
| Zero-knowledge		| AIMER-V 					| 713,922 			| 17,983,857 		| 17,361,691 			| 817,056 			| 18,096,797 		| 17,472,521		|
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
| Code					| pqsigRM-613 				| 6,139,551,981 	| 4,610,319 		| 2,278,806 			| 6,144,274,759 	| 6,276,554 		| 2,376,095			|
| Code					| pqsigRM-612 				| 54,994,439,928 	| 714,647 			| 225,577 				| 55,073,661,751 	| 967,439 			| 234,553			|

### Digital Signature (Environment2, -O2)
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMER-I					| 145,566 			| 3,691,256 		| 3,713,173 			| 159,391 			| 3,845,843 		| 4,018,047			|
| Zero-knowledge		| AIMER-III					| 274,358 			| 7,771,108 		| 7,366,672 			| 304,919 			| 7,863,536 		| 7,438,953			|
| Zero-knowledge		| AIMER-V 					| 790,456 			| 18,394,069 		| 17,662,359 			| 899,383 			| 18,802,192 		| 18,080,181		|
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
| Code					| pqsigRM-613 				| 4,961,556,899 	| 7,505,040 		| 2,125,125 			| 4,973,260,518 	| 10,823,438 		| 2,645,728			|
| Code					| pqsigRM-612 				| 74,021,054,015 	| 2,113,913 		| 1,126,131 			| 73,941,690,821 	| 2,765,068 		| 1,295,161			|


### Digital Signature (Environment2, -O3)
| Type					| Algorithm     			| Keygen(Med.)		| Signature(Med.) 	| Verification(Med.)	| Keygen(Avr.)		| Signature(Avr.)	| Verification(Avr.)| 
| -------------:		| -------------: 			| -------------: 	| -------------:	| -------------:		| -------------:	| -------------:	| -------------:	|
| Zero-knowledge		| AIMER-I 					| 133,130 			| 3,960,345 		| 3,747,101 			| 143,746 			| 4,070,924 		| 3,834,717			|
| Zero-knowledge		| AIMER-III 				| 272,484 			| 8,440,184 		| 7,968,982 			| 282,896 			| 8,530,553 		| 8,041,509			|
| Zero-knowledge		| AIMER-V 					| 643,253 			| 17,998,305 		| 17,373,174 			| 662,744 			| 18,202,241 		| 17,455,874		|	
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
| Code					| pqsigRM-613 				| 4,702,612,115 	| 4,732,706 		| 2,064,731 			| 4,703,836,987 	| 6,667,564 		| 2,458,625			|
| Code					| pqsigRM-612 				| 71,111,088,778 	| 923,513 			| 417,658 				| 71,168,430,985 	| 1,166,665 		| 502,448			|


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
### MQ-Sign
To change MQ-Sign scheme, comment out any other define macros except for the desired MQSign at mqc_config.h file.
### Peregrine
To change Peregrine scheme, change the logn(=9, 10) value at PQC_bench.c file.
### NCCSign
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

