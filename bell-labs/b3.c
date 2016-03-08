/*	Bell Labs benchmark B3
TEST bell-labs3
DESC bell labs benchmark #3
CAT integer
END
*/
#include <bench.h>

char	test[8][7]	=
   {
	"red\0\0\0",	"yellow",	"blue\0\0",	"orange",
	"indigo",		"violet",	0
   };

int	(*qscmp)();
int	qses	= 7;
int	peek	= 0;
bell_labs3()
{
  int i;
  int stcmp();
  char *ldr;
	for ( i = 0; i < WORKLOAD; i++ )
	   {
		qscmp	= stcmp;
		ldr		= &test[0][0];
		qs1( ldr, ldr + 49 );
	   }
}


qs1( a, l )

char	*a, *l;
   {
	register char	*i, *j;
	register		 es;
	register char	*lp, *hp;
	int				 n, c;
	int				 ldiv();

	es	= qses;

start:
	if ((n = l - a)  <=  es)
		return;
	ldiv(0, n, 2 * es);
	n	= es * peek;
	hp	= lp	= a + n;
	i	= a;
	j	= l - es;

	for (;;)
	   {
		if ( i < lp )
		   {
			if ((c = (*qscmp)(i, lp))  ==  0)
			   {
				qserc( i, lp -= es );
				continue;
			   }
			if ( c < 0 )
			   {
				i	+= es;
				continue;
			   }
		   }
loop:
		if ( j > hp )
		   {
			if ((c = (*qscmp)(hp, j))  == 0)
			   {
				qserc( hp += es, j);
				goto	loop;
			   }
			if ( c > 0 )
			   {
				if ( i ==  lp )
				   {
					qsterc( i, hp += es, j );
					i	= lp += es;
					goto	loop;
				   }
				qserc( i, j );
				j	-= es;
				i	+= es;
				continue;
			   }
			j	-= es;
			goto	loop;
		   }
		if ( i == lp )
		   {
			if ( lp - a >= l - hp )
			   {
				qs1( hp + es, l );
				l	= lp;
			   }
			else
			   {
				qs1( a, lp );
				a	= hp + es;
			   }
			goto	start;
		   }

		qsterc( j, lp -= es, i );
		j	= hp	-= es;
	   }
   }

qserc(i, j )
char	*i, *j;
   {
	register char	*ri, *rj, c;
	register int	n;

	n	= qses;
	ri	= i;
	rj	= j;

	do
	   {
		c		= *ri;
		*ri++	= *rj;
		*rj++	= c;
	   }
	while ( --n );
   }

qsterc( i, j, k )

char	*i, *j, *k;
   {
	register char	*ri, *rj, *rk;
	register int	c;
	int	n;

	n	= qses;
	ri	= i;
	rj	= j;
	rk	= k;

	do
	   {
		c		= *ri;
		*ri++	= *rk;
		*rk++	= *rj;
		*rj++	= c;
	   }
	while (--n);
   }

ldiv( l, m, q )
   {
	
   }


stcmp( ss1, ss2 )
char	*ss1, *ss2;
   {
	register char	*s1, *s2;

	s1	= ss1;
	s2	= ss2;

	while( *s1  ==  *s2++ )
	   {
		if ( *s1++  ==  '\0' )
			return	0;
	   }
	return	(*s1 - *--s2);
   }
   
SINGLE_BENCHMARK(bell_labs3())
