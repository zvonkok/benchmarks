/*
TEST byte-arith
DESC byte unix benchmark / arithmetic
CAT integer
DEF ITYPE={signed char,short,int,long,long long,unsigned char}
DEF ITYPE={unsigned short, unsigned int, unsigned long, unsigned long long}
END
*/
/*******************************************************************************
*  The BYTE UNIX Benchmarks - Release 3
*          Module: arith.c   SID: 3.3 5/15/91 19:30:19
*
*******************************************************************************
* Bug reports, patches, comments, suggestions should be sent to:
*
*	Ben Smith, Rick Grehan or Tom Yager
*	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
*
*******************************************************************************
*  Modification Log:
*  May 12, 1989 - modified empty loops to avoid nullifying by optimizing
*                 compilers
*  August 28, 1990 - changed timing relationship--now returns total number
*	                  of iterations (ty)
*  November 9, 1990 - made changes suggested by Keith Cantrell
*                        (digi!kcantrel) to defeat optimization
*                        to non-existence
*
******************************************************************************/


char SCCSid[] = "@(#) @(#)arith.c:3.3 -- 5/15/91 19:30:19";
/*
*  arithmetic test
*
*/

#include <stdio.h>
#include <bench.h>


unsigned long iter;


/************************** dumb_stuff *******************/
int dumb_stuff(int i)
{
	#ifndef arithoh
	ITYPE x, y, z;
	z = 0;
	#endif
	/*
	*     101
	* sum       i*i/(i*i-1)
	*     i=2
	*/
	/* notice that the i value is always reset by the loop */
	for (i=2; i<=101; i++)
	{
		#ifndef arithoh
		x = i;
		y = x*x;
		z += y/(y-1);
	}
	return(x+y+z);
	#else
}
return(0);
#endif
}


void arith(void)
{
	int duration;
	int result = 0;
	for (iter = 0; iter < WORKLOAD; iter++)
		USE(result = dumb_stuff(result));
}

SINGLE_BENCHMARK(arith())
