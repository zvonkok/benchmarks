/*
TEST palette
DESC rgb palette approximation
SOURCE allegro (graphics library for djgpp)
CAT misc
END
*/

/* This routine is grabbed from allegro sources. It is wrote by me, so there
should not be copyright issues. *./
/* Who are you? Do you think this is ok?? how stupid! ;-> */

/* Algorithm is combination of foodfill and priority queues (special case of dijskra) */

/* create_rgb_table:
*  Fills an RGB_MAP lookup table with conversion data for the specified
*  palette. This is the faster version by Jan Hubicka.
*
*  Uses alg. similiar to foodfill - it adds one seed per every color inforeach
*  palette to its best possition. Then areas around seed are filled by
*  same color because it is best aproximation for them, and then areas
*  about them etc...
*
*  It does just about 80000 tests for distances and this is about 100
*  times better than normal 256*32000 tests so the caluclation time
*  is now less than one second at all computers I tested.
*/
#include <bench.h>

#define PAL_SIZE     256
typedef struct RGB
{
	unsigned char r, g, b;
	unsigned char filler;
}
RGB;

typedef RGB PALLETE[PAL_SIZE];
typedef struct
{
	unsigned char data[32][32][32];
}
RGB_MAP;
static RGB_MAP tab;
static PALLETE pal;

/* 1.5k lookup table for color matching */
static unsigned col_diff[3*128];

/* bestfit_init:
* Color matching is done with weighted squares, which are much faster
* if we pregenerate a little lookup table...
*/
static void
bestfit_init ()
{
	int i;

	for (i = 1; i < 64; i++)
	{
		int k = i * i;
		col_diff[0 + i] = col_diff[0 + 128 - i] = k * (59 * 59);
		col_diff[128 + i] = col_diff[128 + 128 - i] = k * (30 * 30);
		col_diff[256 + i] = col_diff[256 + 128 - i] = k * (11 * 11);
	}
}

