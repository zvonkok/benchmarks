/* This loop tests mix of floating point and constant */
/*
TEST fpconmix
DESC floating point and integer mix
CAT fp
DEF TYPE={float,double,long double}
END
*/

#include <bench.h>

static long maxiter = WORKLOAD;
typedef TYPE number_t;

static int
calc (number_t cre, number_t pre)
{
	unsigned long iter = maxiter;
	number_t zre;
	zre = cre;
	while ((iter))
	{
		zre = 3*zre + pre + 20;
		zre = 5*zre * pre + 201;
		zre = 6*zre * pre + 202;
		zre = 7*zre * pre + 203;
		zre = 8*zre * pre + 204;
		zre = 9*zre + pre + 205;
		zre = 10*zre * pre + 206;
		zre = 11*zre * pre + 207;
		zre = 12*zre * pre + 208;
		zre = 13*zre * pre + 209;
		iter--;

	}
	iter = maxiter - iter + zre;
	return (iter);
}


void fpconmix(void)
{
	int i = 0;
	int res = 0;

	number_t z1=ZERO;
	number_t z2=ZERO;

	res += calc(z1, z2);
	
	USE (res);
}

SINGLE_BENCHMARK(fpconmix())
