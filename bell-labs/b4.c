/*	Bell Labs Benchmark B4
TEST bell-labs4
DESC bell labs benchmark #4
CAT integer
END
*/
#include <bench.h>

struct	qlist
   {
	int		 q_count;
	char	*q_char;
	char	*q_free;
   };

struct	packet
   {
	char	*nextpacket;
	char	 pdata[6];
   };

#define	CERASE	'#'
#define	CEOT	004
#define	CKILL	'@'
#define	CQUIT	034
#define	CINTR	0177
#define	SIGINT	2
#define	SIGGIT	3
#define	xERROR	(-1)
#define	NIL		0

int		t_col;

#define	CANBSIZ	132

char	*cbp1;
char	canonb[CANBSIZ];

putq( c, queue )

char	 c, *queue;
   {
	return	 0;
   }

wflushtty(q)
char *q;
   {
	return	0;
   }

canon( ac, queue )

int		 ac;
char	*queue;
   {
	register char	*bp;
	register int	c;

	c	= ac;
	bp	= cbp1;

	if ( bp[-1] != '\\')
	   {
		switch ( c )
		   {
			case CERASE:
			   {
				if ( bp > &canonb[1] )
					--bp;
				break;
			   }

			case CKILL:
			   {
				bp	= &canonb[1];
				ttyoutput('\n', queue );
				break;
			   }

			default:
			   {
				*bp++	= c;
			   }
		   }
	   }
	else
	   {
		if ( c == CERASE  ||  c == CKILL )
			bp[-1]	= c;
		else
			*bp++	= c;
	   }

	if ( c == '\n'  ||  c == CEOT  ||  bp > &canonb[CANBSIZ] )
	   {
		cbp1	= bp;
		bp		= &canonb[1];
		while ( bp < cbp1 )
			putq( *bp++, queue );
		bp		= &canonb[1];
	   }

	cbp1	= bp;
	return	0;
   }

ttyoutput( ac, queue )

int		 ac;
char	*queue;
   {
	register int	c;
	int	extern		t_col;

	c	= ac & 0177;
	if ( c == '\t' )
	   {
		do ttyoutput( ' ', queue);	while (t_col & 07);
		return;
	   }

	if ( c == '\n' )
		ttyoutput( '\r', queue );
	while( putq(c, queue) == xERROR )
		wflushtty( queue );

	switch ( c )
	   {
		default:
		   {
			t_col++;
			break;
		   }

		case 010:
		   {
			if ( t_col )	t_col--;
			break;
		   }

		case '\n':
		   {
			t_col	= 0;
			break;
		   }

		case '\r':
		   {
			ttyoutput(0177,queue);
			ttyoutput(0177,queue);
			ttyoutput(0177,queue);
			ttyoutput(0177,queue);
			t_col	= 0;
		   }
	   }
	return	0;
   }

void bell_labs4(void)
{
	extern char		*cbp1;
	register int	 p, i;
	int l;

	for ( i = 0; i < WORKLOAD; i++ )
	   {
		cbp1	= &canonb[1];
		canon(p='t', canonb);
		canon(p, canonb);
		canon(p, canonb);
		canon(p, canonb);
		canon(p=CKILL, canonb);
		canon(p='t', canonb);
		p	= 0;
	   }
}

SINGLE_BENCHMARK(bell_labs4())

