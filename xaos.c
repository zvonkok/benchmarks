/* TEST xaos
 * DESC XaoS (a fractal zoomer) internal loop
 * SOURCE sunsite, x.org etc..
 * CAT misc
 * END
 */
/*
 *     This test is grabbed from:
 *     XaoS, a fast portable realtime fractal zoomer
 *                  Copyright (C) 1996,1997 by
 *
 *      Jan Hubicka          (hubicka@paru.cas.cz)
 *      Thomas Marsh         (tmarsh@austin.ibm.com)
 *
 * Algorithm sloved special kind of pairing problem using dynamic algorithm.
 * Imput is set of floats that are converted to integers, calculated and then
 * generated new floats. Most time is spent in mk_realloc_table.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdio.h>
#include <math.h>
#include <bench.h>

#define ASIZE 16
#define ALIGN(x) (((x)+ASIZE-1)&(~(ASIZE-1)))
#define WIDTH 255
#define NUMBER_BIG 65536*256
#define MYINT_MAX 65536*256
typedef long double number_t;
static number_t xpos[WIDTH];
struct realloc_s
  {
    number_t possition;
    number_t price;
    unsigned int plus;
    int recalculate;
    int symto;
    int symref;
    int dirty;
  } realloc[WIDTH];
typedef struct realloc_s realloc_t;




struct dyn_data
  {
    int price;
    struct dyn_data *previous;
  };

#define FPMUL 64		/*Let multable fit into pentium cache */
#define RANGES 2		/*shift equal to x*RANGE */
#define RANGE 4

#define DSIZEHMASK (0x7)	/*mask equal to x%(DSIZE) */
#define DSIZE (2*RANGE)
#define DSIZES (RANGES+1)	/*shift equal to x*DSIZE */


#define adddata(n,i) (dyndata+(((n)<<DSIZES)+(((i)&(DSIZEHMASK)))))
#define getbest(i) (dyndata+((size)<<DSIZES)+(i))
#define nosetadd ((size*2)<<DSIZES)


#define PRICE(i,i1) (((i)-(i1)) * ((i)-(i1)))
#define NEWPRICE (FPMUL*FPMUL*(RANGE)*(RANGE))

#define NOSETMASK ((unsigned int)0x80000000)
#define END NULL
#define MAXPRICE MYINT_MAX
/*static int dynsize = (int)sizeof (struct dyn_data);*/
#ifndef INT_MIN
#define INT_MIN (- MYINT_MAX - 1)
#endif
#define IRANGE FPMUL*RANGE

#ifdef USE_MULTABLE
static int multable[RANGE * FPMUL * 2];
static int *mulmid;
#endif

/*Functions looks trought rows/columns marked for calculation and tries to use
 *some symetrical one instead
 */

/*FIXME should be threaded...but thread overhead should take more work than
 *do it in one, since it is quite simple and executes just in case fractal
 *on the screen is symetrical and it is quite rare case...who knows
 */
