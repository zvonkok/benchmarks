/* TEST mandelbrot
* DESCRIPTION This is mandelbrot set loop iterating formula z=z^2+c using complex numbers
* CATEGORY complex
* DEFINE TYPE={float,double,long double}
* END
*/

#include <bench.h>

static long maxiter = WORKLOAD;

typedef __complex__ TYPE number_t;

static int mand_calc(number_t c, number_t p)
{
	number_t rp = 0, ip = 0;
	unsigned long iter = maxiter;
	number_t z;
	z = c;
	while ((iter) && (__real__ z * __real__ z + __imag__ z * __imag__ z < 4))
	{
		z = z * z + p;
		iter--;
	}
	iter = maxiter - iter;
	return (iter);
}

void mandelbrot(void)
{
	int i;
	int loops = 0;
	number_t z = ZERO;

	i += mand_calc(z, z);

	printf("iter %d\n", i);
	USE (i);
}

SINGLE_BENCHMARK(mandelbrot())
