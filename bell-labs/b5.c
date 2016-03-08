/*	Bell Labs Benchmark B5
TEST bell-labs5
DESC bell labs benchmark #5
CAT integer
END
*/
#include <bench.h>

#define	xERROR	(struct symbl *) -1
#define	MAXHASH	8
#define	NIDENT	9
#define	OTHER	0
#define	ALPHA	1
#define	NUMERIC	3
#define	ARITH	4
#define	EFATL	0

struct symbl
   {
	char			 ident[ NIDENT ];
	int				 reloc;
	int				 sdind;
	int				 sclass;
	int				 stype;
	int				 value;
	struct symbl	*next;
	struct symbl	*prev;
	struct symbl	*order;
   };

struct symbl	*fsym	= 0;
int				 sdindex;
struct symbl	*hashtb[ MAXHASH ], *lsym, undef;
int				 p2flg, slno;

type( c )

char	c;
   {
	if ( c )
		return	1;
	return	0;
   }

hash( ap )

char	*ap;
   {
	register char	*p;
	register int	 h;

	p	= ap;
	h	= 0;

	while ( type( *p ) & 1 )
	   {
		h	^= *p++;
		if ( p >= ap + NIDENT - 1 )
			break;
	   }

	return	( h & (MAXHASH - 1));
   }

void error( i, s1, s2, j )

int		 i,j;
char	*s1, *s2;
   {
   }

struct symbl	*alloc( i )
   {
	return &undef;
   }

exit1( status )
   {
   }


struct symbl	*addsym( name, val, rel )

char	*name;
int		 val, rel;
   {
	extern int				 sdindex;
	extern struct symbl		*hashtb[];
	extern int				 p2flg, slno;
	extern struct symbl		*fsym, *lsym, undef;
	register int			 i;
	struct symbl			*pl;
	register struct symbl	*p, *pn;
	char					*c;

	if ( *name ==  '\0' )
	   {
		error( EFATL, "syntax requires symbol", name, sdindex );
		return( &undef );
	   }

	sdindex++;
	if ((p = alloc(sizeof *p))  ==   xERROR)
	   {
		p2flg	= 1;
		slno	= 0;
		c	= name;
		while ( type( *c & 1 ) )
			c++;
		*c	= 0;
		error( EFATL, "%s - too many (%d) symbols", name, sdindex );
		exit1( 0 );
	   }

	for ( i = 0; i < NIDENT; i++ )
	   {
		p->ident[i]	= *name;
		if ((type(*name++) & 1 )  != 1)
			break;
	   }

	p->ident[i]	= '\0';
	p->value	= val;
	p->reloc	= rel;
	p->sdind	= 0;
	p->stype	= 0;
	p->sclass	= 0;
	p->next		= 0;

	i	= hash( p->ident );
	p->order	= hashtb[i];
	hashtb[i]	= p;

	if ( fsym  ==  0 )
	   {
		fsym	= p;
		lsym	= p;
		p->next	= 0;
		p->prev	= 0;
		return	p;
	   }

	lsym->next	= p;
	p->prev		= lsym;
	lsym		= p;
	return	p;
   }

void bell_labs5(void)
{
	register int	i;
	struct symbl	*addsym();
	for ( i = 0; i < MAXHASH; i++ )
        hashtb[ i ]=0; 
	lsym=0;
	fsym=0;
	for ( i = 0; i < WORKLOAD; i++ )
		addsym( "asymbol", 2, 5 );
}
SINGLE_BENCHMARK(bell_labs5())