static void			/*INLINE */
preparesymetries (register realloc_t * realloc, int size, register int symi, number_t sym, number_t step)
{
  register int i;
  register int istart = 0;
  number_t fy, ftmp;
  realloc_t *r = realloc, *reallocs;

  sym *= 2;
  i = 2 * symi - size;
  if (i < 0)
    i = 0;
  realloc += i;

  for (; i <= symi; i++, realloc++)
    {				/*makes symetries */
      int j, min = 0;
      number_t dist = NUMBER_BIG, tmp1;

      if (realloc->symto != -1)
	continue;

      fy = realloc->possition;
      realloc->symto = 2 * symi - i;

      if (realloc->symto >= size - RANGE)
	realloc->symto = size - RANGE - 1;

      dist = RANGE * step;
      min = RANGE;
#ifndef NDEBUG
      if (realloc->symto < 0 || realloc->symto >= size)
	{
	  printf ("Internal error #22-1 %i\n",realloc->symto);
	}
#endif
      reallocs = &r[realloc->symto];
      j = (realloc->symto - istart > RANGE) ? -RANGE : (-realloc->symto + istart);

      if (realloc->recalculate)
	{
	  for (; j < RANGE && realloc->symto + j < size - 1; j++)
	    {
	      ftmp = sym - (reallocs + j)->possition;
	      if ((tmp1 = fabs (ftmp - fy)) < dist)
		{
		  if ((realloc == r || ftmp > (realloc - 1)->possition) &&
		      (ftmp < (realloc + 1)->possition))
		    {
		      dist = tmp1;
		      min = j;
		    }
		}
	      else if (ftmp < fy)
		break;
	    }

	}
      else
	{
	  for (; j < RANGE && realloc->symto + j < size - 1; j++)
	    {
	      if (!realloc->recalculate)
		continue;
	      ftmp = sym - (reallocs + j)->possition;
	      if ((tmp1 = fabs (ftmp - fy)) < dist)
		{
		  if ((realloc == r || ftmp > (realloc - 1)->possition) &&
		      (ftmp < (realloc + 1)->possition))
		    {
		      dist = tmp1;
		      min = j;
		    }
		}
	      else if (ftmp < fy)
		break;
	    }
	}
      realloc->symto += min;

      if (min == RANGE || realloc->symto <= symi ||
	  (reallocs = reallocs + min)->symto != -1 || reallocs->symref != -1)
	{
	  realloc->symto = -1;
	  continue;
	}

      if (!realloc->recalculate)
	{
	  realloc->symto = -1;
	  if (reallocs->symto != -1 || !reallocs->recalculate)
	    continue;
	  reallocs->plus = realloc->plus;
	  reallocs->symto = i;
	  istart = realloc->symto - 1;
	  reallocs->dirty = 1;
	  realloc->symref = (int) (reallocs - r);
	  reallocs->recalculate = 0;
	  reallocs->possition = sym - realloc->possition;
	}
      else
	{
	  if (reallocs->symto != -1)
	    {
	      realloc->symto = -1;
	      continue;
	    }
	  istart = realloc->symto - 1;
	  realloc->dirty = 1;
	  realloc->plus = reallocs->plus;
	  realloc->recalculate = 0;
	  reallocs->symref = i;
	  realloc->possition = sym - reallocs->possition;
	}

#ifndef NDEBUG
      if (realloc->symto < -1 || realloc->symto >= size)
	{
	  printf ("Internal error #22 %i\n",realloc->symto);
	}
      if (reallocs->symto < -1 || reallocs->symto >= size)
	{
	  printf ("Internal error #22-2 %i\n",reallocs->symto);
	}
#endif
    }

}
static /*INLINE */ void
newpossitions (realloc_t * realloc, unsigned int size, number_t begin1, number_t end1, number_t * fpos, int yend)
{
  realloc_t *rs, *re, *rend;
  number_t step = size / (end1 - begin1);
  number_t start;
  number_t end;
  rend = realloc + size;
  rs = realloc - 1;
  re = realloc;
  while (rs < rend - 1)
    {
      re = rs + 1;
      if (re->recalculate)
	{
	  while (re < rend && re->recalculate)
	    re++;

	  if (re == rend)
	    end = end1;
	  else
	    end = re->possition;

	  if (rs == realloc - 1)
	    {
	      start = begin1;
	      if (start > end)
		start = end;
	    }
	  else
	    start = rs->possition;

	  if (re == rend && start > end)
	    end = start;

	  if (re - rs == 2)
	    end = (end - start) * 0.5;
	  else
	    end = ((number_t)(end - start)) / (re - rs);


	  switch (yend)
	    {
	    case 1:
	      for (rs++; rs < re; rs++)
		{
		  start += end, rs->possition = start;
		  rs->price = 1 / (1 + fabs (fpos[rs - realloc] - start) * step);
		}
	      break;
	    case 2:
	      for (rs++; rs < re; rs++)
		{
		  start += end, rs->possition = start;
		  rs->price = (fabs (fpos[rs - realloc] - start) * step);
		  if (rs == realloc || rs == rend - 1)
		    rs->price *= 500;
		}
	      break;
	    default:
	      for (rs++; rs < re; rs++)
		{
		  start += end, rs->possition = start;
		  rs->price = (number_t) 1;
		}
	      break;
	    }
	}
      rs = re;
    }
}

/* This is the main reallocation algorithm described in xaos.info
 * It is quite complex since many loops are unrooled and uses custom
 * fixedpoint
 *
 * Takes approx 30% of time so looking for way to do it threaded.
 * Let me know :)
 */
