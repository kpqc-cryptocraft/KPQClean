#include "pack.h"
#include "params.h"

#include <stdint.h>

#ifdef PARAM_1
void pack_poly(uint8_t* out, int64_t* in)
{
	uint64_t tmp1, tmp2, tmp3, tmp4;

	for (int i = 0; i < _hhN; i++)
	{
		tmp1 = in[4*i];
		tmp2 = in[4*i + 1];
		tmp3 = in[4*i + 2];
		tmp4 = in[4*i + 3];

		out[27*i +  0] = tmp1 >> 46;
		out[27*i +  1] = tmp1 >> 38;
		out[27*i +  2] = tmp1 >> 30;
		out[27*i +  3] = tmp1 >> 22;
		out[27*i +  4] = tmp1 >> 14;
		out[27*i +  5] = tmp1 >>  6;
		out[27*i +  6] = (tmp1 << 2) | ((tmp2 >> 52) & 0x03);
		out[27*i +  7] = tmp2 >> 44;
		out[27*i +  8] = tmp2 >> 36;
		out[27*i +  9] = tmp2 >> 28;		
		out[27*i + 10] = tmp2 >> 20;
		out[27*i + 11] = tmp2 >> 12;
		out[27*i + 12] = tmp2 >> 4;
		out[27*i + 13] = (tmp2 << 4) | ((tmp3 >> 50) & 0x0f);
		out[27*i + 14] = tmp3 >> 42;
		out[27*i + 15] = tmp3 >> 34;
		out[27*i + 16] = tmp3 >> 26;		
		out[27*i + 17] = tmp3 >> 18;
		out[27*i + 18] = tmp3 >> 10;
		out[27*i + 19] = tmp3 >>  2;
		out[27*i + 20] = (tmp3 << 6) | ((tmp4 >> 48) & 0x3f);
		out[27*i + 21] = tmp4 >> 40;
		out[27*i + 22] = tmp4 >> 32;
		out[27*i + 23] = tmp4 >> 24;		
		out[27*i + 24] = tmp4 >> 16;
		out[27*i + 25] = tmp4 >>  8;
		out[27*i + 26] = tmp4;
	}
}
#endif

#ifdef PARAM_2
void pack_poly(uint8_t* out, int64_t* in)
{
	uint64_t tmp1;
	uint64_t tmp2;

	for (int i = 0; i < _hN; i++)
	{
		tmp1 = in[2*i];
		tmp2 = in[2*i + 1];

		out[15*i +  0] = tmp1 >> 52;
		out[15*i +  1] = tmp1 >> 44;
		out[15*i +  2] = tmp1 >> 36;
		out[15*i +  3] = tmp1 >> 28;
		out[15*i +  4] = tmp1 >> 20;
		out[15*i +  5] = tmp1 >> 12;
		out[15*i +  6] = tmp1 >>  4;
		out[15*i +  7] = (tmp1 <<  4) | ((tmp2 >> 56) & 0x0f);
		out[15*i +  8] = tmp2 >> 48;
		out[15*i +  9] = tmp2 >> 40;
		out[15*i + 10] = tmp2 >> 32;
		out[15*i + 11] = tmp2 >> 24;
		out[15*i + 12] = tmp2 >> 16;
		out[15*i + 13] = tmp2 >> 8;
		out[15*i + 14] = tmp2;
	}
}
#endif

