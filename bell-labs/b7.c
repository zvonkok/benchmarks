/*	Bell Labs Benchmark b7
TEST bell-labs7
DESC statistics
CAT integer
DEF ITYPE={short,long,unsigned short,int,unsigned int,unsigned long}
END
*/
#include <bench.h>

ITYPE 	array[10]	=
        {
                500,	1000,	2000,	4000,	100,
                200,	400,	800,	1600,	3200
        };

bell_labs7()
{
	register unsigned	i;

	for ( i = 0; i < WORKLOAD; i++ )
		stat( array, 10 );

	for ( i = 0; i < WORKLOAD; i++ )
		stat( array, 10 );
}


stat( arr, nel )

int		*arr;
ITYPE		 nel;
{
	register int	i;
	register int	sum, mean;
	ITYPE				getmean();

	sum	= 0;
	for ( i = 0; i < nel; i++ )
		sum	+= arr[ i ];

	mean	= getmean(arr, nel, sum );

	getmedian( arr, nel, sum );
}

ITYPE getmean( arr, nel, sum )

int		*arr, nel, sum;
{
	register int	i, mean;

	mean	= 0;
	for ( i = 0; i < nel; i++ )
		mean	+= arr[i];

	mean	/=	sum;
	return	mean;
}

getmedian( arr, nel, sum )

int	*arr, nel, sum;
{
	register int	i;
	register ITYPE	part_sum;

	part_sum	= 0;
	for ( i = 0; i < nel; i++ )
	{
		if ( (part_sum += arr[i])  >  (sum >> 2))
			break;
	}
}

SINGLE_BENCHMARK(bell_labs7())
