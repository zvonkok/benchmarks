/* I/O routines for the ANSI C Livermore Loops driver
   Written by Steven Langer
   Lawrence Livermore National Laboratory

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.
*/
#include <string.h>
#include <stdlib.h>
#include "cflopio.h"
#include "cflops.h"
#include <time.h>

extern void exit(int code);

/*-----Fortran intrinsics converted-----*/
#ifdef abs
#undef abs
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define abs(x) ((x)>=0?(x):-(x))
#define mod(x,y) ((x)%(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
/*-----end of Fortran intrinsics-----*/

/* make sure not to allocate storage for common blocks */
#undef ALLOC_COMMON
#define SET_VARS
#include "c_cernel.h"
#include "c_tags.h"
#include "c_debug.h"
#include "c_tau.h"
#include "c_order.h"
#include "c_sysid.h"
#include "c_rats.h"

#undef SET_VARS
#include "c_cernel.h"
#include "c_tags.h"
#include "c_debug.h"
#include "c_tau.h"
#include "c_order.h"
#include "c_sysid.h"
#include "c_rats.h"

extern void s_trim(char *str);

int full_out= 0;

void str8cpy(char *dst, char *src)
{
  long len, i;

  /* copy an 8 character string, padding with spaces if necessary.
     dst MUST be at least 9 characters long!! */
  len= strlen(src);
  if(len > 8) len= 8;
  for(i= 0; i < len; i++) {
    dst[i]= src[i];
  }
  for(i= len; i < 8; i++) {
    dst[i]= ' ';
  }
  dst[8]= '\0';
}

int str8cmp(char *str1, char *str2)
{
  /* compare two 8 character strings */
  return strncmp(str1, str2, 8);
}

void calibr(void)
{
/*      SUBROUTINE CALIBR
    *********************************************************************
                                                                          *
         CALIBR - Cpu clock calibration tests accuracy of SECOND function.*
                                                                          *
         CALIBR tests function SECOND by using it to time a computation   *
         repeatedly.  These SECOND timings are written to stdout(terminal)*
         one at a time as the cpu-clock is read, so we can observe a real *
         external clock time and thus check the accuracy of SECOND code.  *
         Comparisons with an external clock require a stand-alone run.    *
         Otherwise compare with system charge for total job cpu time.     *
                                                                          *
         Sample Output from CRAY-YMP1:                                    *
                                                                          *
                                                                          *
     CPU CLOCK CALIBRATION:  START STOPWATCH NOW !                        *
               TESTS ACCURACY OF FUNCTION SECOND()                        *
               Monoprocess this test, stand-alone, no TSS                 *
               Verify  T or DT  observe external clock:                   *
                                                                          *
            -------     -------      ------      -----                    *
            Total T ?   Delta T ?    Mflops ?    Flops                    *
            -------     -------      ------      -----                    *
      1        0.00        0.00        9.15    4.00000e+04    4.98000e-02 *
      2        0.01        0.01       11.67    1.20000e+05    8.98000e-02 *
      3        0.02        0.01       12.84    2.80000e+05    1.69800e-01 *
      4        0.04        0.02       13.47    6.00000e+05    3.29800e-01 *
      5        0.09        0.05       13.81    1.24000e+06    6.49800e-01 *
      6        0.18        0.09       14.00    2.52000e+06    1.28980e+00 *
      7        0.36        0.18       14.12    5.08000e+06    2.56980e+00 *
      8        0.72        0.36       14.19    1.02000e+07    5.12980e+00 *
      9        1.44        0.72       14.20    2.04400e+07    1.02498e+01 *
     10        2.88        1.44       14.23    4.09200e+07    2.04898e+01 *
     11        5.74        2.87       14.27    8.18800e+07    4.09698e+01 *
     12       11.48        5.74       14.27    1.63800e+08    8.19298e+01 *
     13       22.98       11.50       14.26    3.27640e+08    1.63850e+02 *
     14       45.92       22.94       14.27    6.55320e+08    3.27690e+02 *
     15       91.88       45.96       14.26    1.31068e+09    6.55369e+02 *
    *********************************************************************
*/

#undef limitn
#define limitn 101
#undef ndim
#define ndim (101+10)

#undef cumtim_1
#define cumtim_1(a1) cumtim[a1-1]
#undef y_1
#define y_1(a1) y[a1-1]
#undef x_1
#define x_1(a1) x[a1-1]

  static REAL  x[(101+10)], y[(101+10)], cumtim[10];
  static long i, k, j;
  static REAL t2, t1, t0, rn, flops, deltat, totalt;
  static long nflop, m, n, l;
  /*-----end-of-declarations-----*/
  /*     CALL TRACE ("CALIBR  ")*/
  cumtim_1(1)= 0.0e0;
  t0= second( cumtim_1(1));
  printf("\n\nCPU CLOCK CALIBRATION:  START STOPWATCH NOW !\n");
  printf("           TESTS ACCURACY OF FUNCTION SECOND()\n");
  printf("           Monoprocess this test, stand-alone, no TSS\n");
  printf("           Verify  T or DT  observe external clock:\n\n");
  printf("           -------     -------      ------      -----\n");
  printf("           Total T ?   Delta T ?    Mflops ?    Flops\n");
  printf("           -------     -------      ------      -----\n");
  l= 0;
  n= 0;
  m= 200;
  nflop= 0;
  totalt= 0.00e0;
  deltat= 0.00e0;
  flops= 0.00e0;
  rn= 0.00e0;
  t1= 0.00e0;
  t2= 0.00e0;
  cumtim_1(1)= 0.0e0;
  t2= second( cumtim_1(1));
  if( t2> 1.00e04 ) goto L_911;
  if( t2< 1.00e-8 ) goto L_911;

L_10:
  l++;
  m= m + m;

  x_1(1)= 0.0098000e0;
  y_1(1)= 0.0000010e0;
  for (i=2 ; i<=limitn ; i++) {
    y_1(i)= y_1(1);
  }
  /*                                  Compute LFK Kernel 11  m times*/
  for (j=1 ; j<=m ; j++) {
    for (k=2 ; k<=limitn ; k++) {
      x_1(k)= x_1(k-1) + y_1(k);
    }
    x_1(1)= x_1(limitn);
  }

  t1= t2;
  cumtim_1(1)= 0.0e0;
  t2= second( cumtim_1(1));
  /*                           IF elapsed time can be observed, Print Mark.*/
  totalt= t2 - t0;
  deltat= t2 - t1;
  nflop= nflop + (limitn - 1) * m;
  if( deltat > 2.00e0  ||  l>12 )  {
    n= n + 1;
    rn= nflop;
    flops= 1.00e-6 *( nflop/( totalt +1.00e-9) );
    printf("   %2ld%12.2f%12.2f%12.2f%15.4e%15.4e\n",
           l, totalt, deltat, flops, rn, x_1(limitn) );
  }
  if( deltat < 200.0e0  ||  n<3 )  goto L_10;

  if( n<=0 )  {
    printf("   %2ld%12.2f%12.2f%12.2f%15.5e%15.5e\n",
           l, totalt, deltat, flops, rn, x_1(limitn) );
  }
  exit(1);

 L_911:
  printf("FATAL(CALIBR): cant measure time using func SECOND()\n");
  printf("\n            using SECOND():  totalt=%20.8e ?\n", totalt);
  exit(1);
}