#ifdef PARAM_3
void pack_poly(uint8_t* out, int64_t* in)
{
	uint64_t tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

	for (int i = 0; i < _hhhN; i++)
	{
		tmp1 = in[8*i];
		tmp2 = in[8*i + 1];
		tmp3 = in[8*i + 2];
		tmp4 = in[8*i + 3];
		tmp5 = in[8*i + 4];
		tmp6 = in[8*i + 5];
		tmp7 = in[8*i + 6];
		tmp8 = in[8*i + 7];

		out[47*i +  0] = tmp1 >> 39;
		out[47*i +  1] = tmp1 >> 31;
		out[47*i +  2] = tmp1 >> 23;
		out[47*i +  3] = tmp1 >> 15;
		out[47*i +  4] = tmp1 >>  7;
		out[47*i +  5] = (tmp1 << 1) | ((tmp2 >> 46) & 0x01);
		out[47*i +  6] = tmp2 >> 38;
		out[47*i +  7] = tmp2 >> 30;
		out[47*i +  8] = tmp2 >> 22;
		out[47*i +  9] = tmp2 >> 14;		
		out[47*i + 10] = tmp2 >>  6;
		out[47*i + 11] = (tmp2 << 2) | ((tmp3 >> 45) & 0x03);
		out[47*i + 12] = tmp3 >> 37;
		out[47*i + 13] = tmp3 >> 29;
		out[47*i + 14] = tmp3 >> 21;
		out[47*i + 15] = tmp3 >> 13;
		out[47*i + 16] = tmp3 >>  5;		
		out[47*i + 17] = (tmp3 << 3) | ((tmp4 >> 44) & 0x07);
		out[47*i + 18] = tmp4 >> 36;
		out[47*i + 19] = tmp4 >> 28;
		out[47*i + 20] = tmp4 >> 20;
		out[47*i + 21] = tmp4 >> 12;
		out[47*i + 22] = tmp4 >>  4;
		out[47*i + 23] = (tmp4 << 4) | ((tmp5 >> 43) & 0x0f);		
		out[47*i + 24] = tmp5 >> 35;
		out[47*i + 25] = tmp5 >> 27;
		out[47*i + 26] = tmp5 >> 19;
		out[47*i + 27] = tmp5 >> 11;
		out[47*i + 28] = tmp5 >>  3;
		out[47*i + 29] = (tmp5 << 5) | ((tmp6 >> 42) & 0x1f);
		out[47*i + 30] = tmp6 >> 34;
		out[47*i + 31] = tmp6 >> 26;
		out[47*i + 32] = tmp6 >> 18;
		out[47*i + 33] = tmp6 >> 10;
		out[47*i + 34] = tmp6 >>  2;
		out[47*i + 35] = (tmp6 << 6) | ((tmp7 >> 41) & 0x3f);
		out[47*i + 36] = tmp7 >> 33;		
		out[47*i + 37] = tmp7 >> 25;
		out[47*i + 38] = tmp7 >> 17;
		out[47*i + 39] = tmp7 >>  9;
		out[47*i + 40] = tmp7 >>  1;
		out[47*i + 41] = (tmp7 << 7) | ((tmp8 >> 40) & 0x7f);
		out[47*i + 42] = tmp8 >> 32;
		out[47*i + 43] = tmp8 >> 24;		
		out[47*i + 44] = tmp8 >> 16;
		out[47*i + 45] = tmp8 >>  8;
		out[47*i + 46] = tmp8;
	}
}
#endif

void pack_s(uint8_t* out, int64_t in[][_N])
{
	uint8_t* tmpout;

	for (int i = 0; i < _M; i++)
	{
		tmpout = out + _hhN * i;

		for (int j = 0; j < _hhN; j++)
		{
			tmpout[j] = ((in[i][4*j + 0] & 0x3) << 6) | ((in[i][4*j + 1] & 0x3) << 4) | ((in[i][4*j + 2] & 0x3) << 2) | (in[i][4*j + 3] & 0x3);
		}
	}
}

