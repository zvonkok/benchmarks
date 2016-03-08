/*	Bell Labs Benchmark B1: Ackerman's function
TEST bell-labs1
DESC ackerman's function
CAT integer
DEF ITYPE={signed char,short,int,long,unsigned char}
DEF ITYPE={unsigned short, unsigned int, unsigned long}
END
*/
#include <bench.h>
long	entries;

ITYPE
bell_labs1(m, n)
ITYPE m, n;
{
	entries++;
	if (m == 0)
		return n + 1;

	if (n == 0)
		return bell_labs1(m - 1, 1);
	else
		return bell_labs1(m - 1, bell_labs1(m, n - 1));
}

SINGLE_BENCHMARK(USE(bell_labs1(3,WORKLOAD)));

