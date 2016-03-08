/*
* TEST dataconv
* DESC data type conversion effectiveness
* SOURCE Marc Lehmann <pcg@goof.com>
* CAT misc
* DEF FROM={signed char,short,long,long long,float,double}
* DEF TO={float,double,long}
* END
*/

#include <bench.h>

FROM a[20] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9,
10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
TO b[20];

int dataconv()
{
	int i; 

	for (i = 0; i < WORKLOAD; i++)
	{
		b[0] = a[0],
		b[1] = a[1],
		b[2] = a[2],
		b[3] = a[3],
		b[4] = a[4],
		b[5] = a[5],
		b[6] = a[6],
		b[7] = a[7],
		b[8] = a[8],
		b[9] = a[9],
		b[10] = a[10],
		b[11] = a[11],
		b[12] = a[12],
		b[13] = a[13],
		b[14] = a[14],
		b[15] = a[15],
		b[16] = a[16],
		b[17] = a[17],
		b[18] = a[18],
		b[19] = a[19];
	}
	return b;
}

SINGLE_BENCHMARK(USE(dataconv()))