void pack_z(uint8_t* out, int64_t in[][_N])
{
	uint8_t* tmpout;

#ifdef PARAM_1
	for (int i = 0; i < _M; i++)
	{
		tmpout = out + i * 15 * _hhhN;

		for (int j = 0; j < _hhhN; j++)
		{
			tmpout[15*j +  0] = (in[i][8*j + 0] >> 7);
			tmpout[15*j +  1] = (in[i][8*j + 0] << 1) | ((in[i][8*j + 1] >> 14) & 0x01);
			tmpout[15*j +  2] = (in[i][8*j + 1] >> 6);
			tmpout[15*j +  3] = (in[i][8*j + 1] << 2) | ((in[i][8*j + 2] >> 13) & 0x03);
			tmpout[15*j +  4] = (in[i][8*j + 2] >> 5);		
			tmpout[15*j +  5] = (in[i][8*j + 2] << 3) | ((in[i][8*j + 3] >> 12) & 0x07);
			tmpout[15*j +  6] = (in[i][8*j + 3] >> 4);
			tmpout[15*j +  7] = (in[i][8*j + 3] << 4) | ((in[i][8*j + 4] >> 11) & 0x0f);
			tmpout[15*j +  8] = (in[i][8*j + 4] >> 3);
			tmpout[15*j +  9] = (in[i][8*j + 4] << 5) | ((in[i][8*j + 5] >> 10) & 0x1f);		
			tmpout[15*j + 10] = (in[i][8*j + 5] >> 2);
			tmpout[15*j + 11] = (in[i][8*j + 5] << 6) | ((in[i][8*j + 6] >>  9) & 0x3f);
			tmpout[15*j + 12] = (in[i][8*j + 6] >> 1);
			tmpout[15*j + 13] = (in[i][8*j + 6] << 7) | ((in[i][8*j + 7] >>  8) & 0x7f);						
			tmpout[15*j + 14] = (in[i][8*j + 7]);
		}
	}
#endif

#ifdef PARAM_2
	for (int i = 0; i < _M; i++)
	{
		tmpout = out + i * 512;

		for (int j = 0; j < 256; j++)
		{
			tmpout[2*j+0]  = in[i][j] >> 8;
			tmpout[2*j+1]  = in[i][j];
		}
	}
#endif


#ifdef PARAM_3
	for (int i = 0; i < _M; i++)
	{
		tmpout = out + i * 1024;

		for (int j = 0; j < 512; j++)
		{
			tmpout[2*j+0]  = in[i][j] >> 8;
			tmpout[2*j+1]  = in[i][j];
		}
	}
#endif
}


#ifdef PARAM_1
void unpack_poly(int64_t* out, uint8_t* in)
{
	int64_t tmp1;
	int64_t tmp2;

	for (int i = 0; i < _hhN; i++)
	{
		tmp1 = ((uint64_t) in[27*i + 0] << 46) | ((uint64_t) in[27*i + 1] << 38) | ((uint64_t) in[27*i + 2] << 30) | ((uint64_t) in[27*i + 3] << 22);
		tmp2 = ((uint64_t) in[27*i + 4] << 14) | ((uint64_t) in[27*i + 5] <<  6) | ((uint64_t) in[27*i + 6] >>  2);

		out[4*i] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[27*i +  6] & 0x03) << 52) | ((uint64_t) in[27*i +  7] << 44) | ((uint64_t) in[27*i +  8] << 36) | ((uint64_t) in[27*i +  9] << 28);
		tmp2 = ((uint64_t)  in[27*i + 10]         << 20) | ((uint64_t) in[27*i + 11] << 12) | ((uint64_t) in[27*i + 12] <<  4) | ((uint64_t) in[27*i + 13] >>  4);

		out[4*i + 1] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[27*i + 13] & 0x0f) << 50) | ((uint64_t) in[27*i + 14] << 42) | ((uint64_t) in[27*i + 15] << 34) | ((uint64_t) in[27*i + 16] << 26);
		tmp2 = ((uint64_t)  in[27*i + 17]         << 18) | ((uint64_t) in[27*i + 18] << 10) | ((uint64_t) in[27*i + 19] <<  2) | ((uint64_t) in[27*i + 20] >>  6);

		out[4*i + 2] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[27*i +  20] & 0x3f) << 48) | ((uint64_t) in[27*i + 21] << 40) | ((uint64_t) in[27*i + 22] << 32) | ((uint64_t) in[27*i + 23] << 24);
		tmp2 = ((uint64_t)  in[27*i +  24]         << 16) | ((uint64_t) in[27*i + 25] <<  8) |  (uint64_t) in[27*i + 26];

		out[4*i + 3] = tmp1 | tmp2;
	}
}
#endif

