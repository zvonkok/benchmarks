/*-----prototypes of functions defined in cflopio.c -----

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.
*/
#include <stdio.h>
#ifdef _WIN32
#include "PCcloops.h"
#endif

extern long modi(long i, long mm);
extern void  tally( FILE *iou, long mode );
extern void  spedup( FILE *iou, char *name[], REAL rate[] );
extern void  sensit( FILE *iou, REAL rates[],REAL wg[],
		    long iq[],REAL sumw[],long map[],REAL tv[],
		    REAL tv1[],REAL tv2[],REAL tv3[],long n);
extern void report( FILE *iou, int iou_flag, long ntk,long nek,REAL flops[],
		   REAL tr[], REAL rates[],long lspan[],REAL wg[],
		   REAL osum[],long id[]);
extern FILE *initio( char *name );
extern void indata( REAL tk[], FILE **iou);
extern void calibr(void);
extern void str8cpy(char *dst, char *src);
extern int str8cmp(char *str1, char *str2);

#define IOU_OK 1
#define IOU_NEG -1

extern int full_out;