void indata( REAL tk[], FILE **iou)
{
  /*
           INDATA     initialize variables
   */
#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_order.h"
#include "block.h"
#include "c_sysid.h"

#undef tk_1
#define tk_1(a1) tk[a1-1]

  /* load static data arrays */
  blockdata();

  tk_1(1)= 0.00e0;
  tk_1(2)= 0.00e0;
  testov= 0.00e0;
  ticks = 0.00e0;
  tclock= 0.00e0;
  tsecov= 0.00e0;
  tic   = 0.00e0;

  jr    = 1;
  nruns = 1;
  il    = 1;
  mk    = 1;
  ik    = 1;

  inseq = 0;
  isave = 0;
  iret  = 0;

  loops2= 1;
  mpylim= loops2;
  mpy   = 1;
  multi = 1;
  mucho = 1;
  l     = 1;
  loop  = 1;
  lp    = loop;
  n     = 0;

  ion= initio(koutfil);
  ion_flag= 1;
  *iou= ion ;

  trace ("indata  ");
  printf(" *********************************************\n");
  printf(" THE LIVERMORE  FORTRAN KERNELS _MFLOPS_ TEST:\n");
  printf(" *********************************************\n");
  printf("\nSize of REAL variable is %ld bytes\n\n", sizeof(REAL));
  fprintf(ion," *********************************************\n");
  fprintf(ion," THE LIVERMORE  FORTRAN KERNELS _MFLOPS_ TEST:\n");
  fprintf(ion," *********************************************\n");
  fprintf(ion,"\nSize of REAL variable is %ld bytes\n\n", sizeof(REAL));
  fprintf(ion, " >>> USE 72 SAMPLES LFK TEST RESULTS SUMMARY (line 330+)\n");
  fprintf(ion, " >>> USE ALL RANGE STATISTICS FOR OFFICIAL QUOTATIONS.  \n");
  track ("indata  ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_order.h"
#include "c_sysid.h"
}


FILE *initio( char *name )
{
  /************************************************************************
                                                                          *
           INITIO - open output file "name" and return FILE *             *
                                                                          *
             name - name to assign to disk file                           *
                                                                          *
    *********************************************************************
  */
  FILE *iou;
  trace("INITIO  ");

  iou= fopen(name, "w");
  track ("INITIO  ");
  return iou;
}


void report( FILE *iou, int iou_flag, long ntk,long nek,REAL flops[],
            REAL tr[],REAL rates[],long lspan[],REAL wg[],
            REAL osum[],long id[])
{
  /************************************************************************
                                                                          *
         REPORT -  Prints Statistical Evaluation Of Fortran Kernel Timings*
                                                                          *
         iou    -  Logical Output Device Number                           *
         ntk    -  Total number of Kernels to Edit in Report              *
         nek    -  Number of Effective Kernels in each set to Edit        *
         FLOPS  -  Array:  Number of Flops executed by each kernel        *
         TR     -  Array:  Time of execution of each kernel(microsecs)    *
         RATES  -  Array:  Rate of execution of each kernel(megaflops/sec)*
         LSPAN  -  Array:  Span of inner DO loop in each kernel           *
         WG     -  Array:  Weight assigned to each kernel for statistics  *
         OSUM   -  Array:  Checksums of the results of each kernel        *
         ID     -  Array:  ?????????????        *
    *********************************************************************

     Version: 22/DEC/86  MF508
     CHECK FOR CLOCK CALIBRATION ONLY:
     Total Job    Cpu Time =     2.47860E+03 Sec.
     Total 24 Kernels Time =     2.17686E+03 Sec.
     Total 24 Kernels Flops=     3.38982E+09 Flops

                                                        F.H.MCMAHON  1991
    ********************************************************************
*/
#define SET_VARS
#include "c_cernel.h"
#include "c_sysid.h"

#undef rates_1
#define rates_1(a1) rates[a1-1]
#undef tr_1
#define tr_1(a1) tr[a1-1]
#undef flops_1
#define flops_1(a1) flops[a1-1]
#undef id_1
#define id_1(a1) id[a1-1]
#undef lspan_1
#define lspan_1(a1) lspan[a1-1]
#undef osum_1
#define osum_1(a1) osum[a1-1]
#undef wg_1
#define wg_1(a1) wg[a1-1]
#undef name_1
#define name_1(a1) name[a1-1]
#undef rate_1
#define rate_1(a1) rate[a1-1]
#undef lq_1
#define lq_1(a1) lq[a1-1]
#undef lvl_1
#define lvl_1(a1) lvl[a1-1]
#undef hm_1
#define hm_1(a1) hm[a1-1]
#undef stat1_1
#define stat1_1(a1) stat1[a1-1]
#undef stat2_1
#define stat2_1(a1) stat2[a1-1]
#undef vl1_1
#define vl1_1(a1) vl1[a1-1]
#undef vl_1
#define vl_1(a1) vl[a1-1]
#undef csum1_1
#define csum1_1(a1) csum1[a1-1]
#undef tv_1
#define tv_1(a1) tv[a1-1]
#undef tv1_1
#define tv1_1(a1) tv1[a1-1]
#undef tv2_1
#define tv2_1(a1) tv2[a1-1]
#undef tv4_1
#define tv4_1(a1) tv4[a1-1]
#undef tv5_1
#define tv5_1(a1) tv5[a1-1]
#undef flops1_1
#define flops1_1(a1) flops1[a1-1]
#undef flops2_1
#define flops2_1(a1) flops2[a1-1]
#undef map_1
#define map_1(a1) map[a1-1]
#undef map1_1
#define map1_1(a1) map1[a1-1]
#undef map2_1
#define map2_1(a1) map2[a1-1]
#undef map3_1
#define map3_1(a1) map3[a1-1]
#undef map4_1
#define map4_1(a1) map4[a1-1]
#undef ispan1_1
#define ispan1_1(a1) ispan1[a1-1]
#undef ispan2_1
#define ispan2_1(a1) ispan2[a1-1]
#undef in_1
#define in_1(a1) in[a1-1]
#undef in2_1
#define in2_1(a1) in2[a1-1]
#undef wt1_1
#define wt1_1(a1) wt1[a1-1]
#undef wt2_1
#define wt2_1(a1) wt2[a1-1]
#undef rt1_1
#define rt1_1(a1) rt1[a1-1]
#undef rt2_1
#define rt2_1(a1) rt2[a1-1]

#undef nt
#define nt 4

  static double sum;  /* double even for a float run */
  static REAL run, frac;
  static long mrl, ll, i1, j, i2, lo, nq, i;
  static REAL weights, precision, efficiency, q1, q2;
  static REAL mean;
  static REAL q3, samples, span, avgeff, peak, twt;
  static REAL rneto, som, precis, bu, bl, fuzz;
  static long lv, nd, k, neff, meff;
  static char *name[nt];
  static REAL rate[nt];
  static long lq[5];
  static REAL hm[12], stat1[20], stat2[20];
  static REAL vl1[141], vl[141], csum1[141];
  static REAL tv[141], tv1[141], tv2[141], tv4[141], tv5[141];
  static REAL flops1[141], flops2[141];
  static long map1[141], map2[141], map3[141], map[141];
  static long ispan1[141], ispan2[141], in[141], in2[141];
  static REAL wt1[141], wt2[141], rt1[141], rt2[141];
  static long lvl[10];
  static long kall= 0;
  /*-----end-of-declarations-----*/


  trace ("report  ");

  if( iou_flag < 0) goto L_73;

		       meff= 0;
  neff= 0;
  fuzz= 1.0e-9;
  for (k=1 ; k<=ntk ; k++) {
    vl_1(k)= lspan_1(k);
  }

  bl= 1.0e-5;
  bu= 1.0e+5;
  valid( tv, map, &neff,  bl, rates, bu, ntk);
  /*
          Compress valid data sets mapping on MAP.
    */
  nd= 0;
  for (k=1 ; k<=neff ; k++) {
    map1_1(k)=   modi( map_1(k),nek);
    flops1_1(k)= flops_1( map_1(k));
    rt1_1(k)=    tr_1( map_1(k));
    vl1_1(k)=    vl_1( map_1(k));
    ispan1_1(k)= lspan_1( map_1(k));
    wt1_1(k)=    wg_1( map_1(k));
    tv1_1(k)=    rates_1( map_1(k));
    csum1_1(k)=  osum_1( map_1(k));
    nd +=    id_1( map_1(k));
  }
  if( nd <= 8*neff )  nd -= 16*((neff-1+24)/24);
  precis= nd/( neff + fuzz);

  som= 0.00e0;
  sum= 0.00e0;
  for (k=1 ; k<=neff ; k++) {
    som += flops1_1(k);
    sum += rt1_1(k);
  }
  rneto= som/(sum + fuzz);

  statw( stat1,tv,in, vl1,wt1,neff);
  lv= stat1_1(1);

  statw( stat1,tv,in, tv1,wt1,neff);
  twt= stat1_1(6);
  /*                             compute average efficiency= GM/Max*/
  kall++;
  peak= 0.00e0;
  if( kall<=1 || il==im ) {
    peak= stat1_1(4);
  }
  avgeff= (100.0e0* stat1_1(10))/( peak + fuzz);
  fprintf(iou,"\n\n\n\n\n\n\n\n\n\n\n\n");
  page( iou);
  fprintf(iou,"******************************************** \n");
  if( ntk == nek )  {
    fprintf(iou,"THE LIVERMORE  FORTRAN KERNELS:  M F L O P S   \n");
  } else {
    fprintf(iou,"THE LIVERMORE  FORTRAN KERNELS:  * SUMMARY *   \n");
  }
  fprintf(iou,"******************************************** \n");
  fprintf(iou,"\nSize of REAL variable is %ld bytes\n\n", sizeof(REAL));
  fprintf(iou,"\n             Computer :  %s\n", komput);
  fprintf(iou,"             System   :  %s\n", kontrl);
  fprintf(iou,"             Compiler :  %s\n", kompil);
  fprintf(iou,"             Date     :  %s\n", kalend);
  fprintf(iou,"\n        When the computer performance range is very large\n");
  fprintf(iou,"        the net Mflops rate of many Fortran programs and\n");
  fprintf(iou,"        workloads will be in the sub-range between the equi-\n");
  fprintf(iou,"        weighted Harmonic and Arithmetic means depending\n");
  fprintf(iou,"        on the degree of code parallelism and optimization.\n");
  fprintf(iou,"        The least biased central measure is the Geometric\n");
  fprintf(iou,"        Mean of 72 rates,  quoted +- a standard deviation.\n");
  fprintf(iou,"        Mean Mflops rates imply the average efficiency of a\n");
  fprintf(iou,"        computing system since the peak rate is well known.\n");
  fprintf(iou,"        LFK test measures a lower bound for a Multi-processor\n");
  fprintf(iou,"        and N * LFK rates project an upper bound for N-procs.\n");
  fprintf(iou,"\nKERNEL  FLOPS   MICROSEC   MFLOP/SEC SPAN WEIGHT  CHECK-SUMS             OK \n");
  fprintf(iou,"------  -----   --------   --------- ---- ------  ---------------------- -- \n");
  for(k= 1; k <= neff; k++) {
    fprintf(iou,"%2ld%11.3e%11.4e%12.4f %4ld %6.2f%24.16e %2ld\n",
	    map1_1(k),  flops1_1(k), rt1_1(k), tv1_1(k),
	    ispan1_1(k), wt1_1(k), csum1_1(k), id_1(k) );
  }
  fprintf(iou,"------  -----   --------   --------- ---- ------  ---------------------- -- \n");

  fprintf(iou,"%2ld%11.4e%11.4e%12.4f %4ld                              %4ld\n",
          neff, som, sum, rneto, lv, nd);
  fprintf(iou,"\n        MFLOPS    RANGE:             REPORT ALL RANGE STATISTICS:\n");
  fprintf(iou,"        Mean DO Span   =  %5ld\n", lv);
  fprintf(iou,"        Code Samples   =  %5ld\n", ntk);
  fprintf(iou,"\n        Maximum   Rate =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(4));
  fprintf(iou,"        Quartile  Q3   =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(14));
  fprintf(iou,"        Average   Rate =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(1));
  fprintf(iou,"        Geometric Mean =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(10));
  fprintf(iou,"        Median    Q2   =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(7));
  fprintf(iou,"        Harmonic  Mean =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(5));
  fprintf(iou,"        Quartile  Q1   =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(13));
  fprintf(iou,"        Minimum   Rate =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(3));
  fprintf(iou,"\n\n");
  fprintf(iou,"        Standard  Dev. =  %12.4f Mega-Flops/Sec. \n",
          stat1_1(2));
  fprintf(iou,"        Avg Efficiency =  %10.2f%% Program & Processor\n",
          avgeff);
  fprintf(iou,"        Mean Precision =  %10.2f   Decimal Digits \n", precis);
  if( ntk != nek )  {
    printf("\n\n");
    printf("******************************************** \n");
    printf("THE LIVERMORE  FORTRAN KERNELS:  * SUMMARY *   \n");
    printf("******************************************** \n");
    printf("\n             Computer :  %s\n", komput);
    printf("\n             System   :  %s\n", kontrl);
    printf("\n             Compiler :  %s\n", kompil);
    printf("\n             Date     :  %s\n", kalend);
    printf("\n        MFLOPS    RANGE:             REPORT ALL RANGE STATISTICS:\n");
    printf("        Mean DO Span   =  %5d\n", lv);
    printf("        Code Samples   =  %5d\n", ntk);
    printf("\n        Maximum   Rate =  %12.4f Mega-Flops/Sec. \n",
           stat1_1( 4));
    printf("        Quartile  Q3   =  %12.4f Mega-Flops/Sec. \n",
           stat1_1(14));
    printf("        Average   Rate =  %12.4f Mega-Flops/Sec. \n",
           stat1_1( 1));
    printf("        Geometric Mean =  %12.4f Mega-Flops/Sec. \n",
           stat1_1(10));
    printf("        Median    Q2   =  %12.4f Mega-Flops/Sec. \n",
           stat1_1( 7));
    printf("        Harmonic  Mean =  %12.4f Mega-Flops/Sec. \n",
           stat1_1( 5));
    printf("        Quartile  Q1   =  %12.4f Mega-Flops/Sec. \n",
           stat1_1(13));
    printf("        Minimum   Rate =  %12.4f Mega-Flops/Sec. \n",
           stat1_1(3));
    printf("\n\n");
    printf("        Standard  Dev. =  %12.4f Mega-Flops/Sec. \n",
           stat1_1(2));
    printf("        Avg Efficiency =  %10.2f%% Program & Processor\n",
           avgeff);
    printf("        Mean Precision =  %10.2f   Decimal Digits \n", precis);
  }
  name_1(1)= komput;
  name_1(2)= komput;
  name_1(3)= kompil;
  name_1(4)= "        ";
  rate_1(1)= stat1_1(1);
  rate_1(2)= stat1_1(10);
  rate_1(3)= stat1_1(5);
  rate_1(4)= stat1_1(2);

  if( ntk != nek )  {
    fprintf(iou,"<<<<<<<<<<<<<<<<<<<<<<<<<<<*>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    fprintf(iou,"< BOTTOM-LINE:   72 SAMPLES LFK TEST RESULTS SUMMARY. >\n");
    fprintf(iou,"< USE RANGE STATISTICS ABOVE FOR OFFICIAL QUOTATIONS. >\n");
    fprintf(iou,"<<<<<<<<<<<<<<<<<<<<<<<<<<<*>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    page( iou);

    if( iovec==1 ) {
      fprintf( iou,"\n\ntop quartile: best architecture/application match \n");
      /*
	      Compute compression index-list MAP1:  Non-zero weights.
	*/
      bl= 1.0e-6;
      bu= 1.0e+6;
      valid( tv,map1, &meff,  bl, wt1, bu, neff);
      /*
	      Re-order data sets mapping on IN (descending order of MFlops).
	*/
      for (k=1 ; k<=meff ; k++) {
        map3_1(k)= in_1( map1_1(k));
      }

      if( meff>0 )  {
        trap( map3, " report  ", 1, neff,meff);
      }

      for (k=1 ; k<=meff ; k++) {
        i=   map3_1(k);
        flops2_1(k)= flops1_1(i);
        rt2_1(k)=    rt1_1(i);
        ispan2_1(k)= ispan1_1(i);
        wt2_1(k)=    wt1_1(i);
        tv2_1(k)=    tv1_1(i);
        map2_1(k)=   modi( map_1(i),nek);
      }
      /*                         Sort kernels by performance into quartiles*/
      nq= meff/4;
      lo= meff -4*nq;
      lq_1(1)= nq;
      lq_1(2)= nq + nq + lo;
      lq_1(3)= nq;
      i2= 0;

      for (j=1 ; j<=3 ; j++) {
        i1= i2 + 1;
        i2 += lq_1(j);
        ll= i2 - i1 + 1;
        statw( stat2,tv,in2, &(tv2_1(i1)), &(wt2_1(i1)),ll);
        frac= stat2_1(6)/( twt +fuzz);
        fprintf(iou,"\n\n");
        fprintf(iou,"\nKERNEL  FLOPS   MICROSEC   MFLOP/SEC SPAN WEIGHT\n");
        fprintf(iou,"\n------  -----   --------   --------- ---- ------\n");
        for(k=i1; k <= i2; k++) {
          fprintf(iou,"%2ld%11.4e%11.4e%12.4f %4ld %6.2f\n", map2_1(k),
		    flops2_1(k), rt2_1(k), tv2_1(k), ispan2_1(k), wt2_1(k) );
        }
        fprintf(iou,"\n------  -----   --------   --------- ---- ------\n");

        fprintf(iou,"\n        Frac.  Weights =  %12.4f\n", frac);
        fprintf(iou,"        Average   Rate =  %12.4f Mega-Flops/Sec. \n", stat2_1(1));
        fprintf(iou,"        Harmonic  Mean =  %12.4f Mega-Flops/Sec. \n", stat2_1(5));
        fprintf(iou,"        Standard  Dev. =  %12.4f Mega-Flops/Sec. \n", stat2_1(2));
      }

    }

  }
  /*
               Sensitivity analysis of harmonic mean rate to 49 workloads
    */
  sensit(   iou,rates,wg,iq,sumw, map,tv,tv4,tv2,tv5, ntk);

  /*
               Sensitivity analysis of harmonic mean rate to SISD/SIMD model
    */
  simd( hm, iou,IOU_OK,rates,wg,fr,9, map,tv,tv4,tv2, ntk);


  if( ntk != nek )  {
    if( iovec==1 )  {
      page( iou);
      mrl= nruns;
      if( nruns>8) mrl= 8;

      for (k=1 ; k<=mk ; k++) {
        for (j=im ; j<=ml ; j++) {
          sum= 0.0e0;
          for (i=1 ; i<=mrl ; i++) {
            sum += csums_3(i,j,k);
            csums_3(i,j,k)= sum;
          }
        }
      }
	  if(full_out) {
        for (i=1 ; i<=mrl ; i++) {
          if( (i!=1)&&(i!=mrl))  continue;
          fprintf(iou,"\n\n Cumulative Checksums:  RUN=%5d\n", i);
          fprintf(iou,"\n k    VL=%5ld%24ld%24ld\n",
		        lvl_1(1), lvl_1(2), lvl_1(3) );
          for (k=1 ; k<=mk ; k++) {
            fprintf(iou,"%2ld%24.16e%24.16e%24.16e\n", k,
        	  csums_3(i,1,k), csums_3(i,2,k), csums_3(i,3,k) );
		  }
		}
	  }
    }

    spedup( iou, name, rate );
  }
  lvl_1(il)= lv;
 L_73:
  track ("report  ");
  return;

#undef SET_VARS
#include "c_cernel.h"
#include "c_sysid.h"
}


void sensit( FILE *iou, REAL rates[],REAL wg[],long iq[],
	    REAL sumw[], long map[],REAL tv[],REAL tv1[],
	    REAL tv2[],REAL tv3[],long n)
{
  /************************************************************************
                                                                          *
         SENSIT  - Sensitivity Of Harmonic Mean Rate(Mflops) 49 Workloads *
                                                                          *
         iou     - input scalar,  i/o unit number                         *
         RATES   - input array ,  execution rates (Mflops)                *
         WG      - input array ,  weights paired with RATES               *
         IQ      - input array ,  1 or 2 quartiles specifier              *
         SUMW    - input array ,  workload fractions.                     *
                                                                          *
         MAP,TV,TV1,TV2,TV3    -  output temporary arrays                 *
         n       - input scalar,  number of rates, etc.                   *
                                                                          *
    *********************************************************************
    */

#undef map_1
#define map_1(a1) map[a1-1]
#undef sumw_1
#define sumw_1(a1) sumw[a1-1]
#undef wg_1
#define wg_1(a1) wg[a1-1]
#undef rates_1
#define rates_1(a1) rates[a1-1]
#undef iq_1
#define iq_1(a1) iq[a1-1]
#undef tv3_1
#define tv3_1(a1) tv3[a1-1]
#undef tv2_1
#define tv2_1(a1) tv2[a1-1]
#undef tv1_1
#define tv1_1(a1) tv1[a1-1]
#undef tv_1
#define tv_1(a1) tv[a1-1]
#undef nr1_1
#define nr1_1(a1) nr1[a1-1]
#undef nr2_1
#define nr2_1(a1) nr2[a1-1]
#undef stat2_1
#define stat2_1(a1) stat2[a1-1]

  static long  nr1[10], nr2[10];
  static REAL  stat2[20];
  static char  *tag[4];
  static long k2, k1, j, mq, k, neff, meff, i;
  static REAL ot, xt, p, q, r, fuzz, sumo, bu, bl;
  /*-----end-of-declarations-----*/

  tag[0]= "1st qt: ";
  tag[1]= "2nd qt: ";
  tag[2]= "3rd qt: ";
  tag[3]= "4th qt: ";

  trace ("sensit  ");
  /*               Compress valid data sets RATES,  mapping on MAP.*/

  meff= 0;
  neff= 0;
  bl= 1.0e-5;
  bu= 1.0e+5;
  valid( tv1,map, &neff, bl, rates, bu, n);

  for (k=1 ; k<=neff ; k++) {
    tv3_1(k)=    wg_1( map_1(k));
  }

  /*                Compress valid data sets WG,  mapping on MAP.*/

  valid( tv3,map, &meff, bl, tv3, bu, neff);

  for (k=1 ; k<=meff ; k++) {
    tv_1(k)=tv1_1( map_1(k));
  }
  /*                 Sort selected rates into descending order*/

  sordid( map,tv2,   tv,meff,2);

  if(full_out) {
    page( iou);
    fprintf(iou,"\n\n");
    fprintf(iou,"\n\n");
    fprintf(iou,"\n\n");
    fprintf(iou,"                   SENSITIVITY ANALYSIS \n");
    fprintf(iou,"\n\n");
    fprintf(iou,"        The sensitivity of the harmonic mean rate (Mflops)  \n");
    fprintf(iou,"        to various weightings is shown in the table below.  \n");
    fprintf(iou,"        Seven work distributions are generated by assigning \n");
    fprintf(iou,"        two distinct weights to ranked kernels by quartiles.\n");
    fprintf(iou,"        Forty nine possible cpu workloads are then evaluated\n");
    fprintf(iou,"        using seven sets of values for the total weights:   \n");
    fprintf(iou,"\n\n");
    fprintf(iou,"            ------ ------ ------ ------ ------ ------ ------\n");
    fprintf(iou,"  %8s      O      O      O      O      O      X      X\n", tag[0]);
    fprintf(iou,"  %8s      O      O      O      X      X      X      O\n", tag[1]);
    fprintf(iou,"  %8s      O      X      X      X      O      O      O\n", tag[2]);
    fprintf(iou,"  %8s      X      X      O      O      O      O      O\n", tag[3]);
    fprintf(iou,"            ------ ------ ------ ------ ------ ------ ------\n");
    fprintf(iou,"  Total\n Weights                    Net Mflops:\n   X    O\n");
    fprintf(iou," ---- ---- \n");
  }

  if( meff <= 0 )  goto L_73;
  fuzz= 1.0e-9;
  r= meff;
  mq= (meff+3)/4;
  q= mq;
  j= 1;
  for (i=8 ; i>=2 ; i+=-2) {
    nr1_1(i  )= j;
    nr1_1(i+1)= j;
    nr2_1(i  )= j + mq + mq - 1;
    nr2_1(i+1)= j + mq - 1;
    j += mq;
  }

  for (j=1 ; j<=7 ; j++) {
    sumo= 1.0e0 - sumw_1(j);
    for (i=1 ; i<=7 ; i++) {
      p= iq_1(i)*q;
      xt= sumw_1(j)/(p + fuzz);
      ot= sumo   /(r - p + fuzz);
      for (k=1 ; k<=meff ; k++) {
        tv3_1(k)= ot;
	  }
      k1= nr1_1(i+2);
      k2= nr2_1(i+2);
      for (k=k1 ; k<=k2 ; k++) {
        tv3_1(k)= xt;
	  }
      statw( stat2,tv,map, tv2,tv3,meff);
      tv1_1(i)= stat2_1(5);
	}
    if(full_out) {
      fprintf(iou,"\n%5.2f%5.2f %7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f\n",
            sumw_1(j), sumo, tv1_1(1), tv1_1(2), tv1_1(3), tv1_1(4), 
            tv1_1(5), tv1_1(6), tv1_1(7) );
	}
  }
  if(full_out) {
    fprintf(iou," ---- ---- \n");
    fprintf(iou,"            ------ ------ ------ ------ ------ ------ ------\n");
  }

 L_73:
  track ("sensit  ");
  return;
}


void  spedup( FILE *iou, char *name[], REAL rate[] )
{
  /************************************************************************
                                                                          *
         SPEDUP  - Computes Speed-ups: A circumspect method of comparison.*
                   Computers are ranked by their Geometric Mean Rates.    *
                                                                          *
         iou     - input scalar,  i/o unit number                         *
         NAME    - input array ,  system name                             *
         RATE    - input array ,  execution rates (Mflops)                *
                                                                          *
    *********************************************************************
    */

#define SET_VARS
#include "c_tags.h"
#include "c_rats.h"

#undef nsys
#define nsys 5
#undef ns
#define ns (nsys+1)
#undef nd
#define nd 11
#undef nt
#define nt 4
#undef name_1
#define name_1(a1) name[a1-1]
#undef ratio_1
#define ratio_1(a1) ratio[a1-1]
#undef rate_1
#define rate_1(a1) rate[a1-1]
#undef it_1
#define it_1(a1) it[a1-1]

  static REAL ratio[nd];
  static char  *it[nt];
  static REAL fuzz, hm, gm, am;
  static long m, j, i, insert, k;
  static char *ijk;
  /*-----end-of-declarations-----*/

  trace ("spedup  ");
  /*                           Rank computer NAME by its Geometric Mean.*/
  for (k=1 ; k<=nsys ; k++) {
    if( rate_1(2) > rated_2(k,2))  break;
  }
  insert= k;
  /*                            Print Table of Speed-ups of Mean Rates.*/
  page( iou);
  it_1(1)= "AM=";
  it_1(2)= "GM=";
  it_1(3)= "HM=";
  ijk  = "-------- ";
  fuzz = 1.0e-9;
  fprintf(iou,"\n\n");
  fprintf(iou,"                         TABLE OF SPEED-UP RATIOS OF MEAN RATES (72 Samples)\n");
  fprintf(iou,"                         (compared to FORTRAN results)\n");
  fprintf(iou,"                         Arithmetic, Geometric, Harmonic Means (AM,GM,HM)\n");
  fprintf(iou,"                         The Geometric Mean is the least biased statistic.\n\n");
  fprintf(iou,"--------  ----  ------   ");
  for(m=1;  m <= ns; m++) {
    fprintf(iou,"%s", ijk );
  }
  fprintf(iou,"\n");
  fprintf(iou,"SYSTEM    MEAN  MFLOPS   ");
  for(m=1;  m <= ns; m++) {
    if(m < insert) {
      fprintf(iou,"%.8s ", names_2(m,2));
    } else if(m == insert) {
      fprintf(iou,"%.8s ", name_1(2));
    } else {
      fprintf(iou,"%.8s ", names_2(m-1,2));
    }
  }
  fprintf(iou,"\n");
  fprintf(iou,"--------  ----  ------   ");
  for(m=1;  m <= ns; m++) {
    fprintf(iou,"%s", ijk );
  }
  fprintf(iou,"\n");
  for (i=1 ; i<insert ; i++) {
    for (j=1 ; j<=nt-1 ; j++) {
      for (m=1 ; m<=ns ; m++) {
        if(m == insert) {
          ratio_1(m)= rated_2(i,j) / (rate_1(j) + fuzz);
        } else if(m > insert) {
          ratio_1(m)= rated_2(i,j) / (rated_2(m-1,j) + fuzz);
		} else {
          ratio_1(m)= rated_2(i,j) / (rated_2(m,j) + fuzz);
		}
      }
      fprintf(iou,"%.8s  %3s%8.3f : ",
	      names_2(i,j), it_1(j), rated_2(i,j));
      for(m=1 ; m<=ns ; m++) {
        fprintf(iou, "%9.3f", ratio_1(m));
      }
      fprintf(iou,"\n");
    }
    fprintf(iou,"          SD=%8.3f\n",  rated_2(i,4));
    fprintf(iou,"\n");
  }

  i= insert;
  for (j=1 ; j<=nt-1 ; j++) {
    for (m=1 ; m<=ns ; m++) {
      if(m == insert) {
        ratio_1(m)= rate_1(j) / (rate_1(j) + fuzz);
      } else if(m > insert) {
        ratio_1(m)= rate_1(j) / (rated_2(m-1,j) + fuzz);
      } else {
        ratio_1(m)= rate_1(j) / (rated_2(m,j) + fuzz);
      }
    }
    fprintf(iou,"%.8s  %3s%8.3f : ",
	    name_1(j), it_1(j), rate_1(j));
    for(m=1 ; m<=ns ; m++) {
      fprintf(iou, "%9.3f", ratio_1(m));
    }
    fprintf(iou,"\n");
  }
  fprintf(iou,"          SD=%8.3f\n",  rate_1(4));

  for (i=insert+1 ; i<=ns ; i++) {
    fprintf(iou,"\n");
    for (j=1 ; j<=nt-1 ; j++) {
      for (m=1 ; m<=ns ; m++) {
	if(m == insert) {
	  ratio_1(m)= rated_2(i-1,j) / (rate_1(j) + fuzz);
	} else if(m > insert) {
	  ratio_1(m)= rated_2(i-1,j) / (rated_2(m-1,j) + fuzz);
	} else {
	  ratio_1(m)= rated_2(i-1,j) / (rated_2(m,j) + fuzz);
	}
      }
      fprintf(iou,"%.8s  %3s%8.3f : ",
	      names_2(i-1,j), it_1(j), rated_2(i-1,j));
      for(m=1 ; m<=ns ; m++) {
	fprintf(iou, "%9.3f", ratio_1(m));
      }
      fprintf(iou,"\n");
    }
    fprintf(iou,"          SD=%8.3f\n",  rated_2(i-1,4));
  }

  track ("spedup  ");
  return;
#undef SET_VARS
#include "c_tags.h"
#include "c_rats.h"
}


void  tally( FILE *iou, long mode )
{
  /************************************************************************
                                                                          *
        TALLY      computes average and minimum Cpu timings and variances.*
                                                                          *
                   iou -  i/o unit number                                 *
                                                                          *
                  mode -  = 1 selects average run time: Preferred mode.   *
                          = 2 selects minimum run time: Less accurate mode*
                                                                          *
    **********************************************************************/

#define SET_VARS
#include "c_cernel.h"

/* undefine unused macros */
#undef m

#undef s1_1
#define s1_1(a1) s1[a1-1]
#undef s2_1
#define s2_1(a1) s2[a1-1]
#undef s3_1
#define s3_1(a1) s3[a1-1]
#undef s4_1
#define s4_1(a1) s4[a1-1]
#undef t1_1
#define t1_1(a1) t1[a1-1]
#undef t4_1
#define t4_1(a1) t4[a1-1]

  static double    cs;  /* double even for a float run */
  static long k4, npft, i, k, j, m;
  static REAL  s1[20], s2[20], s3[20], s4[20];
  static REAL  t1[47], t4[47];

  trace ("tally   ");

  sizes(-1);

  m= 1;
  if( mode == 2 )  m= 3;
  if(full_out) {
    page(iou);
    fprintf(iou, "\n\ntime TEST overhead (t err):  \n");
    fprintf(iou,"\n     RUN        AVERAGE        STANDEV        MINIMUM        MAXIMUM \n");
  }
  /*                       Checks valid domain for min and max of data sets*/
  for (j=1 ; j<=nruns ; j++) {
	if(full_out) {
      fprintf(iou,"TICK %3ld%15.6e%15.6e\n",
	    j, see_4(1,1,j,il), see_4(2,1,j,il) );
	}
    t1_1(j)= see_4(1,1,j,il);
    i= 0;
    if( (see_4(3,2,j,il)< 0.01) || (see_4(4,2,j,il)> 1.0))  i= i+1;
    if( (see_4(3,3,j,il)< 0.01) || (see_4(4,3,j,il)> 1.0))  i= i+1;
    if( i>0 )  {
      fprintf(iou,"**  TALLY: ERROR INVALID DATA** %6ld%6ld\n", j, il);
    }
    if( ( j==nruns ) || ( i>0 ))  {
	  if(full_out) {
        fprintf(iou,"DATA %3ld%15.6e%15.6e%15.6e%15.6e\n", j,
	      see_4(1,2,j,il), see_4(2,2,j,il), see_4(3,2,j,il), see_4(4,2,j,il) );
        fprintf(iou,"DATA %3ld%15.6e%15.6e%15.6e%15.6e\n", j,
	      see_4(1,3,j,il), see_4(2,3,j,il), see_4(3,3,j,il), see_4(4,3,j,il) );
	  }
	}
  }

  stats( s1, t1, nruns);
  if(full_out) {
    fprintf(iou,"TICK %3ld%15.6e%15.6e%15.6e%15.6e\n",  nruns, s1_1(1),
	  s1_1(2), s1_1(3), s1_1(4) );

    fprintf(iou,"\n\nTHE EXPERIMENTAL TIMING ERRORS FOR ALL%3d RUNS\n", nruns);
    fprintf(iou,"--  ---------  ---------  --------- -----  -----   ---\n");
    fprintf(iou," k   T min      T avg      T max    T err   tick   P-F\n");
    fprintf(iou,"--  ---------  ---------  --------- -----  -----   ---\n");
  }
  /* Computes and Checks experimental timing errors*/
  for (k=1 ; k<=mk ; k++) {
    npft= 0;
    cs= 0.0e0;

    for (j=1 ; j<=nruns ; j++) {
      npft += npfs_3(j,il,k);
      cs += csums_3(j,il,k);
    }

    stats( s2, &(times_3(1,il,k)), nruns);
    time_1(k)= s2_1(m);
    csum_1(k)= cs;
    terr1_1(k)= 100.0e0*( s2_1(2)/( s2_1(1) + 1.0e-9));
    t4_1(k)= terr1_1(k);

    /*
       If this clock resolution test fails, you must increase Loop (Subr. SIZES)
     */
    stats( s3, &(terrs_3(1,il,k)), nruns);
    if( s3_1(1) > 15.0)  {
      fprintf(iou,"\n%2ld POOR CPU CLOCK RESOLUTION; NEED LONGER RUN. \n", k);
    }
    if(full_out) {
      fprintf(iou,"%2ld%11.3e%11.3e%11.3e%6.2f%%%6.2f%%%5ld\n", k, s2_1(3), s2_1(1), s2_1(4), terr1_1(k), s3_1(1), npft);
    }
	terr1_1(k)= max( terr1_1(k), s3_1(1));
    stats( s1, &(dos_3(1,il,k)), nruns);
    total_1(k)= s1_1(1);
    if( (s1_1(1)<=0.0e0) || (abs(s1_1(3)-s1_1(4))>1.0e-5)) {
      fprintf(iou,"**  TALLY: ERROR INVALID DATA** %6ld%6ld%14.6e%14.6e%14.6e%14.6e\n",
	      il, k, s1_1(1), s1_1(2), s1_1(3), s1_1(4) );
    }
    stats( s4, &(fopn_3(1,il,k)), nruns);
    flopn_1(k)= s4_1(1);
    if( (s4_1(1)<=0.0e0) || (abs(s4_1(3)-s4_1(4))>1.0e-5)) {
      fprintf(iou,"**  TALLY: ERROR INVALID DATA** %6ld%6ld%14.6e%14.6e%14.6e%14.6e\n",
	      il, k, s4_1(1), s4_1(2), s4_1(3), s4_1(4) );
    }
  }
  if(full_out) {
    fprintf(iou,"--  ---------  ---------  --------- -----  -----   ---\n");
  }
  stats( s4, t4, mk);
  printf("\n\nNET CPU TIMING VARIANCE (T err);  A few %%is ok: \n");
  printf("\n                AVERAGE        STANDEV        MINIMUM        MAXIMUM \n");
  printf("    Terr%14.2f%%%14.2f%%%14.2f%%%14.2f%%\n",
	   s4_1(1), s4_1(2), s4_1(3), s4_1(4) );
  fprintf(iou,"\n\nNET CPU TIMING VARIANCE (T err);  A few %%is ok: \n");
  fprintf(iou,"\n                AVERAGE        STANDEV        MINIMUM        MAXIMUM \n");
  fprintf(iou,"    Terr%14.2f%%%14.2f%%%14.2f%%%14.2f%%\n",
	   s4_1(1), s4_1(2), s4_1(3), s4_1(4) );

  track ("tally   ");
  return;
#undef SET_VARS
#include "c_cernel.h"
}

void s_trim(char *str)
{
  long len;

  len= strlen(str);
  if(len < 2) {
    puts("string in file input is too short\n");
    exit(1);
  }
  if(str[len-1] == '\n' || str[len-1] == '\r') str[len-1]= '\0';
  if(str[len-2] == '\n' || str[len-2] == '\r') str[len-2]= '\0';
}

void set_system(void)
{
#define SET_VARS
#include "c_sysid.h"
  time_t tim_now;
  FILE *fil;
#define MAX_SZ 80

  /* Set the computer and compiler versions and other info 
     that will help identify the output in the future.
     There is room in the output file for about 40 characters
     per string. 
     These values are read from a file.
  */
  fil= fopen("input", "r");
  if(!fil) {
    puts("Unable to open the file named input\n");
    exit(1);
  }
  komput= malloc(MAX_SZ);
  fgets(komput, MAX_SZ, fil);
  s_trim(komput);
  kontrl= malloc(MAX_SZ);
  fgets(kontrl, MAX_SZ, fil);
  s_trim(kontrl);
  kompil= malloc(MAX_SZ);
  fgets(kompil, MAX_SZ, fil);
  s_trim(kompil);
  koutfil= malloc(MAX_SZ);
  fgets(koutfil, MAX_SZ, fil);
  s_trim(koutfil);
  time(&tim_now);
  kalend  =  ctime(&tim_now);
  fclose(fil);
#undef SET_VARS
#include "c_sysid.h"
}

/*  ************************************************

      SECOND= Cumulative CPU time for job in seconds.  MKS unit is seconds.
              Clock resolution should be less than 2% of Kernel 11 run-time.
              ONLY CPU time should be measured, NO system or I/O time included.
              In VM systems, page-fault time must be avoided (Direction 8).
              SECOND accuracy may be tested by calling: CALIBR test.
    
      Defing CLOCK_FUNCTION and/or BSD_TIMER on the compile line will
      often produce a working timer routine. If not, you will need to
      write your own. In addition, you should consider modifying 
      the timer routine if your system has a higher
      resolution clock. The run time for the loops is shorter
      when using a high resolution clock.
    
      Timing routines with resolutions of order 1/60 second are typical on 
      PCs and workstations, and require the use
      of multiple-pass loops around each kernel to make the run time
      at least 50 times the tick-period of the timing routine.
      Function SECOVT measures the overhead time for a call to SECOND.
    
      An independent calibration of the running time may be wise.
      Compare the Total Job Cpu Time printout at end of the LFK output file
      with the job Cpu time charged by your operating system, if available.
      The output written during the run provides a chance to check the
      timing with a stopwatch.
    
                USE THE HIGHEST RESOLUTION CPU-TIMER FUNCTION AVAILABLE
    ***********************************************************************
*/

#ifdef __MWERKS__
#define MAC_CLOCK
#define CLOCK_FUNCTION get_time
#define TICKS_PER_SEC 1.0
#include <time.h>
#include <Timer.h>
extern double get_time(void);
#endif

#ifdef _WIN32
#include <time.h>
#include <windows.h>
extern double get_time(void);
#define CLOCK_FUNCTION get_time
#define TICKS_PER_SEC 1.0
#endif

#ifdef ANSI_TIMER
#define CLOCK_FUNCTION clock
#endif

#ifndef CLOCK_FUNCTION
#ifdef BSD_TIMER
/* This will work on our Suns -- other BSD machines may differ?
    sys/time.h includes time.h on Sun */
#include <sys/time.h>
#include <sys/resource.h>
double second(double oldsec)
{
  double cpu, sys;
  struct rusage cpuTime;

  getrusage(RUSAGE_SELF, &cpuTime);
  cpu= cpuTime.ru_utime.tv_sec + 1.0e-6*cpuTime.ru_utime.tv_usec;
  sys= cpuTime.ru_stime.tv_sec + 1.0e-6*cpuTime.ru_stime.tv_usec;
  return cpu+sys-oldsec;
}

#else        /* Not BSD_TIMER */
/* Assume POSIX 1003.1-1990 standard timing interface.
   However-- CLK_TCK is noted as "obsolescent" there...  */
#include <time.h>
#include <sys/times.h>
/* Try to handle modest deviations from POSIX standard (e.g.- Sun).  */
#ifndef CLK_TCK
#include <unistd.h>
#ifndef CLK_TCK
#define CLK_TCK sysconf(_SC_CLK_TCK)
#endif
#endif
double second(double oldsec)
{
  static double ticksPerSecond= 0.0;
  struct tms cpuTime;
  long wallTicks= times(&cpuTime);
  double cpu, sys;

  if (ticksPerSecond==0.0) ticksPerSecond= CLK_TCK;
  cpu= cpuTime.tms_utime/(double)ticksPerSecond;
  sys= cpuTime.tms_stime/(double)ticksPerSecond;
  return cpu+sys-oldsec;
}

#endif    /* end of non BSD_TIMER branch */
#else     /* Branch where CLOCK_FUNCTION is defined */
/* Define CLOCK_FUNCTION then modify the following version to suit
   your architecture.  See sysdep.h for other suggestions.
 */
#include <time.h>
double second(double oldsec)
{
  return ((double)CLOCK_FUNCTION()) / ((double)CLOCKS_PER_SEC) - oldsec;
}
#endif

#ifdef MAC_CLOCK
double get_time(void)
{
  /* WARNING: Mac doesn't distinguish between system and user time
     (so count all time as user time) and it is totally wrong 
     if MultiFinder makes a switch while the program is being timed. 
  */
  double tim;
  UnsignedWide microTickCount;

  Microseconds(&microTickCount);
  tim= 1.0e-6*microTickCount.lo+1.0e-6*1024.0*1024.0*4096.0*microTickCount.hi;
  return tim;
}
#endif  /* MAC_CLOCK */

#ifdef _WIN32
double get_time(void)
{
	/* NOTE: PC doesn't distinguish between system and user time
	   (so count all time as user time).
	*/
    /* WARNING: Use low resolution timer on a multi-processor Intel computer. */
#ifdef USE_FANCY_CLOCK
	unsigned _int64 time64, ticksPerSecond;

	QueryPerformanceCounter( (LARGE_INTEGER *)&time64);
	QueryPerformanceFrequency( (LARGE_INTEGER *)&ticksPerSecond);
	return ((double)(_int64)time64)/(_int64)ticksPerSecond;
#else
	/* The GetTickCount function seems to have the same 1/60 second
	   resolution as clock(), so don't bother with it. The
	   timeGetTime() function from the multi-media library 
	   sounds like it has better resolution, but it isn't in any 
	   of the standard libraries. Someday it might be worth hunting
	   down the proper library to link in and trying it.
	*/
	static double clock_rate= -1.0;
	if(clock_rate <= 0.0) clock_rate= 1.0/CLOCKS_PER_SEC;
	return (double)clock()*clock_rate;
#endif
}
#endif   /* _WIN32 */

