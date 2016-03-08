/*
 * TEST takeuchi
 * DESC calculates a highly recursive function, nice for testing function call overhead
 * DESC - tak x y z = z, if y >= x
 * DESC -           = tak (tak (x-1) y z) (tak (y - 1) z x) (tak (z - 1) x y), otherwise
 * SOURCE Marc Lehmann <pcg@goof.com>
 * CAT integer
 * DEF TYPE={signed char,short,int,long,long long}
 * CAT fp
 * DEF TYPE={float,double,long double}
 * END
 */

#include <bench.h>

TYPE tak (TYPE x, TYPE y, TYPE z)
{
  return (y >= x) ? z
                  : tak ( tak (x-1, y, z), tak (y-1, z, x), tak (z-1, x, y));
}

//SINGLE_BENCHMARK (USE (tak (18, 12, 6)))
void takeuchi(void)
{
	int i = 0;
	for (i = 0; i < WORKLOAD; i++)
	{
		USE (tak (18, 12, 6));
	}
}


SINGLE_BENCHMARK(takeuchi)