void create_rgb_table(RGB_MAP *table, PALLETE pal)
{
	#define UNUSED 65535
	#define LAST 65532

	/* macro add adds to single linked list */
	#define add(i)    (next[(i)] == UNUSED ? (next[(i)] = LAST, \
		(first != LAST ? (next[last] = (i)) : (first = (i))), \
		(last = (i))) : 0)

	/* same but w/o checking for first element */
	#define add1(i)   (next[(i)] == UNUSED ? (next[(i)] = LAST, \
		next[last] = (i), \
		(last = (i))) : 0)

	/* calculates distance between two colors */
	#define dist(a1, a2, a3, b1, b2, b3) \
	(col_diff[ ((a2) - (b2)) & 0x7F] + \
		(col_diff + 128)[((a1) - (b1)) & 0x7F] + \
		(col_diff + 256)[((a3) - (b3)) & 0x7F])

	/* converts r,g,b to position in array and back */
	#define pos(r, g, b) \
	(((r) / 2) * 32 * 32 + ((g) / 2) * 32 + ((b) / 2))

	#define depos(pal, r, g, b) \
	((b) = ((pal) & 31) * 2, \
		(g) = (((pal) >> 5) & 31) * 2, \
		(r) = (((pal) >> 10) & 31) * 2)

	/* is current color better than pal1? */
	#define better(r1, g1, b1, pal1) \
	(((int)dist((r1), (g1), (b1), \
		(pal1).r, (pal1).g, (pal1).b)) > (int)dist2)

	/* checking of possition */
	#define dopos(rp, gp, bp, ts) \
	if ((rp > -1 || r > 0) && (rp < 1 || r < 61) && \
		(gp > -1 || g > 0) && (gp < 1 || g < 61) && \
	(bp > -1 || b > 0) && (bp < 1 || b < 61)) { \
	i = first + rp * 32 * 32 + gp * 32 + bp; \
	if (ts ? data[i] != val : !data[i]) { \
		dist2 = (rp ? (col_diff+128)[(r+2*rp-pal[val].r) & 0x7F] : r2) + \
		(gp ? (col_diff)[(g+2*gp-pal[val].g) & 0x7F] : g2) + \
		(bp ? (col_diff+256)[(b+2*bp-pal[val].b) & 0x7F] : b2); \
		if (better((r+2*rp), (g+2*gp), (b+2*bp), pal[data[i]])) { \
			data[i] = val; \
			add1 (i); \
		} \
	} \
	}

	int i, curr, r, g, b, val, r2, g2, b2, dist2;
	unsigned short next[32*32*32];
	unsigned char *data;
	int first = LAST;
	int last = LAST;
	int count = 0;
	int cbcount = 0;

	#define AVERAGE_COUNT   18000

	if (col_diff[1] == 0)
		bestfit_init();

	memset(next, 255, sizeof(next));
	memset(table->data, 0, sizeof(char)*32*32*32);

	data = (unsigned char *)table->data;

	/* add starting seeds for foodfill */
	for (i=1; i<256; i++) {
		curr = pos(pal[i].r, pal[i].g, pal[i].b);
		if (next[curr] == UNUSED) {
			data[curr] = i;
			add(curr);
		}
	}

	/* main foodfill: two versions of loop for faster growing in blue axis */
	while (first != LAST) {
		depos(first, r, g, b);

		/* calculate distance of current color */
		val = data[first];
		r2 = (col_diff+128)[((pal[val].r)-(r)) & 0x7F];
		g2 = (col_diff    )[((pal[val].g)-(g)) & 0x7F];
		b2 = (col_diff+256)[((pal[val].b)-(b)) & 0x7F];

		/* try to grow to all directions */
		dopos( 0, 0, 1, 1);
		dopos( 0, 0,-1, 1);
		dopos( 1, 0, 0, 1);
		dopos(-1, 0, 0, 1);
		dopos( 0, 1, 0, 1);
		dopos( 0,-1, 0, 1);

		/* faster growing of blue direction */
		if ((b > 0) && (data[first-1] == val)) {
			b -= 2;
			first--;
			b2 = (col_diff+256)[((pal[val].b)-(b)) & 0x7F];

			dopos(-1, 0, 0, 0);
			dopos( 1, 0, 0, 0);
			dopos( 0,-1, 0, 0);
			dopos( 0, 1, 0, 0);

			first++;
		}

		/* get next from list */
		i = first;
		first = next[first];
		next[i] = UNUSED;

		/* second version of loop */
		if (first != LAST) {
			depos(first, r, g, b);

			val = data[first];
			r2 = (col_diff+128)[((pal[val].r)-(r)) & 0x7F];
			g2 = (col_diff    )[((pal[val].g)-(g)) & 0x7F];
			b2 = (col_diff+256)[((pal[val].b)-(b)) & 0x7F];

			dopos( 0, 0, 1, 1);
			dopos( 0, 0,-1, 1);
			dopos( 1, 0, 0, 1);
			dopos(-1, 0, 0, 1);
			dopos( 0, 1, 0, 1);
			dopos( 0,-1, 0, 1);

			if ((b < 61) && (data[first + 1] == val)) {
				b += 2;
				first++;
				b2 = (col_diff+256)[((pal[val].b)-(b)) & 0x7f];

				dopos(-1, 0, 0, 0);
				dopos( 1, 0, 0, 0);
				dopos( 0,-1, 0, 0);
				dopos( 0, 1, 0, 0);

				first--;
			}

			i = first;
			first = next[first];
			next[i] = UNUSED;
		}

		count++;
	}
}

void allegro(void)
{

	int k;

	for (k = 0; k < WORKLOAD; k++)
	{
	int i;
	bestfit_init();
	for(i=0;i<PAL_SIZE;i++)
	{
	pal[i].r=rand()%64, pal[i].g=rand()%64, pal[i].b=rand()%64;
}
	create_rgb_table(&tab, pal);
}
}

SINGLE_BENCHMARK(allegro())
