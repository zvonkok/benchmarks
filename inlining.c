/*
TEST inlining
DESC various inlining tests
DESC position: calling a function that is defined before use vs. calling
DESC position: a function that is defined _after_ use.
DESC macro: calling a (possibly inlined) function vs. using a macro.
RESTYPE norm
SOURCE Marc Lehmann <pcg@goof.com>
CAT misc
END
*/

#include <bench.h>

/* normal function, defined before use.  */
/* this is carefully crafted to generate imul on intel, rather than some shift.  */
static inline int a(int l, int c)
{
	return l*c;
}

/* macro.  */
#define c(l,c) ((l)*(c))

/* normal function, defined after use.  */
static int b(int, int);

int l;

void inlining_macro(int l)
{
	USE (c(l,1)+c(l,2)+c(l,3)+c(l,4)+c(l,5)+c(l,6)+c(l,7)+c(l,8)); /* r2 = GET_CYCLES; */
	/* TAG("macro"   ); RESULT(r2/r1); */
}

void inlining_inline(int l)
{
	USE (a(l,1)+a(l,2)+a(l,3)+a(l,4)+a(l,5)+a(l,6)+a(l,7)+a(l,8)); /* r1 = GET_CYCLES; */
}

void inlining_func(int l)
{
	USE (b(l,1)+b(l,2)+b(l,3)+b(l,4)+b(l,5)+b(l,6)+b(l,7)+b(l,8)); /* r2 = GET_CYCLES; */
}

void inlining(void)
{
	int i = 0;
	int loops = WORKLOAD;
	/* double r1, r2; */
	l = ZERO + 7;

	for (i = 0; i < loops; i++)
	{
		inlining_inline(l);
		inlining_func(l);
		/* TAG("position"); RESULT(r2/r1); */
		inlining_macro(l);
	}
}

static int b(int l, int c)
{
	return l*c;
}

SINGLE_BENCHMARK (inlining())