#ifdef PARAM_2
void unpack_poly(int64_t* out, uint8_t* in)
{
	int64_t tmp1;
	int64_t tmp2;

	for (int i = 0; i < _hN; i++)
	{
		tmp1 = ((uint64_t) in[15*i + 0] << 52) | ((uint64_t) in[15*i + 1] << 44) | ((uint64_t) in[15*i + 2] << 36) | ((uint64_t) in[15*i + 3] << 28);
		tmp2 = ((uint64_t) in[15*i + 4] << 20) | ((uint64_t) in[15*i + 5] << 12) | ((uint64_t) in[15*i + 6] <<  4) | ((uint64_t) in[15*i + 7] >> 4);

		out[2*i] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[15*i +  7] & 0xf) << 56) | ((uint64_t) in[15*i +  8] << 48) | ((uint64_t) in[15*i +  9] << 40) | ((uint64_t) in[15*i + 10] << 32);
		tmp2 = ((uint64_t)  in[15*i + 11] << 24       ) | ((uint64_t) in[15*i + 12] << 16) | ((uint64_t) in[15*i + 13] <<  8) | ((uint64_t) in[15*i + 14]);

		out[2*i + 1] = tmp1 | tmp2;
	}
}
#endif

#ifdef PARAM_3
void unpack_poly(int64_t* out, uint8_t* in)
{
	int64_t tmp1;
	int64_t tmp2;

	for (int i = 0; i < _hhhN; i++)
	{
		tmp1 = ((uint64_t) in[47*i + 0] << 39) | ((uint64_t) in[47*i + 1] << 31) | ((uint64_t) in[47*i + 2] << 23) | ((uint64_t) in[47*i + 3] << 15);
		tmp2 = ((uint64_t) in[47*i + 4] <<  7) | ((uint64_t) in[47*i + 5] >>  1);

		out[8*i] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i +  5] & 0x01) << 46) | ((uint64_t) in[47*i +  6] << 38) | ((uint64_t) in[47*i +  7] << 30) | ((uint64_t) in[47*i +  8] << 22);
		tmp2 = ((uint64_t)  in[47*i +  9]         << 14) | ((uint64_t) in[47*i + 10] <<  6) | ((uint64_t) in[47*i + 11] >>  2);

		out[8*i + 1] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 11] & 0x03) << 45) | ((uint64_t) in[47*i + 12] << 37) | ((uint64_t) in[47*i + 13] << 29) | ((uint64_t) in[47*i + 14] << 21);
		tmp2 = ((uint64_t)  in[47*i + 15]         << 13) | ((uint64_t) in[47*i + 16] <<  5) | ((uint64_t) in[47*i + 17] >>  3);

		out[8*i + 2] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 17] & 0x07) << 44) | ((uint64_t) in[47*i + 18] << 36) | ((uint64_t) in[47*i + 19] << 28) | ((uint64_t) in[47*i + 20] << 20);
		tmp2 = ((uint64_t)  in[47*i + 21]         << 12) | ((uint64_t) in[47*i + 22] <<  4) | ((uint64_t) in[47*i + 23] >>  4);

		out[8*i + 3] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 23] & 0x0f) << 43) | ((uint64_t) in[47*i + 24] << 35) | ((uint64_t) in[47*i + 25] << 27) | ((uint64_t) in[47*i + 26] << 19);
		tmp2 = ((uint64_t)  in[47*i + 27]         << 11) | ((uint64_t) in[47*i + 28] <<  3) | ((uint64_t) in[47*i + 29] >>  5);

		out[8*i + 4] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 29] & 0x1f) << 42) | ((uint64_t) in[47*i + 30] << 34) | ((uint64_t) in[47*i + 31] << 26) | ((uint64_t) in[47*i + 32] << 18);
		tmp2 = ((uint64_t)  in[47*i + 33]         << 10) | ((uint64_t) in[47*i + 34] <<  2) | ((uint64_t) in[47*i + 35] >>  6);

		out[8*i + 5] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 35] & 0x3f) << 41) | ((uint64_t) in[47*i + 36] << 33) | ((uint64_t) in[47*i + 37] << 25) | ((uint64_t) in[47*i + 38] << 17);
		tmp2 = ((uint64_t)  in[47*i + 39]         <<  9) | ((uint64_t) in[47*i + 40] <<  1) | ((uint64_t) in[47*i + 41] >>  7);

		out[8*i + 6] = tmp1 | tmp2;

		tmp1 = ((uint64_t) (in[47*i + 41] & 0x7f) << 40) | ((uint64_t) in[47*i + 42] << 32) | ((uint64_t) in[47*i + 43] << 24) | ((uint64_t) in[47*i + 44] << 16);
		tmp2 = ((uint64_t)  in[47*i + 45]         <<  8) | ((uint64_t) in[47*i + 46]);

		out[8*i + 7] = tmp1 | tmp2;
	}
}
#endif
		

