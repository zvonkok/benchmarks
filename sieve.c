/*
TEST bell-labs-sieve
DESC sieve of erathostenes
CAT integer
DEF ITYPE={short,int,long,long long}
DEF ITYPE={unsigned short, unsigned int, unsigned long}
END
*/
#include <bench.h>
#define	TRUE	(1==1)
#define	FALSE	(1!=1)
#define	SIZE	WORKLOAD

ITYPE flags[ SIZE ];
void sieve()
{
	register ITYPE 	i, prime, k;

		for ( i = 0; i < SIZE; i++ )
			flags[ i ]	= TRUE;

		for ( i = 0; i < SIZE; i++ )
		{
			if ( flags[ i ] )
			{
				prime	= i + i + 3;
				for ( k = i + prime; k < SIZE; k += prime )
					flags[ k ]	= FALSE;
			}
		}
}

SINGLE_BENCHMARK(sieve());

