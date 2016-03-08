/* Bresenham line drawing algorithm. */

/* TEST bresenham
* DESCRIPTION bresenham line drawing algorithm
* CATEGORY integer
* DEFINE TYPE={short,int,long,long long}
* END
*/

#include <bench.h>

#define WIDTH  2000
#define HEIGHT 2000

#define byte unsigned char

typedef int data[WIDTH];

static TYPE videoram[WIDTH*HEIGHT];

static void bresenham_(TYPE x1, TYPE y1, TYPE x2, TYPE y2, TYPE c1)
{
	TYPE dx, dy;
	TYPE ax, ay;
	TYPE sy;
	TYPE x, y;
	TYPE *point, *endpoint;
	TYPE c = c1;
	if (x1 > x2)
	{
		x1 ^= x2 ^= x1 ^= x2;
		y1 ^= y2 ^= y1 ^= y2;
	}
	dx = x2 - x1;
	dy = y2 - y1;
	ax = dx << 1;
	ay = abs (dy) << 1;
	sy = (dy >= 0) ? 1 : -1;
	x = x1;
	y = y1;

	point = videoram + x + y * WIDTH;
	endpoint = videoram + x2 + y2 * WIDTH;
	if (ax > ay)
	{
		register int d = ay - (ax >> 1);
		register int aa = ay;
		if (sy > 0)
		{
			while (point != endpoint)
			{
				*point = c;
				if (d >= 0)
				{
					point += WIDTH;
					d -= ax;
				}
				point++;
				d += aa;
			}
		}
		else
		{
			while (point != endpoint)
			{
				*point = c;
				if (d >= 0)
				{
					point -= WIDTH;
					d -= ax;
				}
				point++;
				d += aa;
			}
		}
	}
	else
	{
		register int d = ax - (ay >> 1);
		register int aa = ax;
		if (sy > 0)
		{
			while (point != endpoint)
			{
				*(point) = c;
				if (d > 0)
				{
					point++;
					d -= ay;
				}
				point += WIDTH;
				d += aa;
			}
		}
		else
		{
			while (point != endpoint)
			{
				*(point) = c;
				if (d > 0)
				{
					point++;
					d -= ay;
				}
				point -= WIDTH;
				d += aa;
			}
		}
	}
	*(point) = c;
}


void bresenham(void)
{
	int i = 0;

	for (i = 0; i < WORKLOAD; i++)
	{
		bresenham_(0, 0, WIDTH-1, HEIGHT-1, 5);
	}
}

SINGLE_BENCHMARK(bresenham())
