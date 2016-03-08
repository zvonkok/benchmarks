/*-----prototypes of functions defined in cflops.c -----

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.
*/
#include <stdio.h>
#ifdef _WIN32
#include "PCcloops.h"
#endif

extern void set_system(void);
extern double frac(double z);
extern int isigdg(REAL relerr);
extern long modi(long ii, long mm);
extern long nper(long ii, long mm);
extern void where( long mode);
extern void watch( long mode);
extern void verify( FILE *iou );
extern void values(long i);
extern void valid( REAL vx[],long map[],long *lx, REAL bl,
                  REAL x[],REAL bu,long n );
extern void trial( FILE *iou, long i, REAL t0, REAL tj );
extern void trap( long i[], char *name, long mini, long maxi,long meff);
extern void stops(void);
extern void tile( REAL *sm, REAL *si, REAL ox[],long ix[],REAL w[],
                 REAL ew,REAL t,REAL tiles,long n);
extern REAL tick( FILE *iou, int iou_flag, long ntimes);
extern void tests( long i, REAL tempus );
extern void tdigit( REAL *derr, long *nzd, REAL s );
extern void supply(long i);
extern REAL sumo( REAL v[], long n);
extern void statw( REAL stat[],REAL ox[],long ix[], REAL x[],
                  REAL w[],long n);
extern void stats( REAL stat[], REAL x[], long n);
extern void sordid( long i[],REAL w[], REAL v[],long n,long kind);
extern void sizes(long i);
extern void simd( REAL hm[], FILE *iou,int iou_flag,REAL rates[],
		 REAL wg[], REAL fr[],long m, long map[],
		 REAL tv1[], REAL tv2[],REAL tv3[],long n);
extern void signel( REAL v[], REAL scale,REAL bias, REAL n);
extern void seqdig( long nd[], long *isum, REAL a[], REAL b[], long nr);
extern REAL secovt( FILE *iou, int iou_flag );
extern double second( double oldsec);
extern void result( FILE *iou,REAL flops[],REAL tr[],REAL rates[],
                   long lspan[],REAL wg[],REAL osum[],REAL terr[],
                   long id[]);
extern double relerr( double u,double v);
extern void page( FILE *iou);
extern void iqranf( long m[], long mmin,long mmax, long n);
extern void iqran0( long newk);
extern int main(int argc, char *argv[]);
extern void trace( char *name );
extern void space(void);
extern long test( long i );
extern void track( char *name );

/* NOTE: kernel is really in ccernel.c, but declare in this header */
extern void kernel( REAL TK[] );
extern double kernel_12(long lo, long n, REAL *x);
