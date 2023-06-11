#include "reduce.h"
#include "params.h"

int64_t caddp(int64_t in) // c sampling -> + Q
{
	in += (in >> 63) & Q;

	return in;
}

int64_t csubp(int64_t in)
{
	
	in -= Q;
	in += (in >> 63) & Q;
	
	return in;
}

int64_t center(int64_t in)
{
	int64_t _hP = (Q-1) >> 1;

	in -= _hP + 1;
	in += ((int64_t)in >> 63) & Q;
	in -= _hP;

	return in;
}

int64_t madd(int64_t a, int64_t b)
{
	return csubp(a + b);
}

int64_t msub(int64_t a, int64_t b)
{
	return csubp(a + (Q - b));
}