static /*INLINE */ void
mkrealloc_table (number_t * fpos, realloc_t * realloc, unsigned int size, number_t begin, number_t end, number_t sym, unsigned char *tmpdata)
{
  unsigned int i;
  int counter;
  unsigned int ps, ps1 = 0, pe;
  register unsigned int p;
  int bestprice = MAXPRICE;
  realloc_t *r = realloc;
  struct dyn_data *dyndata;
  int yend, y;
  register struct dyn_data **best;
  struct dyn_data **best1, **tmp;
  register int *pos;
  number_t step, tofix;
  int symi = -1;
  unsigned int lastplus = 0;
  struct dyn_data *data;
  register struct dyn_data *previous = NULL, *bestdata = NULL;
  register int myprice;

  pos = (int *) tmpdata;
  best = (struct dyn_data **) (tmpdata + ALIGN ((size + 2) * sizeof (int)));
  best1 = (struct dyn_data **) (tmpdata + ALIGN ((size + 2) * sizeof (int)) + ALIGN (size * sizeof (struct dyn_data **)));
  dyndata = (struct dyn_data *) (tmpdata + ALIGN ((size + 2) * sizeof (int)) + 2 * ALIGN (size * sizeof (struct dyn_data **)));

  tofix = size * FPMUL / (end - begin);
  pos[0] = INT_MIN;
  pos++;
  for (counter = size - 1; counter >= 0; counter--)
    {
      pos[counter] = (int)((fpos[counter] - begin) * tofix);/*first convert everything into fixedpoint */
      if (counter < size - 1 && pos[counter] > pos[counter + 1])	/*Avoid processing of missordered rows.
						   They should happend because of limited
						   precisity of FP numbers */
	pos[counter] = pos[counter + 1];
    }
  pos[size] = MYINT_MAX;
  step = (end - begin) / (number_t) size;
  if (begin > sym || sym > end)	/*calculate symetry point */
    symi = -2;
  else
    {
      symi = (int)((sym - begin) / step);

    }

  ps = 0;
  pe = 0;
  y = 0;

  /* This is first pass that fills table dyndata, that holds information
   * about all ways algorithm thinks about. Correct way is discovered at
   * end by looking backward and determining witch way algorithm used to
   * calculate minimal value*/

  for (i = 0; i < size; i++, y += FPMUL)
    {
      bestprice = MAXPRICE;
      p = ps;			/*just inicialize parameters */

      tmp = best1;
      best1 = best;
      best = tmp;

      yend = y - IRANGE;
      if (yend < -FPMUL)	/*do no allow lines outside screen */
	yend = -FPMUL;

      while (pos[p] <= yend)	/*skip lines out of range */
	p++;
      ps1 = p;
      yend = y + IRANGE;

      /*First try case that current line will be newly calculated */

      /*Look for best way how to connect previous lines */
      if (ps != pe && p > ps)
	{			/*previous point had lines */
	  if (p < pe)
	    {
	      previous = best[p - 1];
	    }
	  else
	    previous = best[pe - 1];
	  myprice = previous->price;	/*find best one */
	}
      else
	{
	  if (i > 0)
	    {			/*previous line had no lines */
	      previous = getbest (i - 1);
	      myprice = previous->price;
	    }
	  else
	    previous = END, myprice = 0;
	}

      data = getbest (i);	/*find store possition */
      myprice += NEWPRICE;
      bestdata = data;
      data->previous = previous;
      bestprice = myprice;	/*calculate best available price */
      data->price = myprice;	/*store data */
#ifdef _UNDEFINED_
      if (yend > end + FPMUL)	/*check bounds */
	yend = end + FPMUL;
#endif
      data = adddata (p, i);	/*calcualte all lines good for this y */

      /* Now try all acceptable connection and calculate best possibility
       * with this connection
       */
      if (ps != pe)
	{			/*in case that previous had also possitions */
	  int price1 = MYINT_MAX;
	  /*At first line of previous interval we have only one possibility
	   *don't connect previous line at all.
	   */
	  if (p == ps)
	    {			/*here we must skip previous point */
	      if (pos[p] != pos[p + 1])
		{
		  previous = getbest (i - 1);
		  myprice = previous->price;
		  myprice += PRICE (pos[p], y);		/*store data */
		  if (myprice < bestprice)
		    {		/*calcualte best */
		      bestprice = myprice, bestdata = data;
		      data->price = myprice;
		      data->previous = previous;
		    }
		}
	      best1[p] = bestdata;
	      data += DSIZE;
	      p++;
	    }

	  previous = NULL;
	  price1 = myprice;
	  while (p < pe)
	    {			/*this is area where intervals of current point and previous one are crossed */
	      if (pos[p] != pos[p + 1])
		{
		  if (previous != best[p - 1])
		    {

		      previous = best[p - 1];
		      price1 = myprice = previous->price;

		      /*In case we found revolutional point, we should think
		       *about changing our gusesses in last point too - don't
		       *connect it at all, but use this way instead*/
		      if (myprice + NEWPRICE < bestprice)	/*true in approx 2/3 of cases */
			{
			  bestprice = myprice + NEWPRICE, bestdata = data - DSIZE;
			  (bestdata)->price = bestprice;
			  (bestdata)->previous = previous + nosetadd;
			  best1[p - 1] = bestdata;
			}
		    }
		  else
		    myprice = price1;

		  myprice += PRICE (pos[p], y);		/*calculate price of new connection */

		  if (myprice < bestprice)	/*2/3 of cases */
		    {		/*if it is better than previous, store it */
		      bestprice = myprice, bestdata = data;
		      data->price = myprice;
		      data->previous = previous;
		    }
		  else if (pos[p] > y)
		    {
		      best1[p] = bestdata;
		      data += DSIZE;
		      p++;
		      break;
		    }

		}

	      best1[p] = bestdata;
	      data += DSIZE;
	      p++;
	    }
	  while (p < pe)
	    {			/*this is area where intervals of current point and previous one are crossed */
#ifdef DEBUG
	      if (pos[p] != pos[p + 1])
		{
		  if (previous != best[p - 1])
		    {
		      printf("Missoptimization found!\n");
		    }
		}
#endif
#ifdef _UNDEFINED_
	      /* Experimental results show, that probability for better approximation
	       * in this area is extremly low. Maybe it never happends.
	       * I will have to think about it a bit more... It seems to have
	       * to do something with meaning of universe and god... no idea
	       * why it is true.
	       *
	       * Anyway it don't seems to worth include the expensive tests
	       * here.
	       */
	      if (pos[p] != pos[p + 1])
		{
		  if (previous != best[p - 1])
		    {

		      previous = best[p - 1];
		      myprice = previous->price;

		      /*In case we found revolutional point, we should think
		       *about changing our gusesses in last point too - don't
		       *connect it at all, but use this way instead*/
		      if (myprice + NEWPRICE < bestprice)	/*true in approx 2/3 of cases */
			{
			  bestprice = myprice + NEWPRICE, bestdata = data - DSIZE;
			  (bestdata)->price = bestprice;
			  (bestdata)->previous = previous + nosetadd;
			  best1[p - 1] = bestdata;
			}
		      myprice += PRICE (pos[p], y);	/*calculate price of new connection */
		      if (myprice < bestprice)
			{	/*if it is better than previous, store it */
			  bestprice = myprice, bestdata = data;
			  data->price = myprice;
			  data->previous = previous;
			}
		    }
		}
#endif
	      best1[p] = bestdata;
	      data += DSIZE;
	      p++;
	    }

	  /* OK...we passed crossed area. All next areas have same previous
	   * situation so our job is easier
	   * So find the best solution once for all od them
	   */
	  if (p > ps)
	    {
	      previous = best[p - 1]; /*find best one in previous */
	      price1 = previous->price;
	    }
	  else
	    {
	      previous = getbest (i - 1);
	      price1 = previous->price;
	    }

	  /* Since guesses for "revolutional point" was allways one
	   * step back, we need to do last one*/
	  if (price1 + NEWPRICE < bestprice && p > ps1)
	    {
	      myprice = price1 + NEWPRICE;
	      bestprice = myprice, bestdata = data - DSIZE;
	      (bestdata)->price = myprice;
	      (bestdata)->previous = previous + nosetadd;
	      best1[p - 1] = bestdata;
	      myprice -= NEWPRICE;
	    }

	  while (pos[p] < yend)
	    {
	      if (pos[p] != pos[p + 1])
		{
		  myprice = price1;
		  myprice += PRICE (pos[p], y);		/*store data */
		  if (myprice < bestprice)
		    {		/*calcualte best */
		      bestprice = myprice, bestdata = data;
		      data->price = myprice;
		      data->previous = previous;
		    }
		  else if (pos[p] > y)
		    break;
		}

	      best1[p] = bestdata;
	      data += DSIZE;
	      p++;
	    }
	  while (pos[p] < yend)
	    {
	      best1[p] = bestdata;
	      p++;
	    }
	}
      else
	{
	  /* This is second case - previous y was not mapped at all.
	   * Situation is simplier now, since we know that behind us is
	   * large hole and our decisions don't affect best solution for
	   * previous problem. Se we have just one answer
	   * Situation is similiar to latest loop in previous case
	   */
	  int myprice1;		/*simplified loop for case that previous
				   y had no lines */
	  if (pos[p] < yend)
	    {
	      if (i > 0)
		{
		  previous = getbest (i - 1);
		  myprice1 = previous->price;
		}
	      else
		previous = END, myprice1 = 0;
	      while (pos[p] < yend)
		{
		  if (pos[p] != pos[p + 1])
		    {
		      myprice = myprice1 + PRICE (pos[p], y);
		      if (myprice < bestprice)
			{
			  data->price = myprice;
			  data->previous = previous;
			  bestprice = myprice, bestdata = data;
			}
		      else if (pos[p] > y)
			break;
		    }
		  best1[p] = bestdata;
		  p++;
		  data += DSIZE;
		}
	      while (pos[p] < yend)
		{
		  best1[p] = bestdata;
		  p++;
		}
	    }
	}
      /*previous = ps; *//*store possitions for next loop */
      ps = ps1;
      ps1 = pe;
      pe = p;
    }
  realloc = realloc + size;
  yend = (int)((begin > fpos[0]) && (end < fpos[size - 1]));

  if (pos[0] > 0 && pos[size - 1] < size * FPMUL)
    yend = 2;



  /*This part should be made threaded quite easily...but does it worth
   *since it is quite simple loop 0...xmax
   */
  for (i = size; i > 0;)
    {				/*and finally traces the path */
      struct dyn_data *bestdata1;
      realloc--;
      i--;
      realloc->symto = -1;
      realloc->symref = -1;
      bestdata1 = bestdata->previous;

      if (bestdata1 >= dyndata + nosetadd || bestdata >= dyndata + ((size) << DSIZES))
	{
	  if (bestdata1 >= dyndata + nosetadd)
	    bestdata1 -= nosetadd;

	  realloc->recalculate = 1;
	  realloc->dirty = 1;
	  lastplus++;

	  if (lastplus >= size)
	    lastplus = 0;

	  realloc->plus = lastplus;

	}
      else
	{
	  p = ((unsigned int)(bestdata - dyndata)) >> DSIZES;
	  realloc->possition = fpos[p];
	  realloc->plus = p;
	  realloc->dirty = 0;
	  realloc->recalculate = 0;
	  lastplus = p;
	}
      bestdata = bestdata1;
    }



  newpossitions (realloc, size, begin, end, fpos, yend);
  realloc = r;
  if (symi <= (int)size && symi >= 0)
    {
      preparesymetries (r, (int)size, symi, sym, step);
    }


}

void
init_tables ()
{
  int i;

  /*c->dirty = 2; */
  for (i = 0; i < WIDTH; i++)
    xpos[i] = -1 + i*2.0/WIDTH;
}

char tmpdata[ALIGN ((WIDTH) * (DSIZE + 1) * (int)sizeof (struct dyn_data)) + 2 * ALIGN (WIDTH * (int)sizeof (struct dyn_data **)) + ALIGN ((WIDTH + 2) * (int)sizeof (int))*2];

double size=1;
int
do_test ()
{ int i;
  mkrealloc_table (xpos, realloc, WIDTH, -size, size, 0, tmpdata);
  size = size * 0.9;
  if (size < 0.001)
    size = 1;
  for (i = 0; i < WIDTH; i++)
    xpos[i] = realloc[i].possition;
}


void xaos(void)
{
	int i = 0;
	int loops = WORKLOAD;
	for (i = 0; i < loops; i++)
	{
		  int count=0;
		  init_tables();
		  do_test();
	}

}

SINGLE_BENCHMARK(xaos())

