/* TEST mandelbrot-unrolled
* DESC unrolled version of the mandelbrot set loop calculating z=z*z+c in complex numbers
* CATEGORY fp
* DEFINE TYPE={float,double,long double}
* END
*/
#include <bench.h>
static long maxiter = WORKLOAD;
typedef TYPE number_t;
static int
mand_calc (number_t cre,
	number_t cim,
	number_t pre,
	number_t pim)
{
	number_t rp = 0, ip = 0;
	unsigned long iter = maxiter;
	number_t zre, zim;
	zre = cre;
	zim = cim;
	while ((iter) && (rp + ip < 4))
	{
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;
		ip = (zim * zim);
		zim = (zim * zre) * 2 + pim;
		rp = (zre * zre);
		zre = rp - ip + pre;
		iter--;

	}
	iter = maxiter - iter;
	return (iter);
}

void mandelbrot_int_unroll(void)
{
	int i;
	int loops = 0;
	number_t z1=ZERO;
	number_t z2=ZERO;
	number_t z3=ZERO;
	number_t z4=ZERO;
	i += mand_calc(z1, z2, z3, z4);
	USE (i);
}

SINGLE_BENCHMARK(mandelbrot_int_unroll())