void unpack_s(int64_t out[][_N], uint8_t* in)
{
	uint8_t* tmpin;

	for (int i = 0; i < _M; i++)
	{
		tmpin = in + _hhN * i;

		for (int j = 0; j < _hhN; j++)
		{
			out[i][4*j + 0] = ((((int64_t)tmpin[j]) << 56) >> 62);
			out[i][4*j + 1] = ((((int64_t)tmpin[j]) << 58) >> 62);
			out[i][4*j + 2] = ((((int64_t)tmpin[j]) << 60) >> 62);				
			out[i][4*j + 3] = ((((int64_t)tmpin[j]) << 62) >> 62);
		}
	}
}

void unpack_z(int64_t out[][_N], uint8_t* in)
{
	uint8_t* tmpin;

#ifdef PARAM_1
	for (int i = 0; i < _M; i++)
	{
		tmpin = in + i * _hhhN * 15;

		for (int j = 0; j < _hhhN; j++)
		{
			out[i][8*j + 0] =                                         (((uint64_t) tmpin[15*j +  0]) << 7) | (tmpin[15*j +  1] >> 1); 
			out[i][8*j + 1] = (((uint64_t) tmpin[15*j +  1]) << 14) | (((uint64_t) tmpin[15*j +  2]) << 6) | (tmpin[15*j +  3] >> 2);
			out[i][8*j + 2] = (((uint64_t) tmpin[15*j +  3]) << 13) | (((uint64_t) tmpin[15*j +  4]) << 5) | (tmpin[15*j +  5] >> 3);
			out[i][8*j + 3] = (((uint64_t) tmpin[15*j +  5]) << 12) | (((uint64_t) tmpin[15*j +  6]) << 4) | (tmpin[15*j +  7] >> 4);
			out[i][8*j + 4] = (((uint64_t) tmpin[15*j +  7]) << 11) | (((uint64_t) tmpin[15*j +  8]) << 3) | (tmpin[15*j +  9] >> 5);
			out[i][8*j + 5] = (((uint64_t) tmpin[15*j +  9]) << 10) | (((uint64_t) tmpin[15*j + 10]) << 2) | (tmpin[15*j + 11] >> 6);
			out[i][8*j + 6] = (((uint64_t) tmpin[15*j + 11]) <<  9) | (((uint64_t) tmpin[15*j + 12]) << 1) | (tmpin[15*j + 13] >> 7);
			out[i][8*j + 7] = (((uint64_t) tmpin[15*j + 13]) << 8) | (tmpin[15*j + 14]);
		}

		for (int j = 0; j < _N; j++)
		{
			out[i][j] = (out[i][j] << 49) >> 49; 
		}
	}
#endif

#ifdef PARAM_2
	for (int i = 0; i < _M; i++)
	{
		tmpin = in + i * 512;

		for (int j = 0; j < 256; j++)
		{
			out[i][j]  = ((((int64_t) tmpin[2*j] << 8) | tmpin[2*j+1]) << 48) >> 48;
		}
	}
#endif

#ifdef PARAM_3
	for (int i = 0; i < _M; i++)
	{
		tmpin = in + i * 1024;

		for (int j = 0; j < 512; j++)
		{
			out[i][j]  = ((((int64_t) tmpin[2*j] << 8) | tmpin[2*j+1]) << 48) >> 48;
		}
	}
#endif

}