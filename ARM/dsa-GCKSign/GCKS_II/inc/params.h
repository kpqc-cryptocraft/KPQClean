#ifndef __PARAMS_H__
#define __PARAMS_H__

//PARAM options
#define PARAM_1 //
#define Raw_s //either NTT_s or Raw_s

#ifdef PARAM_1

	#define _log_P 54
	#define _H 39

	#define N 256
	#define Q 18014398509471233 //(2<<54)-10751
	#define W 11423391321580314 
	#define _MONT_betamodQ 11009024
	#define _MONT_betaSquare 121198609432576
	#define _MONT_QInvmodbeta 4926968990982744577

	#define _B ((1 << 14) -1)
	#define _log_B 14
	#define _N 256
	#define _M 4

	#define _log_N 8

	#define _hN    (_N >> 1)
	#define _hhN   (_N >> 2)
	#define _2hhN  _hN
	#define _3hhN  (_hN + _hhN)
	#define _hhhN  (_N >> 3)
	#define _hhhhN (_N >> 4)
	#define _Bound1 18
	#define _Bound2 (_B - 18)
#endif

#ifdef PARAM_2
	#define _log_P 60
	#define _H 45

	#define N 256
	#define Q 1152921504606844417 //(2<<60)-2559
	#define W 312311433193348612 
	#define _MONT_betamodQ 40944
	#define _MONT_betaSquare 1676411136
	#define _MONT_QInvmodbeta 12954260266494986753

	#define _B ((1 << 14) + (1 << 9))
	#define _log_B 15
	#define _N 256
	#define _M 4

	#define _log_N 8

	#define _hN    (_N >> 1)
	#define _hhN   (_N >> 2)
	#define _2hhN  _hN
	#define _3hhN  (_hN + _hhN)
	#define _hhhN  (_N >> 3)
	#define _hhhhN (_N >> 4)
	#define _Bound1 18
	#define _Bound2 (_B - 18)
#endif

#ifdef PARAM_3
	#define _log_P 47
	#define _H 44

	#define N 512
	#define Q 140737488348161 //(2<<47)-7167
	#define W 46236542166801 // w = 46236542166801 \beta = 939393024
	#define _MONT_betamodQ 939393024 
	#define _MONT_betaSquare 35201596895106
	#define _MONT_QInvmodbeta 18060808164123614209

	#define _B ((1 << 15) -1)
	#define _log_B 15
	#define _N 512
	#define _M 3

	#define _log_N 9

	#define _hN    (_N >> 1)
	#define _hhN   (_N >> 2)
	#define _2hhN  _hN
	#define _3hhN  (_hN + _hhN)
	#define _hhhN  (_N >> 3)
	#define _hhhhN (_N >> 4)
	#define _Bound1 19
	#define _Bound2 (_B - 19)
#endif


#endif