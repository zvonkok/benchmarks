/*	Bell Labs Benchmark b6
TEST bell-labs6
DESC release buffers
CAT integer
END
release buffers
*/
#include <bench.h>
struct	buf
   {
	int			 b_flags;
	struct buf	*b_forw;
	struct buf	*b_back;
	struct buf	*av_forw;
	struct buf	*av_back;
	int			 b_dev;
	int			 b_wcount;
	char		*b_addr;
	char		*b_xmem;
	char		*b_blkno;
	char		 b_error;
	char		*b_resid;
   };

struct dummy
   {
	int		integ;
   }	stuff;

struct dummy	*PS	= &stuff;
struct buf		bfreelist;

#define	B_WRITE		0
#define	B_READ		01
#define	B_DONE		02
#define	B_ERROR		04
#define	B_BUSY		010
#define	B_PHYS		020
#define	B_MAP		040
#define	B_WANTED	0100
#define	B_AGE		0200
#define	B_ASYNC		0400
#define	B_DELWRI	01000

static bell_labs6()
   {
	long		 i;
	struct buf	*rbp;
	int l;

	rbp	= &bfreelist;
	rbp->av_forw	= &bfreelist;
	rbp->av_back	= &bfreelist;
	rbp->b_forw		= &bfreelist;
	rbp->b_back		= &bfreelist;

	for ( i = 0; i < 100000L; i++ )
	   {
		bfreelist.b_flags	= (B_WANTED | B_AGE );
		brelse( &bfreelist );
	   }
   }


brelse( bp )

struct buf	*bp;
   {
	register struct buf	*rbp, **backp;
	register int		 sps;
	
	rbp	= bp;
	if ( rbp->b_flags & B_WANTED )
		wakeup( rbp );
	if (bfreelist.b_flags & B_WANTED)
	   {
		bfreelist.b_flags	&=	~B_WANTED;
		wakeup( &bfreelist );
	   }
	if ( rbp->b_flags & B_ERROR )
	   {
		rbp->b_dev	|= 0200;
		rbp->b_flags	&=	~(B_ERROR | B_DELWRI);
	   }

	sps		= PS->integ;
	sp16();

	if ( rbp->b_flags & B_AGE )
	   {
		backp				= &bfreelist.av_forw;
		(*backp)->av_back	= rbp;
		rbp->av_forw		= *backp;
		*backp				= rbp;
		rbp->av_forw		= &bfreelist;
	   }
	else
	   {
		backp				= &bfreelist.av_back;
		(*backp)->av_forw	= rbp;
		rbp->av_back		= *backp;
		*backp				= rbp;
		rbp->av_back		= &bfreelist;
	   }

	rbp->b_flags &= ~(B_WANTED | B_BUSY | B_ASYNC | B_AGE );
	bfreelist.b_wcount++;
	PS->integ	= sps;
   }

wakeup( bp )
   {}

sp16()
   {}
   
   SINGLE_BENCHMARK(bell_labs6())
