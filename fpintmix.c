/* TEST fpintmix
* DESCRIPTION mix of floating point variable and -constant tests
* CATEGORY fp
* DEFINE TYPE={float,double,long double}
* END
*/
#include <bench.h>
static long maxiter = WORKLOAD;
typedef TYPE number_t;
static int
calc (number_t cre, int pre)
{
	unsigned long iter = maxiter;
	number_t zre;
	int i;
	zre = cre;
	while ((iter))
	{
		zre = zre + pre + i;
		i=2*zre;
		zre = i*zre - pre + i;
		i=-3*zre;
		zre = i*zre + pre + i;
		i=4*zre;
		zre = i*zre - pre + i;
		i=-5*zre;
		zre = i*zre + pre + i;
		i=6*zre;
		iter--;

	}
	iter = maxiter - iter + zre;
	return (iter);
}

void fpintmix(void)
{
	int i = 0;
	int res = 0;
	int loops = 1000;

	number_t z1=ZERO;
	number_t z2=ZERO;

	res += calc(z1, z2);
	
	USE (res);
}

SINGLE_BENCHMARK(fpintmix())
