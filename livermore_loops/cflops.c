/* This file contains driver routines for the ANSI C version of 
   the Livermore Loops.

   This program is based on the Livermore Fortran Kernels
   written by Frank McMahon.

   The mflops driver program was translated from Fortran to ANSI C by 
   Steven Langer. The only changes made to the C version of the 
   kernel file provided by McMahon were to move the macros 
   for common block access into a separate header file
   and to remove the section that dealt with underscores
   appended to common block names by Fortran.

   The macros permit Fortran style indexing in C.
   A much more idiomatic version of this program could certainly 
   be created, but the reasonably close correspondence to the Fortran 
   source code is sometimes useful.

   The Fortran version stores many variables in COMMON blocks.
   The C version stores the same variables in global data structures.
   The "common block" header files are set up so that they can be included
   either to declare or instantiate the variables.

   A C compiler, as of 1998, often produces faster code when accessing 
   global data than when accessing data through a pointer. A side effect 
   of using global structures is that the performance seen in this
   benchmark may be higher than that of many real world codes that use
   pointers heavily. A compiler flag declaring that there is no
   "pointer aliasing" in a program may get around this performance loss. 

   The only changes that should be needed before running the
   program are to modify the Makefile (samples are provided for
   many popular unix systems) and to change the system name etc.
   in cmfsub.c. Part of modifying the Makefile is to set the 
   symbols that control the choice of timer routine. In some cases,
   it many be necessary to write a timer routine, if the ones used in
   mfsub.c are not available or a more accurate routine is provided
   on the system.

   Steven Langer
   Lawrence Livermore National Laboratory
   shl@icf.llnl.gov
   March 16, 1995
 */
#include <math.h>
#include "cflopio.h"
#include "cflops.h"
#include "block.h"

/* define FAST_RUN to force multi to be 1. 
   undef it to get a run with good timing accuracy.
   define can be in Makefile. */
/*
#define FAST_RUN
#undef FAST_RUN
*/


/*-----Fortran intrinsics converted-----*/
#define abs(x) ((x)>=0?(x):-(x))
#define mod(x,y) ((x)%(y))
#define aint(x) ((double)((long)(x)))
#define sign(x,y) ((((x)<0)!=((y)<0))?-(x):(x))
#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
/*-----end of Fortran intrinsics-----*/

/* C equivalents of COMMON blocks to communicate with the kernels.
   Set flags so as to get all common blocks in the file.
   Note that this invocation creates storage for
   the common blocks.
*/
#define ALLOC_COMMON
#define SET_VARS

#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"
#include "c_order.h"
#include "c_sysid.h"
#include "c_debug.h"
#include "c_iqrand.h"
#include "c_fake1.h"
#include "c_fake2.h"
#include "c_cksums.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"
#include "c_space3.h"
#include "c_rats.h"
#include "c_tags.h"

/* remove the macros so local variables of the same name may be used */
#undef ALLOC_COMMON
#undef SET_VARS

#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"
#include "c_order.h"
#include "c_sysid.h"
#include "c_debug.h"
#include "c_iqrand.h"
#include "c_fake1.h"
#include "c_fake2.h"
#include "c_cksums.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"
#include "c_space3.h"
#include "c_rats.h"
#include "c_tags.h"


int main(int argc, char *argv[])
{
#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_order.h"
#include "c_sysid.h"

#undef n_times
#define n_times 18

#undef ck_1
#define ck_1(a1) ck[a1-1]
#undef rates_1
#define rates_1(a1) rates[a1-1]
#undef tr_1
#define tr_1(a1) tr[a1-1]
#undef id_1
#define id_1(a1) id[a1-1]
#undef lspan_1
#define lspan_1(a1) lspan[a1-1]
#undef tk_1
#define tk_1(a1) tk[a1-1]
#undef terr_1
#define terr_1(a1) terr[a1-1]
#undef osum_1
#define osum_1(a1) osum[a1-1]
#undef wg_1
#define wg_1(a1) wg[a1-1]

  REAL  flops[141], tr[141], rates[141];
  long  id[141];
  long  lspan[141];
  REAL  wg[141],  osum [141],  terr[141],  tk[6];
  REAL totjob, tock, tj, ti;
  long j, i, k, nt;
  FILE *iou;
  char *fmt9;
  /*-----end-of-declarations-----*/

  /*                        Job start Cpu time*/
  cumtim_1(1)= 0.0e0;
  ti= second( cumtim_1(1));
  /*                                Define your computer system:*/
  set_system();
  /*                    Initialize variables and Open Files*/
  indata( tk, &iou);
  /*                    Record name in active linkage chain in COMMON /DEBUG/  */
  trace (" main.  ");
  /*                    Verify Sufficient Loop Size Versus Cpu Clock Accuracy*/
  verify( iou );
  tj= second( cumtim_1(1));
  nt= n_times;
  /*                        Define control limits:  Nruns(runs), Loop(time)*/
  sizes(-1);
  /*
                            Run test Mruns times Cpu-limited; I/O is deferred:
  */
  for (k=1 ; k<=mruns ; k++) {
    i= k;
    jr= mod( i-1,7) + 1;
    iqran0( 256);
    /*                Run test using one of 3 sets of DO-Loop spans:
                      Set iou Negative to supress all I/O during Cpu timing.*/
    for (j=im ; j<=ml ; j++) {
      il= j;
      tock= tick( iou, -1, nt);
      kernel( tk);
    }
    trial( iou, i, ti, tj);
  }
  /*
                            Report timing errors, Mflops statistics:*/
  for (j=im ; j<=ml ; j++) {
    il= j;
    result( iou,flops,tr,rates,lspan,wg,osum,terr,id);
    /*
              Report  Mflops for Vector Cpus( short, medium, long vectors):
     */
    iovec= 0;
    if( iovec==1 )  {
      report( iou,IOU_OK, mk,mk,flops,tr,rates,lspan,wg,osum,id);
    }
  }
  /*                Report  Mflops SUMMARY Statistics: for Official Quotations
    */
  report( iou,IOU_OK,3*mk,mk,flops,tr,rates,lspan,wg,osum,id);

  cumtim_1(1)= 0.0e0;
  totjob= second( cumtim_1(1)) - ti - tsecov;
  fmt9= "\n\nVersion: 22/DEC/86  mf522   %12ld\n CHECK FOR CLOCK CALIBRATION ONLY: \n Total Job    Cpu Time =  %14.5e Sec.\n"
    " Total 24 Kernels Time =  %14.5e Sec.\n Total 24 Kernels Flops=  %14.5e Flops\n";
  fprintf(iou, fmt9,  inseq, totjob, tk_1(1), tk_1(2));
  printf(fmt9,  inseq, totjob, tk_1(1), tk_1(2));
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_order.h"
#include "c_sysid.h"
  return 0;
}


/************************************************

Index and cross reference for subroutines and common blocks


           MODULE     PURPOSE
           ------     -----------------------------------------------

           CALIBR     cpu clock calibration tests accuracy of SECOND function

           INDATA     initialize variables

           IQRANF     computes a vector of pseudo-random indices
           IQRAN0     define seed for new IQRANF sequence

           KERNEL     executes 24 samples of Fortran computation

           PFM        optional call to system hardware performance monitor

           RELERR     relative error between  u,v  (0.,1.)

           REPORT     prints timing results

           RESULT     computes execution rates  into pushdown store

           SECOND     cumulative CPU time for task in seconds (M.K.S. units)

           SECOVT     measures the Overhead time for calling   SECOND

           SENSIT     sensitivity analysis of harmonic mean to 49 workloads

           SEQDIG     computes nr significant, equal digits in pairs of numbers

           SIGNEL     generates a set of floating-point numbers near 1.0

           SIMD       sensitivity analysis of harmonic mean to SISD/SIMD model

           SIZES      test and set the loop controls before each kernel test

           SORDID     simple sort

           SPACE      sets memory pointers for array variables.  optional.

           SPEDUP     computes Speed-ups: A circumspect method of comparison.

           STATS      calculates unweighted statistics

           STATW      calculates   weighted statistics

           SUMO       check-sum with ordinal dependency

           SUPPLY     initializes common blocks containing type real arrays.

           TALLY      computes average and minimum Cpu timings and variances.

           TDIGIT     counts lead digits followed by trailing zeroes

           TEST       Repeats and times the execution of each kernel

           TESTS      Checksums and initializes the data for each kernel test

           TICK       measures timing overhead of subroutine test

           TILE       computes  m-tile value and corresponding index

           TRACE ,TRACK    push/pop caller"s name and serial nr. in /DEBUG/

           TRAP       checks that index-list values are in valid domain

           TRIAL      validates checksums of current run for endurance trial

           VALID      compresses valid timing results

           VALUES     initializes special values

           VERIFY     verifies sufficient Loop size versus cpu clock accuracy

           WATCH      can continually test COMMON variables and localize bugs

      ------------ -------- -------- -------- -------- -------- --------
      ENTRY LEVELS:   1        2        3        4        5        6
      ------------ -------- -------- -------- -------- -------- --------
                   MAIN.    SECOND
                            INDATA
                            VERIFY   SECOND
                                     SIZES    IQRAN0
                                     STATS    SQRT
                                     TDIGIT   LOG10
                            SIZES    IQRAN0

                            TICK     TEST     TESTS    SECOND
                                                       SIZES
                                                       SUMO
                                                       VALUES   SUPPLY   SIGNEL
                                                                IQRANF   MOD
                                              SECOND
                                     VALID    TRAP              TRAP
                                     STATS    SQRT
                                     IQRANF   MOD
                                              TRAP
                            KERNEL   SPACE
                                     SQRT
                                     EXP
                                     TEST     TESTS    SECOND
                                                       SIZES
                                                       SUMO
                                                       VALUES   SUPPLY   SIGNEL
                                                                IQRANF   MOD
                                              SECOND
                            TRIAL    SEQDIG   LOG10    TDIGIT
                                     IQRAN0

                            RESULT   TALLY    SIZES    IQRAN0   TRAP
                                              PAGE
                                              STATS    SQRT

                                     SEQDIG   LOG10    TDIGIT

                            REPORT   VALID    TRAP
                                     MOD
                                     STATW    SORDID   TRAP
                                              TILE
                                              SQRT
                                              LOG10
                                     PAGE
                                     TRAP
                                     SENSIT   VALID    TRAP
                                              SORDID   TRAP
                                              PAGE
                                              STATW    SORDID   TRAP
                                                       TILE
                                     SIMD     VALID    TRAP
                                              STATW    SORDID   TRAP
                                                       TILE
                                     SPEDUP
                            STOP


      All subroutines also call TRACE , TRACK , and WATCH to assist debugging.




        ------ ---- ------     -----   ------------------------------------
        BASE   TYPE CLASS      NAME    GLOSSARY
        ------ ---- ------     -----   ------------------------------------
        SPACE0    R Array      BIAS  - scale factors for SIGNEL data generator
        SPACE0    R Array      CSUM  - checksums of KERNEL result arrays
        BETA      R Array      CSUMS - sets of CSUM for all test runs
        BETA      R Array      DOS   - sets of TOTAL flops for all test runs
        SPACE0    R Array      FLOPN - flop counts for one execution pass
        BETA      R Array      FOPN  - sets of FLOPN for all test runs
        SPACE0    R Array      FR    - vectorisation fractions; abscissa for REPORT
        SPACES    I scalar     ibuf  - flag enables one call to SIGNEL
        ALPHA     I scalar     ik    - current number of executing kernel
        ALPHA     I scalar     il    - selects one of three sets of loop spans
        SPACES    I scalar     ion   - logical I/O unit number for output
        SPACEI    I Array      IPASS - Loop control limits for multiple-pass loops
        SPACE0    I Array      IQ    - set of workload weights for REPORT
        SPACEI    I Array      ISPAN - loop control limits for each kernel
        SPACES    I scalar     j5    - datum in kernel 16
        ALPHA     I scalar     jr    - current test run number (1 thru 7)
        SPACES    I scalar     k2    - counter in kernel 16
        SPACES    I scalar     k3    - counter in kernel 16
        SPACES    I scalar     kr    - a copy of mk
        SPACES    I scalar     laps  - multiplies Nruns for long Endurance test
        SPACES    I scalar     Loop  - current multiple-pass loop limit in KERNEL
        SPACES    I scalar     m     - temp integer datum
        ALPHA     I scalar     mk    - number of kernels to evaluate .LE.24
        ALPHA     I scalar     ml    - maximum value of il=  3
        SPACES    I scalar     mpy   - repetiton counter of MULTI pass loop
        SPACES    I scalar     Loops2- repetiton loop limit
        ALPHA     I scalar     Mruns - number of complete test runs .GE.Nruns
        SPACEI    I Array      MULT  - multipliers * IPASS defines Loop
        SPACES    I scalar     MULTI - Multiplier used to compute Loop in SIZES
        SPACES    I scalar     n     - current DO loop limit in KERNEL
        SPACES    I scalar     n1    - dimension of most 1-D arrays
        SPACES    I scalar     n13   - dimension used in kernel 13
        SPACES    I scalar     n13h  - dimension used in kernel 13
        SPACES    I scalar     n14   - dimension used in kernel 14
        SPACES    I scalar     n16   - dimension used in kernel 16
        SPACES    I scalar     n2    - dimension of most 2-D arrays
        SPACES    I scalar     n21   - dimension used in kernel 21
        SPACES    I scalar     n213  - dimension used in kernel 21
        SPACES    I scalar     n416  - dimension used in kernel 16
        SPACES    I scalar     n813  - dimension used in kernel 13
        SPACE0    I scalar     npf   - temp integer datum
        ALPHA     I Array      NPFS  - sets of NPFS1 for all test runs
        SPACE0    I Array      NPFS1 - number of page-faults for each kernel
        ALPHA     I scalar     Nruns - number of complete test runs .LE.7
        SPACES    I scalar     nt1   - total size of common -SPACE1- words
        SPACES    I scalar     nt2   - total size of common -SPACE2- words
        BETA      R Array      SEE   - (i,1,jr,il) sets of TEST overhead times
        BETA      R Array      SEE   - (i,2,jr,il) sets of csums of SPACE1
        BETA      R Array      SEE   - (i,3,jr,il) sets of csums of SPACE2
        SPACE0    R Array      SKALE - scale factors for SIGNEL data generator
        SPACE0    R scalar     start - temp start time of each kernel
        PROOF     R Array      SUMS  - sets of verified checksums for all test runs
        SPACE0    R Array      SUMW  - set of quartile weights for REPORT
        TAU       R scalar     tclock- minimum cpu clock time= resolution
        SPACE0    R Array      TERR1 - overhead-time errors for each kernel
        BETA      R Array      TERRS - sets of TERR1 for all runs
        TAU       R scalar     testov- average overhead time in TEST linkage
        BETA      R scalar     tic   - average overhead time in SECOND (copy)
        SPACE0    R scalar     ticks - average overhead time in TEST linkage(copy)
        SPACE0    R Array      TIME  - net execution times for all kernels
        BETA      R Array      TIMES - sets of TIME for all test runs
        SPACE0    R Array      TOTAL - total flops computed by each kernel
        TAU       R scalar     tsecov- average overhead time in SECOND
        SPACE0    R Array      WS    - unused
        SPACE0    R Array      WT    - weights for each kernel sample
        SPACEI    R Array      WTP   - weights for the 3 span-varying passes
        SPACE0    R Array      WW    - unused


      --------- -------------------------------------------------------------
       COMMON   Usage
      --------- -------------------------------------------------------------

       /ALPHA /
                VERIFY    TICK      TALLY     SIZES     RESULT    REPORT    KERNEL
                MAIN.
       /BASE1 /
                SUPPLY
       /BASE2 /
                SUPPLY
       /BASER /
                SUPPLY
       /BETA  /
                TICK      TALLY     SIZES     RESULT    REPORT    KERNEL
       /DEBUG /
                TRACE     TRACK     TRAP
       /ORDER /
                TRACE     TRACK     TRAP
       /PROOF /
                RESULT    BLOCKDATA
       /SPACE0/
                VALUES    TICK      TEST      TALLY     SUPPLY    SIZES     RESULT
                REPORT    KERNEL    BLOCKDATA
       /SPACE1/
                VERIFY    VALUES    TICK      TEST      SUPPLY    SPACE     KERNEL
       /SPACE2/
                VERIFY    VALUES    TICK      TEST      SUPPLY    SPACE     KERNEL
       /SPACE3/
                VALUES
       /SPACEI/
                VERIFY    VALUES    TICK      TEST      SIZES     RESULT    REPORT
                KERNEL    BLOCKDATA
       /SPACER/
                VALUES    TICK      TEST      SUPPLY    SIZES     KERNEL
       /SPACES/
                VERIFY    VALUES    TICK      TEST      SUPPLY    SIZES     KERNEL
                BLOCKDATA
      --------- -------------------------------------------------------------


               Subroutine Timing on CRAY-XMP1:

               Subroutine   Time(%) All Scalar

               KERNEL       52.24%
               SUPPLY       17.85%
               VERIFY        8.76%
               VALUES        6.15%
               STATS         5.44%
               DMPY          1.97%
               DADD          1.53%
               EXP           1.02%
               SQRT           .99%
               SORDID         .81%
               DDIV           .38%
               IQRANF         .25%
               SUMO           .22%
               TRACE          .19%
               SIGNEL         .16%
               TRAP           .10%
               TRACK          .10%
               STATW          .08%
               TILE           .04%
               SIZES          .03%
               LOG10         .03%

               Subroutine   Time(%)  Auto Vector

               KERNEL       56.28%
               VALUES       10.33%
               STATS         8.57%
               DADD          4.34%
               DMPY          3.86%
               VERIFY        2.61%
               SUPPLY        2.28%
               SQRT          2.10%
               SORDID        1.84%
               SUMO           .80%
               DDIV           .78%
               SDOT           .67%
               TRACE          .53%
               IQRANF         .50%
               SIGNEL         .36%
               EXP            .32%
               TRACK          .23%
               TRAP           .20%
               LOG10         .18%
               STATW          .16%
    */



void iqran0( long newk)
{
  /* ***************************************
         IQRAN0  - define seed for new IQRANF sequence
   */
#define SET_VARS
#include "c_iqrand.h"

  trace ("iqran0  ");

  if( newk<=0 ) {
    where(1);
  }
  k = newk;

  track ("iqran0  ");
#undef SET_VARS
#include "c_iqrand.h"
}


void iqranf( long mray[], long mmin,long mmax, long n)
{
  /************************************************************************
                                                                          *
         IQRANF  - computes a vector of psuedo-random indices             *
                   in the domain (Mmin,Mmax)                              *
                                                                          *
         Mray  - result array ,  psuedo-random positive integers          *
         Mmin  - input integer,  lower bound for random integers          *
         Mmax  - input integer,  upper bound for random integers          *
         n     - input integer,  number of results in M.                  *
                                                                          *
           Mray(i)= Mmin + INT( (Mmax-Mmin) * RANF(0))                    *
                                                                          *
            CALL IQRAN0( 256 )                                            *
            CALL IQRANF( IX, 1,1001, 30)      should produce in IX:       *
               3  674  435  415  389   54   44  790  900  282             *
             177  971  728  851  687  604  815  971  155  112             *
             877  814  779  192  619  894  544  404  496  505  ...        *
                                                                          *
         S.K.Park, K.W.Miller, Random Number Generators: Good Ones        *
         Are Hard To Find, Commun ACM, 31(10), 1192-1201 (1988).          *
    *********************************************************************
    */

#define SET_VARS
#include "c_iqrand.h"
  double dq, dp, per, dk, spin, span;
  long i, inset;
  double q, scale, realn;
  /*-----end-of-declarations-----*/

  trace ("iqranf  ");
  if( n > 0 ) {
    inset= mmin;
    span= mmax - mmin;
    spin= 16807;
    per= 2147483647;
    realn= n;
    scale= 1.0000100e0;
    q= scale*(span/realn);

    dk= k;
    for (i=1 ; i<=n ; i++) {
      dp= dk*spin;
      dk= dp - (int)( dp/per)*per;
      dq= dk*span;
      mray[i-1]= (long) (inset + ( dq/per));
      if(mray[i-1]<mmin || mray[i-1]>mmax) mray[i-1]= (long)(inset+i*q);
    }
    k= dk;

    trap( mray, " iqranf  ", 1, mmax, n);
  }
  track ("iqranf  ");
  return;

#undef SET_VARS
#include "c_iqrand.h"
}


void  page( FILE *iou)
{
  trace ("page    ");
  fprintf(iou, "\n\n\n\n\n");
  track ("page    ");
  return;
}


double relerr(double u, double v)
{
  /*
    ******************************************

           RELERR - RELATIVE ERROR BETWEEN  U,V  (0.,1.)
                U - INPUT
                V - INPUT
    ******************************************
    */

  double x, y, b, a, o, w;
  /*-----end-of-declarations-----*/
  trace ("relerr  ");
  w= 0.00e0;
  if( u != v ) {
    w= 1.00e0;
    o= 1.00e0;
    if( sign( o, u) == sign( o, v)) {
      a= abs( u);
      b= abs( v);
      x= max( a, b);
      y= min( a, b);
      if( x !=  0.00e0) {
	w= 1.00e0 - y/x;
      }
    }
  }

  track ("relerr  ");
  return w;
}


void result( FILE *iou,REAL flops[],REAL tr[],REAL rates[],
	    long lspan[],REAL wg[],REAL osum[],REAL terr[],long id[])
{
  /************************************************************************
                                                                          *
         RESULT -  Computes timing Results into pushdown store.           *
                                                                          *
          iou   -  Input   IO unit number for print output                *
         FLOPS  - Out.Ary  Number of Flops executed by each kernel        *
         TR     - Out.Ary  Time of execution of each kernel(microsecs)    *
         RATES  - Out.Ary  Rate of execution of each kernel(megaflops/sec)*
         LSPAN  - Out.Ary  Span of inner DO loop in each kernel           *
         WG     - Out.Ary  Weight assigned to each kernel for statistics  *
         OSUM   - Out.Ary  Checksums of the results of each kernel        *
         TERR   - Out.Ary  Experimental timing errors per kernel          *
         ID     - Out.Ary  Number of valid digits in checksum.            *
                                                                          *
    ***********************************************************************
    */

#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"


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
#undef terr_1
#define terr_1(a1) terr[a1-1]
#undef osum_1
#define osum_1(a1) osum[a1-1]
#undef wg_1
#define wg_1(a1) wg[a1-1]


  static double   cs;  /* cs is double, even for a float run */
  static REAL tmin;
  static long ijk, k, j, limit, isum;
  /*-----end-of-declarations-----*/
  trace ("result  ");

  tally( iou, 1 );
  /*                     Push Result Arrays Down before entering new result*/
  isum= 0;
  limit= 141 - mk;
  j = 141;
  for (k=limit ; k>=1 ; k+=-1) {
    flops_1(j)= flops_1(k);
    tr_1(j)=    tr_1(k);
    rates_1(j)= rates_1(k);
    lspan_1(j)= lspan_1(k);
    wg_1(j)=    wg_1(k);
    osum_1(j)=  osum_1(k);
    terr_1(j)=  terr_1(k);
    id_1(j)=    id_1(k);
    j--;
  }
  /*
                                 CALCULATE MFLOPS FOR EACH KERNEL
                              setting RATES_1(k)= 0. deletes kernel k from REPORT.*/
  tmin= 1.0e0*tsecov;
  for (k=1 ; k<=mk ; k++) {
    flops_1(k)= flopn_1(k)*total_1(k);
    tr_1(k)=  time_1(k) * 1.0e+6;
    rates_1(k)= 0.0e0;
    if( tr_1(k)!= 0.0e0)   rates_1(k)= flops_1(k)/tr_1(k);
    if( wt_1(k)<= 0.0e0)   rates_1(k)= 0.0e0;
    if( time_1(k)<tmin)   rates_1(k)= 0.0e0;
    if( time_1(k)<= 0.0e0) rates_1(k)= 0.0e0;
    lspan_1(k)= ispan_2(k,il);
    wg_1(k)= wt_1(k)*wtp_1(il);
    osum_1(k)= csum_1(k);
    terr_1(k)= terr1_1(k);
    /*
                       compute relative error and digits of precision in CSUM
      */

    ijk= 4;
    if( multi<=   1 )  ijk= 1;
    if( multi==  10 )  ijk= 2;
    if( multi==  50 )  ijk= 3;
    if( multi>= 100 )  ijk= 4;
    cs= (REAL) nruns * sums_3(k,il,ijk);
    terr1_1(k)= cs;
  }

  seqdig( id, &isum, terr1, csum, mk);

  track ("result  ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"
}


REAL secovt( FILE *iou, int iou_flag )
{
  /*      FUNCTION  SECOVT( iou )
    *********************************************************************
                                                                          *
         SECOVT  - Measures the Overhead time for calling SECOND
          toler  - tolerance for convergence= Relative error :  0.02
            iou  - I/O unit number
            iou_flag  - I/O unit number, negative if iou negative in fortran
    *********************************************************************
    */

#define SET_VARS
#include "c_fake1.h"
#include "c_fake2.h"

#undef tmx_1
#define tmx_1(a1) tmx[a1-1]
#undef ter_1
#define ter_1(a1) ter[a1-1]
#undef tim_1
#define tim_1(a1) tim[a1-1]
#undef inx_1
#define inx_1(a1) inx[a1-1]


  static REAL tim[20], ter[20], tmx[20];
  static long  inx[20];
  static REAL rerr, toler, elapst, tseco;
  static long k, i, j, jj, klm;
  /*-----end-of-declarations-----*/
  trace ("secovt  ");
  /*
    *********************************************************************
         Measure  tsecov:  Overhead time for calling SECOND
    *********************************************************************
    */
  tseco= 0.000e0;
  klm= 1600;
  jj= 0;

  for (j=1 ; j<=15 ; j++) {

    for (i=1 ; i<=10 ; i++) {
      t12_1(i)= 0.000e0;
    }
    tcum_1(1)= 0.000e0;
    t_0= second( tcum_1(1));
    /*                   assure that 10 calls to SECOND are NOT optimized*/
    for (k=1 ; k<=klm ; k++) {
      for (i=1 ; i<=10 ; i++) {
	tcum_1(i)= t12_1(i);
      }
      t1= second( tcum_1(1));
      t2= second( tcum_1(2));
      t3= second( tcum_1(3));
      t4= second( tcum_1(4));
      t5= second( tcum_1(5));
      t6= second( tcum_1(6));
      t7= second( tcum_1(7));
      t8= second( tcum_1(8));
      t9= second( tcum_1(9));
      t10= second( tcum_1(10));
    }
    elapst= t10 - t_0;
    tseco= elapst/( 10.0*klm + 1.0e-9);
    toler= 0.020e0;
    rerr= 1.000e0;
    /*
                                   Convergence test:  Rel.error .LT. 1%*/
    if( elapst> 1.00e04 ) goto L_911;
    if( elapst< 1.00e-10 && j>10 ) goto L_911;
    if( elapst> 1.00e-9 ) {
      jj= jj + 1;
      tim_1(jj)= tseco;
      if( jj>1 ) {
	rerr= relerr( tim_1(jj), tim_1(jj-1));
      }
      ter_1(jj)= rerr;
    }

    if( iou_flag ) {
      if(full_out) {
        fprintf( iou,"SECOVT:%10ld%12.4e%11.4f\n", 10*klm,  tseco, rerr);
	  }
    }
    if( rerr  < toler   ) goto L_825;
    if( elapst> 10.00e0 ) goto L_822;
    klm= klm + klm;
  }
  /*                                  Poor accuracy on exit from loop*/
 L_822:     if( j <= 1 )  goto L_911;
  if( jj< 1 )  goto L_911;
  sordid( inx, tmx,  ter, jj, 1);

  i= 0;
 L_823:      i= i + 1;
  tseco= tim_1( inx_1(i));
  rerr= tmx_1(i);
  if( tseco<= 0.00e0 && i<jj ) goto L_823;

  if(  rerr> 0.050e0 ) {
    fprintf( iou,"WARNING(SECOVT): SECOND overhead time relerr%9.4f%%\n",  100.00 * rerr);
  }
  /*                           Good convergence, satifies 1% error tolerence*/
 L_825:

  track ("secovt  ");
  return tseco;

 L_911:
  fprintf( iou,"FATAL(SECOVT): cant measure overhead time subr SECOND\n");
  fprintf( iou,"\n            using SECOND:  elapst=%20.8e      J=%4ld\n",elapst,j);
  where(0);
  return 0.0;
#undef SET_VARS
#include "c_fake1.h"
#include "c_fake2.h"
}


int isigdg(REAL relerr)
{
 return (int) (abs( log10( abs( relerr))) + 0.500e0);
}


void seqdig( long nd[], long *isum, REAL a[], REAL b[], long nr)
{
  /************************************************************************
                                                                          *
         SEQDIG - compute relative error and significant digits of precisi*
                                                                          *
         ND     - Out.Ary  Number of Significant Equal Digits in A.eq.B   *
         isum   - Result   Total number of Equal digits.                  *
         A      -  In.Ary  Reference values for comparison.               *
         B      -  In.Ary  New values of unknown precision.               *
         nr     -  Input   number of results in ND                        *
    *********************************************************************
    */
#undef maxsd
#define maxsd 16

#undef nd_1
#define nd_1(a1) nd[a1-1]
#undef b_1
#define b_1(a1) b[a1-1]
#undef a_1
#define a_1(a1) a[a1-1]

  static REAL re, se, reler, sum, sd, one;
  static long k, maxs;
  /*-----end-of-declarations-----*/
  trace ("seqdig  ");
  /*
         Try to determine floating-point precision used: Max Sig Digits
    */
  maxs= maxsd;
  one= 1.00e0;
  sd= 1073741824.00e0;
  sum= sd + one;
  if( sum == sd )  maxs= 8;

  *isum= 0;
  for (k=1 ; k<=nr ; k++) {
    se= sign( one, a_1(k)) * sign( one, b_1(k));
    if( se < 0.0)  {
      nd_1(k)= 0;
    } else {
      /*
	             compute relative error and digits of precision in B.
	*/
      re=  relerr( a_1(k), b_1(k));
      if((re> 0.0e0 ) && (re< 1.0e0))  {
	nd_1(k)= isigdg(re);
      } else if( re == 0.0e0 )  {
	nd_1(k)= maxs;
      } else if( re >= 1.0e0 )  {
	nd_1(k)= 0;
      }
      if( nd_1(k)> maxs  )  nd_1(k)= maxs;
    }
    *isum += nd_1(k);
  }

  track ("seqdig  ");
  return;
}


void  signel( REAL v[], REAL scale,REAL bias, REAL num)
{
  /************************************************

        SIGNEL GENERATES VERY FRIENDLY FLOATING-POINT NUMBERS NEAR 1.0
                         WHEN SCALE= 1.0 AND BIAS= 0.

         V    - result array,  floating-point test data
         SCALE  - input scalar,  scales magnitude of results
         BIAS   - input scalar,  offsets magnitude of results
         num    - input integer, number of results in V.

    **********************************************/
#undef v_1
#define v_1(a1) v[a1-1]
  static double scaled,biased,fuzz,buzz,fizz,one;
  static long k;
  /*-----end-of-declarations-----*/
  trace ("signel  ");

  scaled= scale;
  biased= bias;

  scaled= 10.00e0;
  scaled=  1.00e0/scaled;
  biased=  0.00e0;
  /*         fuzz= 1.234500d-9*/
  fuzz= 1.234500e-3;
  buzz= 1.000e0  + fuzz;
  fizz= 1.100e0  * fuzz;
  one= 1.000e0;

  for (k=1 ; k<=num ; k++) {
    buzz= (one - fuzz)*buzz +fuzz;
    fuzz= -fuzz;
    /*         V(k)=((BUZZ- FIZZ) -BIASED)*SCALED*/
    v_1(k)= (buzz- fizz)*scaled;
  }

  track ("signel  ");
  return;
}


void simd( REAL hm[],FILE *iou,int iou_flag,REAL rates[],REAL wg[],
	  REAL frval[], long m,long map[],REAL tv1[],REAL tv2[],
	  REAL tv3[], long n)
{
  /************************************************************************
                                                                          *
         SIMD  - Sensitivity Of Harmonic Mean Rate(Mflops) SISD/SIMD Model*
                                                                          *
         HM      - result array,  Harmonic Mean Rates(k)= f( FR(k))       *
         iou     - input scalar,  i/o unit number                         *
         RATES   - input array ,  execution rates (Mflops)                *
         WG      - input array ,  weights paired with RATES               *
         FR      - input array ,  fractions of flops executed SIMD        *
         m       - input scalar,  number of fractions                     *
                                                                          *
         MAP,TV,TV1,TV2,TV3    -  output temporary arrays                 *
         n       - input scalar,  number of rates, etc.                   *
                                                                          *
    *********************************************************************

SENSITIVITY OF NET MFLOPS RATE TO USE OF OPTIMAL FORTRAN CODE(SISD/SIMD MODEL)
   */
#undef hm_1
#define hm_1(a1) hm[a1-1]
#undef wg_1
#define wg_1(a1) wg[a1-1]
#undef rates_1
#define rates_1(a1) rates[a1-1]
#undef frval_1
#define frval_1(a1) frval[a1-1]
#undef tv3_1
#define tv3_1(a1) tv3[a1-1]
#undef tv2_1
#define tv2_1(a1) tv2[a1-1]
#undef tv1_1
#define tv1_1(a1) tv1[a1-1]
#undef map_1
#define map_1(a1) map[a1-1]
#undef stat2_1
#define stat2_1(a1) stat2[a1-1]

  static REAL stat2[20];
  static REAL g, fuzz, smf, vmf, rate, bu, bl;
  static long nq, lh, med, k, neff, meff;
  /*-----end-of-declarations-----*/
  trace ("simd    ");

  /*                 Compress valid data sets RATES,  mapping on MAP.*/

  meff= 0;
  neff= 0;
  bl= 1.0e-5;
  bu= 1.0e+5;
  valid( tv1, map, &neff,  bl, rates, bu, n);

  for (k=1 ; k<=neff ; k++) {
    tv3_1(k)=    wg_1( map_1(k));
  }


  /*                 Compress valid data sets WG,  mapping on MAP.*/

  valid( tv3,map,&meff,  bl, tv3, bu, neff);

  for (k=1 ; k<=meff ; k++) {
    tv2_1(k)= tv1_1( map_1(k));
  }

  /*                 Sort RATES,WT into descending order.*/

  statw( stat2,tv1,map, tv2, tv3, meff);
  med= meff + 1 - (int)stat2_1(8);
  lh= meff + 1 - med;

  for (k=1 ; k<=meff ; k++) {
    tv2_1(k)= tv3_1( map_1(k));
  }


  /*                 Estimate vector rate= HMean of top LFK quartile.*/

  nq= meff/4;
  statw( stat2,tv3,map, tv1,tv2,nq);
  vmf= stat2_1(5);

  /*                 Estimate scalar rate= HMean of lowest two LFK quartiles.*/

  statw( stat2,tv3,map, &(tv1_1(med)),&(tv2_1(med)),lh);
  smf= stat2_1(5);
  fuzz= 1.0e-9;

  g= 1.0e0 -   smf/( vmf + fuzz);
  hm_1(1)= smf;

  for (k=2 ; k<=m ; k++) {
    hm_1(k)=   smf/( 1.0e0 - frval_1(k)*g + fuzz);
  }

  if( iou_flag > 0)  {
    if(full_out) {
      fprintf( iou,"\n\n\n\n\n\n\n\n\n");
      fprintf( iou," SENSITIVITY OF NET MFLOPS RATE TO USE OF OPTIMAL FORTRAN CODE(SISD/SIMD MODEL)\n");
      for(k= 1; k <= 5; k++) {
        fprintf( iou,"%7.2f", hm_1(k));
	  }
      for(k= 6; k <= 9; k++) {
        fprintf( iou,"%8.2f", hm_1(k));
	  }
      fprintf( iou,"\n\n");
      for(k= 1; k <= 5; k++) {
        fprintf( iou,"%7.2f", frval_1(k));
	  }
      for(k= 6; k <= 9; k++) {
        fprintf( iou,"%8.2f", frval_1(k));
	  }
      fprintf( iou,"\n");
      fprintf( iou,"   Fraction Of Operations Run At Optimal Fortran Rates\n");
      fprintf( iou,"\n\n");
	}
  }

  track ("simd    ");
  return;

}


void sizes(long i)
{
/* *******************************************************************
            SIZES      test and set the loop controls before each kernel test

     i    :=  kernel number

     mk    :=  number of kernels to test
     Nruns :=  number of timed runs of complete test.
     tclock:=  cpu clock resolution or minimum time in seconds.
     Loop  :=  multiple pass control to execute kernel long enough to time.
     n     :=  DO loop control for each kernel.

     ******************************************************************
*/
#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"

  static long iup, nif;
  static FILE *io;

  trace ("sizes   ");

  nif= 0;
  /*                        Set  mk .LE. 47  number of kernels to test.*/
  mk= 24;
  im= 1;
  ml= 3;
  /*                        Set  Nruns .LT. 8  number of timed runs of KERNEL test
                            Set  Nruns= 1   to REDUCE RUN TIME for debug runs.*/
  nruns= 1;
  /*                        Set  Nruns= 7   for Standard BENCHMARK Test. Maximum.*/
  nruns= 7;
  if( nruns> 7) nruns= 7;
  /*
                            Set  Mruns= 7   for Standard BENCHMARK Test.*/
  mruns= nruns;
  /*
    **************************************************************************
             OPTIONAL LONG ENDURANCE TEST FOR NEW HARDWARE ACCEPTANCE TESTING.
             OPTIONAL       Set  Mruns=     for Hardware ENDURANCE TRIAL

             Mruns= Nruns * ( Desired Trial Time(sec) / totjob Time(sec))
                              where totjob-time is LFK Standard benchmark
                              test Job-time printed at end of output file.

       e.g.  12 Hour run on CRAY-XMP :   laps = 43200./ 17.5 = 2468
             12 Hour run on VaxS3500 :   laps = 43200./478.4 =   90

              laps= 1
    **************************************************************************
    */
  mruns= nruns * laps;
  if( mruns<nruns || mruns>500000 ) mruns= nruns;

  if( i==-1)  goto L_73;
  /*
    **************************************************************************
         Domain tests follow to detect overstoring of controls for array opns.
    **************************************************************************
    */
  nif= 1;
  iup= 999000;
  if( iup<65000 ) iup= 65000;
  if( i<1 ||  (i-1)>  24)      goto L_911;
  if( n<0 ||  n>   1001)          goto L_911;
  if(loop<0 || loop>iup)        goto L_911;

  nif= 2;
  if(  il<1 || il>3 )  goto L_911;
  n= ispan_2(i,il);
  loop        = ipass_2(i,il) * mul_1(il);
  loop = multi * loop;
  lp   = loop;
  /*
    Computers with high resolution clocks tic= O(microsec.) should use Loop= 1
         to show un-initialized as well as encached execution rates.

         Loop= 1
    */
  if( loop< 1)   loop= 1;
  lp  = loop;
  l   = 1;
  mpy = 1;
  nif = 3;
  if( n<0 ||  n>   1001)  goto L_911;
  if(loop<0 || loop>iup)  goto L_911;
  n1  = 1001;
  n2  = 101;
  n13 = 64;
  n13h= 32;
  n213= 96;
  n813= 512;
  n14 = 2048;
  n16 = 75;
  n416= 300;
  n21 = 25;

  nt1= 16*1001 +13*101 +2*300 + 2048;
  nt2= 4*512 + 3*25*101 +121*101 +3*64*64;

 L_73:
  track ("sizes   ");
  return;


 L_911:
  if( !ion ) {
    io=stdout;
  } else {
    io= ion;
  }
  fprintf( io,"\n\n\n\n\n\n FATAL OVERSTORE/ DATA LOSS.  TEST=  %6ld%6ld%6ld%6ld%6ld\n", i, nif, n, loop, il);
  where(0);

/* clear out common block usage */
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
}


void sordid( long i[],REAL w[], REAL v[],long n,long kind)
{
  /* ***********************************************
                        QUICK AND DIRTY PORTABLE SORT.

                    I - RESULT INDEX-LIST. MAPS V TO SORTED W.
                    W - RESULT ARRAY, SORTED V.

                    V - INPUT  ARRAY SORTED IN PLACE.
                    n - INPUT  NUMBER OF ELEMENTS IN V
                 KIND - SORT ORDER:   = 1  ASCENDING MAGNITUDE
                                      = 2 DESCENDING MAGNITUDE

    ********************************************* */

#undef i_1
#define i_1(a1) i[a1-1]
#undef v_1
#define v_1(a1) v[a1-1]
#undef w_1
#define w_1(a1) w[a1-1]

  static REAL x;
  static long m, j, k;
  /*-----end-of-declarations-----*/
  trace ("sordid  ");

  if( n<=0 )  goto L_73;
  for (k=1 ; k<=n ; k++) {
    w_1(k)= v_1(k);
    i_1(k)= k;
  }

  if( kind==1)  {

    for (j=1 ; j<=n-1 ; j++) {
      m= j;
      for (k=j+1 ; k<=n ; k++) {
	if( w_1(k)<w_1(m)) m= k;
      }
      x= w_1(j);
      k= i_1(j);
      w_1(j)= w_1(m);
      i_1(j)= i_1(m);
      w_1(m)= x;
      i_1(m)= k;
    }


  } else {

    for (j=1 ; j<=n-1 ; j++) {
      m= j;
      for (k=j+1 ; k<=n ; k++) {
	if( w_1(k)>w_1(m)) m= k;
      }
      x= w_1(j);
      k= i_1(j);
      w_1(j)= w_1(m);
      i_1(j)= i_1(m);
      w_1(m)= x;
      i_1(m)= k;
    }
  }

  if( n>0 )  {
    trap( i, " sordid  ", 1, n,n);
  }

 L_73:
  track ("sordid  ");
  return;
}


void  space(void)
{
  /************************************************

                SPACE      sets memory pointers for array variables.  optional.

         Subroutine Space dynamically allocates physical memory space
         for the array variables in KERNEL by setting pointer values.
         The POINTER declaration has been defined in the IBM PL1 language
         and defined as a Fortran extension in Livermore and CRAY compilers.

         In general, large FORTRAN simulation programs use a memory
         manager to dynamically allocate arrays to conserve high speed
         physical memory and thus avoid slow disk references (page faults).

         It is sufficient for our purposes to trivially set the values
         of pointers to the location of static arrays used in common.
         The efficiency of pointered (indirect) computation should be measured
         if available.

    **********************************************/

  trace ("space   ");

  track ("space   ");
  return;
}


void stats( REAL stat[], REAL x[], long n)
{
  /************************************************

         UNWEIGHTED STATISTICS: MEAN, STADEV, MIN, MAX, HARMONIC MEAN.

         STAT(1)= THE MEAN OF X.
         STAT(2)= THE STANDARD DEVIATION OF THE MEAN OF X.
         STAT(3)= THE MINIMUM OF X.
         STAT(4)= THE MAXIMUM OF X.
         STAT(5)= THE HARMONIC MEAN
         X       IS THE ARRAY  OF INPUT VALUES.
         n       IS THE NUMBER OF INPUT VALUES IN X.

    **********************************************/

#undef stat_1
#define stat_1(a1) stat[a1-1]
#undef x_1
#define x_1(a1) x[a1-1]

  static REAL h, v, u, d, a, s;
  static long k;
  /*-----end-of-declarations-----*/
  trace ("stats   ");

  /* The original Fortran version zeroed 9 elements, but some
     of the arrays passed in have only 5 elements. Thus change to 5. */
  for (k=1 ; k<=5 ; k++) {
    stat_1(k)= 0.0;
  }

  if(n<=0)  goto L_73;
  /*                             CALCULATE MEAN OF X.*/
  s= 0.0;
  for (k=1 ; k<=n ; k++) {
    s += x_1(k);
  }
  a= s/n;
  stat_1(1)= a;
  /*                             CALCULATE STANDARD DEVIATION OF X.*/
  d= 0.0;
  for (k=1 ; k<=n ; k++) {
    d += (x_1(k)-a)*(x_1(k)-a);
  }
  d /= n;
  stat_1(2)= sqrt(d);
  /*                             CALCULATE MINIMUM OF X.*/
  u= x_1(1);
  for (k=2 ; k<=n ; k++) {
    u= min(u,x_1(k));
  }
  stat_1(3)= u;
  /*                             CALCULATE MAXIMUM OF X.*/
  v= x_1(1);
  for (k=2 ; k<=n ; k++) {
    v= max(v,x_1(k));
  }
  stat_1(4)= v;
  /*                             CALCULATE HARMONIC MEAN OF X.*/
  h= 0.0;
  for (k=1 ; k<=n ; k++) {
    if( x_1(k)!=0.0) h= h + 1.0/x_1(k);
  }
  if( h!=0.0) h= (REAL)n/h;
  stat_1(5)= h;

 L_73:
  track ("stats   ");
  return;
}


void statw( REAL stat[],REAL ox[],long ix[], REAL x[],REAL w[],long n)
{
  /* ***********************************************

         WEIGHTED STATISTICS: MEAN, STADEV, MIN, MAX, HARMONIC MEAN, MEDIAN.

         STAT( 1)=  THE MEAN OF X.
         STAT( 2)=  THE STANDARD DEVIATION OF THE MEAN OF X.
         STAT( 3)=  THE MINIMUM OF X.
         STAT( 4)=  THE MAXIMUM OF X.
         STAT( 5)=  THE HARMONIC MEAN
         STAT( 6)=  THE TOTAL WEIGHT.
         STAT( 7)=  THE MEDIAN.
         STAT( 8)=  THE MEDIAN INDEX, ASCENDING.
         STAT( 9)=  THE ROBUST MEDIAN ABSOLUTE DEVIATION.
         STAT(10)=  THE GEOMETRIC MEAN
         STAT(11)=  THE MOMENTAL SKEWNESS
         STAT(12)=  THE KURTOSIS
         STAT(13)=  THE LOWER QUARTILE BOUND Q1/Q2 VALUE
         STAT(14)=  THE UPPER QUARTILE BOUND Q3/Q4 VALUE
         STAT(15)=  THE DEVIATION OF THE GEOMETRIC MEAN OF X

         OX      IS THE ARRAY  OF ORDERED (DECENDING) Xs.
         IX      IS THE ARRAY  OF INDEX LIST MAPS X TO OX.

         X       IS THE ARRAY  OF INPUT VALUES.
         W       IS THE ARRAY  OF INPUT WEIGHTS.
         n       IS THE NUMBER OF INPUT VALUES IN X.

    ********************************************* */

#undef w_1
#define w_1(a1) w[a1-1]
#undef x_1
#define x_1(a1) x[a1-1]
#undef ix_1
#define ix_1(a1) ix[a1-1]
#undef ox_1
#define ox_1(a1) ox[a1-1]
#undef stat_1
#define stat_1(a1) stat[a1-1]

  static REAL gm, dxd, powten, g, r, qt, ew, h, v, b, u;
  static REAL q, f, e, d, t, s, a, stin14, stin13, stin09;
  static long k;
  /*-----end-of-declarations-----*/
  trace ("statw   ");
  stin09= 0.00e0;
  stin13= 0.00e0;
  stin14= 0.00e0;

  for (k=1 ; k<=15 ; k++) {
    stat_1(k)= 0.0e0;
  }

  if( n<=0 )  goto L_73;

  if( n==1 )  {
    stat_1( 1)= x_1(1);
    stat_1( 3)= x_1(1);
    stat_1( 4)= x_1(1);
    stat_1( 5)= x_1(1);
    stat_1( 6)= w_1(1);
    stat_1( 7)= x_1(1);
    stat_1( 8)= 1.0e0;
    stat_1(10)= x_1(1);
    goto L_73;
  }

  /*                            CALCULATE MEAN OF X.*/
  a= 0.0e0;
  s= 0.0e0;
  t= 0.0e0;

  for (k=1 ; k<=n ; k++) {
    s += w_1(k)*x_1(k);
    t += w_1(k);
  }
  if( t!=0.0e0) a= s/t;
  stat_1(1)= a;
  /*                             CALCULATE STANDARD DEVIATION OF X.*/
  d= 0.0e0;
  e= 0.0e0;
  f= 0.0e0;
  q= 0.0e0;
  u= 0.0e0;

  for (k=1 ; k<=n ; k++) {
    b= w_1(k) * (x_1(k) -a)*(x_1(k) -a);
    d += b;
    e += b*( x_1(k) -a);
    f += b*(x_1(k) -a)*(x_1(k) -a);
  }
  if( t!=0.0e0) q= 1.0e0/t;
  d= d*q;
  e= e*q;
  f= f*q;
  if( d>=0.0e0) u= sqrt(d);
  stat_1(2)= u;
  /*                             CALCULATE MINIMUM OF X.*/
  u= x_1(1);
  for (k=2 ; k<=n ; k++) {
    u= min(u,x_1(k));
  }
  stat_1(3)= u;
  /*                             CALCULATE MAXIMUM OF X.*/
  v= x_1(1);
  for (k=2 ; k<=n ; k++) {
    v= max(v,x_1(k));
  }
  stat_1(4)= v;
  /*                             CALCULATE HARMONIC MEAN OF X.*/
  h= 0.0e0;
  for (k=1 ; k<=n ; k++) {
    if( x_1(k)!=0.0e0) h += w_1(k)/x_1(k);
  }
  if( h!=0.0e0) h= t/h;
  stat_1(5)= h;
  stat_1(6)= t;
  /*                             CALCULATE WEIGHTED MEDIAN*/
  sordid( ix, ox, x, n, 1);

  ew= 0.0e0;
  for (k=2 ; k<=n ; k++) {
    if( w_1(1) != w_1(k))  goto L_75;
  }
  ew= 1.0e0;
 L_75:

  qt= 0.500e0;
  tile( &(stat_1(7)), &(stat_1(8)), ox,ix,w,ew,t, qt,n);

  qt= 0.250e0;
  tile( &(stat_1(13)),  &stin13, ox,ix,w,ew,t, qt,n);

  qt= 0.750e0;
  tile( &(stat_1(14)),  &stin14, ox,ix,w,ew,t, qt,n);

  /*                        CALCULATE ROBUST MEDIAN ABSOLUTE DEVIATION (MAD)*/
  for (k=1 ; k<=n ; k++) {
    ox_1(k)= abs( x_1(k) - stat_1(7));
  }

  sordid( ix, ox, ox, n, 1);

  qt= 0.700e0;
  tile( &(stat_1(9)),  &stin09, ox,ix,w,ew,t, qt,n);
  /*                             CALCULATE GEOMETRIC MEAN*/
  r= 0.0e0;
  for (k=1 ; k<=n ; k++) {
    if( x_1(k) > 0.0e0) {
      r += w_1(k) *log10( x_1(k));
    }
  }
  u= r*q;
  g= 10.0e0;
  powten= 50.0e0;
  if(u < 0) {
    g= 0.1;
    if( -u > powten)  u= -powten;
    stat_1(10)=  pow(g, -u);
  } else {
    if( u > powten)  u= powten;
    stat_1(10)=  pow(g, u);
  }
  /*                             CALCULATE MOMENTAL SKEWNESS*/
  g= 0.0e0;
  dxd= d*d;
  if( dxd!=0.0e0) g= 1.0e0/(dxd);
  stat_1(11)= 0.50e0*e*g*stat_1(2);
  /*                             CALCULATE KURTOSIS*/
  stat_1(12)= 0.50e0*( f*g -3.0e0);
  /*                             CALCULATE DEVIATION OF GEOMETRIC MEAN*/
  d= 0.0e0;
  q= 0.0e0;
  u= 0.0e0;
  gm= stat_1(10);

  for (k=1 ; k<=n ; k++) {
    b= w_1(k) * (x_1(k) -gm)*(x_1(k) -gm);
    d += b;
  }
  if( t!=0.0e0) q= 1.0e0/t;
  d= d*q;
  if( d>=0.0e0) u= sqrt(d);
  stat_1(15)= u;
  /*                          CALCULATE DESCENDING ORDERED X.*/
  sordid( ix, ox, x, n, 2);

 L_73:
  track ("statw   ");
  return;
}


REAL sumo( REAL v[], long n)
{
  /*
    *********************************************

         CHECK-SUM WITH ORDINAL DEPENDENCY.

         V   - input array,   floating-point numbers
         n   - input integer, number of elements in V.

    **********************************************

       Use the following Double Precision declaration to improve Real*4 tests.
       Use the following Real*16          declaration to improve Real*8 tests.
   */
#undef v_1
#define v_1(a1) v[a1-1]

  double s;  /* double even for a float run */
  long k;

  trace ("sumo    ");
  s= 0.00e0;

  for (k=1 ; k<=n ; k++) {
    s += (double)k*v_1(k);
  }
  track ("sumo    ");
  return s;
}


void  supply(long i)
{
  /************************************************

             SUPPLY     initializes common blocks containing type real arrays.

         i    :=  kernel number

    ***************************************************************************

   The Fortran source plays games with different common block
   definitions in different routines. The variables used here
   are the ones that map the whole common block into a single
   array
*/
#define SET_VARS
#include "c_cernel.h"
#include "c_cksums.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"

/*
    *********************************************************************
               Method 2:  Double space and least cpu time
    *********************************************************************

*/

  static double    ds, dw;  /* double even for a float run */
  static long k, j, nt0, ip1;
  /*-----end-of-declarations-----*/
  trace ("supply  ");

  ip1= i;
  nt0= 39;
  /*            Execute SIGNEL calls only once; re-use generated data.*/
  ibuf++;
  if( ibuf== 1) {
    signel(  bufu, skale_1(ip1), bias_1(ip1), nt1);
    signel(  bufp, skale_1(ip1), bias_1(ip1), nt2);
    signel(  bufa, skale_1(ip1), bias_1(ip1), nt0);
    ds= 1.000e0;
    dw= 0.500e0;
    for (j=1 ; j<=4 ; j++) {
      for (k=1 ; k<=512 ; k++) {
	p0_2(j,k) = ds;
	ds += dw;
      }
    }
  }
  /*                                  Test for Trashing Data in BUF*/
  idebug=   0;
  if(idebug == 1 || ibuf == 1 || i == (24-1))  {

    cksumu= sumo( bufu, nt1);
    cksump= sumo( bufp, nt2);
    cksuma= sumo( bufa, nt0);

    if( ibuf== 1) {
      ckoldu= cksumu;
      ckoldp= cksump;
      ckolda= cksuma;
    } else if(   cksumu!=ckoldu
	      || cksump!=ckoldp
	      || cksuma!=ckolda )  {
      fprintf( ion," SUPPLY: OVERSTORED! Trial=%2ld Pass=%2ld Kernel=%3ld\n", jr, il, ik);
      fprintf( ion," ckold:%24.15e%24.15e%24.15e\n", ckoldu, ckoldp, ckolda);
      fprintf( ion," cksum:%24.15e%24.15e%24.15e\n", cksumu, cksump, cksuma);
    }
  }
  /*                           Refill Work-Space from copies in Buffers*/
  for (k=1 ; k<=nt0 ; k++) {
    aaa11_1(k)= bufa_1(k);
  }
  for (k=1 ; k<=nt1 ; k++) {
    uuu_1(k)= bufu_1(k);
  }
  for (k=1 ; k<=nt2 ; k++) {
    ppp_1(k)= bufp_1(k);
  }

  track ("supply  ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_cksums.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"
}


double frac(double z)
{
  double res;

  if(z < 0) {
    res= -( abs( abs(z) - floor(abs(z)) ) );
  } else {
    res= ( abs( abs(z) - floor(abs(z)) ) );
  }
  return res;
}


void tdigit( REAL *derr, long *nzd, REAL s )
{
  /************************************************************************
                                                                          *
         TDIGIT  -  Count Lead Digits Followed By Trailing Zeroes.        *
                                                                          *
           derr  -  Result,  Digital Error in percent.                    *
            nzd  -  Result,  Number Of Lead Digits                        *
              s  -  Input ,  A Floated Integer                            *
                                                                          *
    **********************************************************************/

  static double   fuzz, x, y, v, z;  /* double even for a float run */
  static long k, n;
  /*-----end-of-declarations-----*/

  trace ("tdigit  ");

  x= 0.00e0;
  n= 14;
  x= abs(s);
  fuzz= 1.0e-6;
  *derr= 100.0e0;
  *nzd= 0;
  if( x != 0.0) {
    /*                                  Normalize x*/
    y= log10(x);
    v= pow(10.0,( abs((int)y) + 1 ) );

    if( (y >= 0.0e0) && (v!= 0.0e0))  {
      x= (x/v) * 10.0e0;
    } else {
      x= x*v;
    }
    /*                        Multiply x Until Trailing Digits= Fuzz*/
    for (k=1 ; k<=n ; k++) {
      if( ((1.0e0-frac(x))<=fuzz) || (frac(x)<=fuzz)) break;
      x= 10.0e0*x;
    }

    if( x!= 0.0e0)  {
      *derr= 50.0e0/x;
      *nzd= (int) log10( abs( 9.999999990e0*x ));
    }
  }

 track ("tdigit  ");
  return;
}


long test( long i )
{
  /*
    *********************************************************************
                                                                          *
                  REPEAT AND TIME THE EXECUTION OF KERNEL i               *
                                                                          *
                        i  - Input integer;   Test Kernel Serial Number   *
                     TEST  - Repetition Loop Counter, decremented to 0    *
                                                                          *
    *********************************************************************
    */
#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"

/* remove the start etc. macros as they are not used here */
#undef start
#undef ik
#undef k
#undef xz_1
#define xz_1(a1) xz[a1-1]
#undef zx_1
#define zx_1(a1) zx[a1-1]

  static REAL  *zx= z, *xz= x;
  static REAL start, tempus;
  static long j, k, nn, ik;
  /*-----end-of-declarations-----*/


/*
    *************************************************************************
             Repeat execution of each Kernel(i) :     DO 1 L= 1,Loop   etc.
    *************************************************************************

        From the beginning in 1970 each sample kernel was executed just
        once since supercomputers had high resolution, microsecond clocks.
        In 1982 a repetition Loop was placed around each of the 24 LFK
        kernels in order to run each kernel long enough for accurate
        timing on mini-computer systems with poor cpu-clock resolution since
        the majority of systems could only measure cpu-time to 0.01 seconds.
        By 1990 however, several compilers" optimizers were factoring or
        hoisting invariant computation outside some repetition Loops thus
        distorting those Fortran samples.  The effect was usually absurd
        Mflop rates which had to be corrected with compiler directives.
        Therefore, in April 1990 these repetition Loops were removed from
        subroutine KERNEL and submerged in subroutine TEST beyond the scope
        of compiler optimizations.   Thus the 24 samples are now foolproof
        and it will no longer be necessary to double check the machine code.

        Very accurate, convergent methods have been developed to measure the
        overhead time used for subroutines SECOND and TEST in subroutines
        SECOVT and TICK respectively.  Thus, the LFK test may use substantially
        more cpu time on systems with poor cpu-clock resolution.
        The 24 C verison tests in CERNEL have also been revised to correspond with
        the Fortran KERNEL. The 24 computation samples have NOT been changed.

    **************************************************************************

                                        Repeat kernel test:   Loop times.
*/
  if( l < loop )  {
    l++;
    return l;
  }
  /*                                    Repeat kernel test:   Loop*Loops2*/
  ik   = i;
  if( mpy < loops2 )  {
    mpy++;
    nn   = n;

    if( i==0 ) goto L_100;
    if( i<0 || i>24 )  {
      trace("test    ");
      where(0);
    }
    /*                   RE-INITIALIZE OVER-STORED INPUTS:   */
    switch(i) {
    /*
           When MULTI.GE.100 each kernel is executed over a million times
           and the time used to re-intialize overstored input variables
           is negligible.  Thus each kernel may be run arbitrarily many times
           (MULTI >> 100) without overflow and produce verifiable checksums.

      *********************************************************************
      */
    default:
      /* ************************************************************** */
    case 2:
      for (k=1 ; k<=nn ; k++) {
        x_1(k)= x0_1(k);
      }
      break;
      /* *************************************** */
    case 4:
      m= (1001-7)/2;
      for (k=7 ; k<=1001 ; k+=m) {
        xz_1(k)= x0_1(k);
      }
      break;
      /* *************************************** */
    case 5:
      for (k=1 ; k<=nn ; k++) {
        x_1(k)= x0_1(k);
      }
      break;
      /* *************************************** */
    case 6:
      for (k=1 ; k<=nn ; k++) {
        w_1(k)= w0_1(k);
      }
      break;
      /* *************************************** */
    case 10:
      for (k=1 ; k<=nn ; k++) {
        for (j=5 ; j<=13 ; j++) {
          px_2(j,k)= px0_2(j,k);
        }
      }
      break;
      /* *************************************** */
    case 13:
      for (k=1 ; k<=nn ; k++) {
        p_2(1,k)= p0_2(1,k);
        p_2(2,k)= p0_2(2,k);
        p_2(3,k)= p0_2(3,k);
        p_2(4,k)= p0_2(4,k);
      }

      for (k=1 ; k<=64 ; k++) {
        for (j=1 ; j<=64 ; j++) {
          h_2(j,k)= h0_2(j,k);
        }
      }
      break;
      /* *************************************** */
    case 14:
      {
        long ndx;
        for (k=1 ; k<=nn ; k++) {
          ndx= ir_1(k);
          rh_1(ndx)= rh0_1(ndx);
          rh_1(ndx+1)= rh0_1(ndx+1);
        }
      }
      break;
      /* *************************************** */
    case 16:
      k2= 0;
      k3= 0;
      break;
      /* *************************************** */
    case 17:
      for (k=1 ; k<=nn ; k++) {
	vxne_1(k)= vxne0_1(k);
      }
      break;
      /* *************************************** */
    case 18:
      for (k=2 ; k<=6 ; k++) {
        for (j=2 ; j<=nn ; j++) {
          zu_2(j,k)= zu0_2(j,k);
          zv_2(j,k)= zv0_2(j,k);
          zr_2(j,k)= zr0_2(j,k);
          zz_2(j,k)= zz0_2(j,k);
        }
      }
      break;
      /* *************************************** */
    case 19: stb5= stb50;
      break;
      /* *************************************** */
    case 20:
      xx_1(1)= xx0_1(1);
      break;
      /* *************************************** */
    case 21:
      for (k=1 ; k<=nn ; k++) {
        for (j=1 ; j<=25 ; j++) {
          px_2(j,k)= px0_2(j,k);
        }
      }
      break;
      /* *************************************** */
    case 23:
      /* NOTE: intend to fall through to L_100 */
      for (k=2 ; k<=6 ; k++) {
        for (j=2 ; j<=nn ; j++) {
          za_2(j,k)= za0_2(j,k);
        }
      }
    case 1:
    case 3:
    case 7:
    case 8:
    case 9:
    case 11:
    case 12:
    case 15:
    case 22:
    case 24:
    case 25:
      break;
    }
L_100:
    l    = 1;
    return 1;
  }

  mpy  = 1;
  l    = 1;
  /*                                switchback to TICK to measure testov*/
  if( i== (-73))  return 0;
  /*
    *********************************************************************
               t= second(0)  := cumulative cpu time for task in seconds.
    *********************************************************************
    */
  cumtim_1(1)= 0.0e0;
  tempus= second( cumtim_1(1)) - start;

  trace ("test    ");

  /*                             Checksum results; re-initialize all inputs*/
  tests ( i, tempus );

  track ("test    ");
  mpy   = 1;
  mpylim= loops2;
  l     = 1;
  lp    = loop;
  ik    = i+1;
  cumtim_1(1)= 0.0e0;
  start= second( cumtim_1(1));
  return 0;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_base1.h"
#include "c_base2.h"
#include "c_baser.h"
}


void  tests( long i, REAL tempus )
{
  /**********************************************************************
                                                                        *
               CHECKSUM AND INITIALIZE THE EXECUTION OF KERNEL i        *
                                                                        *
                    i  - Input integer;  Test Kernel Serial Number      *
               TEMPUS  - Input REAL  ;  Elapsed Cpu-time Test(i) used *
                                                                        *
    *********************************************************************
    */
#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"

  static REAL err, overr;
  static long k, mm, nn, np;
  /*-----end-of-declarations-----*/
  ik   = i;
  trace ("tests   ");

  np   = loop * loops2;
  loop = 1;
  lp   = loop;
  nn   = n;
  if( i<0 || i>24 )  {
    where(0);
  }

  if( i==0 )  goto L_120;
  sizes(i);
  /*
         Net Time=  Timing - Overhead Time
    */
  time_1(i)= tempus - (double)np *testov - tsecov;

  switch(i) {

  default:
    /* ************************************************************* */
  case 1:
    csum_1(1) =  sumo( x, n);
    total_1(1) =  np*nn;
    break;
    /* ************************************************************* */
  case 2:
    csum_1(2) =  sumo( x, 2*n);
    total_1(2) =  np*(nn-4);
    break;
  /* ************************************************************* */
  case 3:
    csum_1(3) =  q;
    total_1(3) =  np*nn;
    break;
  /* ************************************************************* */
  case 4:
    mm= (1001-7)/2;
    for (k=7 ; mm>0?k<=1001:k>=1001 ; k+=mm) {
      v_1(k)= x_1(k);
    }
    csum_1(4) = sumo ( v, 3);
    total_1(4) =  np*(((nn-5)/5)+1)*3;
    break;
    /* ************************************************************* */
  case 5:
    csum_1(5) =  sumo( x+2-1, n-1);
    total_1(5) =  np*(nn-1);
    break;
    /* ************************************************************* */
  case 6:
    csum_1(6) =  sumo( w, n);
    total_1(6) =  np*nn*((nn-1)/2);
    break;
    /* ************************************************************* */
  case 7:
    csum_1(7) =  sumo ( x, n);
    total_1(7) =  np*nn;
    break;
    /* ************************************************************* */
  case 8:
    csum_1(8) = sumo( (REAL *)u1,5*n*2) + sumo( (REAL *)u2,5*n*2)
      + sumo( (REAL *)u3,5*n*2);
    total_1(8) =  np*(nn-1)*2;
    break;
    /* ************************************************************* */
  case 9:
    csum_1(9) =  sumo( (REAL *)px, 15*n);
    total_1(9) =  np*nn;
    break;
    /* ************************************************************* */
  case 10:
    csum_1(10) =  sumo( (REAL *)px, 15*n);
    total_1(10) =  np*nn;
    break;
    /* ************************************************************* */
  case 11:
    csum_1(11) =  sumo( x+2-1, n-1);
    total_1(11) =  np*(nn-1);
    break;
    /* ************************************************************* */
  case 12:
    csum_1(12) =  sumo( x, n-1);
    total_1(12) =  np*nn;
    break;
    /* ************************************************************* */
  case 13:
    csum_1(13) =  sumo( (REAL *)p, 8*n) + sumo( (REAL *)h, 8*n);
    total_1(13) =  np*nn;
    break;
    /* ************************************************************* */
  case 14:
    csum_1(14) =  sumo( vx,n) + sumo( xx,n) + sumo( rh,67);
    total_1(14) =  np*nn;
    break;
    /* ************************************************************* */
  case 15:
    csum_1(15) =  sumo( (REAL *)vy, n*7) + sumo( (REAL *)vs, n*7);
    total_1(15) =  np*(nn-1)*5;
    break;
    /* ************************************************************* */
  case 16:
    csum_1(16) =  (REAL) k3+k2+j5+m;
    flopn_1(16) =  ( k2+k2+10*k3 ) * loops2;
    total_1(16) =  1.0e0;
    break;
    /* ************************************************************* */
  case 17:
    csum_1(17) =  sumo( vxne, n) + sumo( vxnd, n) + xnm;
    total_1(17) =  np*nn;
    break;
    /* ************************************************************* */
  case 18:
    csum_1(18) =  sumo( (REAL *)zr, n*7) + sumo( (REAL *)zz, n*7);
    total_1(18) =  np*(nn-1)*5;
    break;
    /* ************************************************************* */
  case 19:
    csum_1(19) =  sumo( b5, n) + stb5;
    total_1(19) =  np*nn;
    break;
    /* ************************************************************* */
  case 20:
    csum_1(20) =  sumo( xx+2-1, n);
    total_1(20) =  np*nn;
    break;
    /* ************************************************************* */
  case 21:
    csum_1(21) =  sumo( (REAL *)px, 25*n);
    total_1(21) =  np*25*25*nn;
    break;
    /* ************************************************************* */
  case 22:
    csum_1(22) =  sumo( w, n);
    total_1(22) =  np*nn;
    break;
    /* ************************************************************* */
  case 23:
    csum_1(23) =  sumo( (REAL *)za, n*7);
    total_1(23) =  np*(nn-1)*5;
    break;
    /* ************************************************************* */
  case 24:
    csum_1(24) =  (REAL)m;
    total_1(24) =  np*(nn-1);
    break;
    /* ************************************************************* */
  case 25:
    break;
  }
  /* ************************************************************* */
  /*         delta( testov)= relerr * testov*/
  overr= see_4(2,1,jr,il)*(REAL)np* testov;
  terr1_1(i)= 100.0;
  if( time_1(i)!= 0.0e0) terr1_1(i)= terr1_1(i)*(overr/time_1(i));
  npfs1_1(i)= npf;
  if( ion_flag < 0 )  goto L_120;
  /* If this clock resolution test fails, you must increase Loop (Subr. SIZES)
   */
  if( terr1_1(i) < 15.0)  goto L_114;
  fprintf(ion, "\n%2ld TESTS:  POOR TIMING OR ERROR. NEED LONGER RUN\n", i);
 L_114:
  fprintf(ion, " %2ld Done  T=%11.4e  T err=%8.2f%%%8ld  Page-Faults\n",
	  i, time_1(i), terr1_1(i), npf);
 L_120:      if( i>=0 && i<24 )  {
   values(i+1);
   sizes(i+1);
 }

  track ("tests   ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
}


REAL tick( FILE *iou, int iou_flag, long ntimes)
{
  /*
    *********************************************

                TICK       measures timing overhead of subroutine test

         iou    -  Logical Output Device Number                           *

    *********************************************
    */

#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"

  /* NOTE: the "length" of p1 is 4*512+1 */
#define l4813  (4*512)
#undef p1_1
#define p1_1(a1) p1[a1-1]
#undef tmx_1
#define tmx_1(a1) tmx[a1-1]
#undef ter_1
#define ter_1(a1) ter[a1-1]
#undef tim_1
#define tim_1(a1) tim[a1-1]
#undef inx_1
#define inx_1(a1) inx[a1-1]

  REAL *p1= (REAL *) p;
  static REAL tim[20], ter[20], tmx[20];
  static long inx[20];
  static REAL rerr, toler, elapst, t1, t0;
  static long k, j, nt, jj, klm, neff;
  static REAL testo, tick_res;
  static REAL retest=0;
  /*-----end-of-declarations-----*/


  trace ("tick    ");

  /* save output file pointer in common block */
  ion= iou;
  ion_flag= iou_flag;
  kr = mk;
  n  = 0;
  k2 = 0;
  k3 = 0;
  m  = 0;
  neff= 0;
  if( il == 1 )  {
    /*
      *********************************************************************
           Measure tsecov:  Overhead time for calling SECOND
      *********************************************************************
      */
    tsecov = secovt( iou, iou_flag);
    tic = tsecov;
    /*
      *********************************************************************
           Measure testov:  Overhead time for calling TEST
      *********************************************************************
      */
    testo= 0.00e0;
    klm= 8000;
    jj= 0;
    nt= ntimes - 6;
    j= nt;
    if( nt<8 || nt>30 )  goto L_911;

    for (j=1 ; j<=nt ; j++) {
      l= 1;
      mpy= 1;
      loops2= 1;
      mpylim= loops2;
      loop= klm;
      lp= loop;
      /*                             Measure overhead time for empty loop*/
      cumtim_1(1)= 0.0e0;
      t0= second( cumtim_1(1));
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      while( test(-73) > 0 )  ;
      cumtim_1(1)= 0.0e0;
      t1= second( cumtim_1(1)) - tsecov;
      elapst= t1 - t0;
      testo= elapst/( (REAL)(10*klm) + 1.0e-9);
      toler= 0.020e0;
      rerr= 1.00e0;
      /*                             Convergence test:  Rel.error .LT. 1%*/
      if( elapst> 1.00e04 ) goto L_911;
      if( elapst< 1.00e-9 && j>8 ) goto L_911;
      if( elapst> 1.00e-9 ) {
	jj++;
	tim_1(jj)= testo;
	if( jj>1 ) {
	  rerr= relerr( tim_1(jj), tim_1(jj-1));
	}
	ter_1(jj)= rerr;
      }

      if( iou_flag >= 0  ) {
	fprintf( iou,"testov(TICK)%12ld%12.4e%11.4f\n", 10*klm,  testo, rerr);
      }
      if( rerr  < toler   ) goto L_825;
      if( elapst> 10.00e0 ) goto L_822;
      klm= klm + klm;
    }
    /*                                Poor accuracy on exit from loop*/
  L_822:     if( j <= 1 )  goto L_911;
    if( jj< 1 )  goto L_911;
    sordid( inx,tmx,  ter,jj,1);
    testo= tim_1( inx_1(1));
    rerr= tmx_1(1);
    fprintf( iou,"WARNING(TICK):  TEST overhead time relerr%9.4f%%\n", 100.0*rerr);
    /*                       Good convergence, satifies 1% error tolerence*/
  L_825:
    testov        = testo;
    retest        = rerr * testov;
  }
  /*
    ***********************************************************************
                                      Generate data sets*/
  see_4(1,1,jr,il)= testov;
  see_4(2,1,jr,il)= retest;
  ticks         = testov;
  tick_res      = testov;
  l      = 1;
  loop   = 1;
  lp     = loop;
  j      = test(0);

  for (k=1 ; k<=47 ; k++) {
    time_1(k)= 0.0e0;
    csum_1(k)= 0.0e0;
  }

  if( il == 1 )  {
    stats( &(see_4(1,2,jr,il)), u, nt1);
    stats( &(see_4(1,3,jr,il)), (REAL *) p, nt2);
    stats( &(see_4(1,3,jr,il)), &(p1_1(l4813+1)), nt2-l4813);
  } else {
    for (k=1 ; k<=5 ; k++) {
      see_4( k,2,jr,il)= see_4( k,2,jr,1);
      see_4( k,3,jr,il)= see_4( k,3,jr,1);
    }
  }

  if( iou_flag >= 0 ) {
    fprintf( iou, "\n\nCLOCK OVERHEAD:  \n");
    fprintf( iou,"\n             AVERAGE        STANDEV        MINIMUM        MAXIMUM\n");
    fprintf( iou,"\n TICK%15.6e%15.6e\n", see_4(1,1,jr,il),
	    see_4(2,1,jr,il) );
    fprintf( iou,"\n DATA%15.6e%15.6e%15.6e%15.6e\n", see_4(1,2,jr,il),
	    see_4(2,2,jr,il), see_4(3,2,jr,il), see_4(4,2,jr,il) );
    fprintf( iou,"\n DATA%15.6e%15.6e%15.6e%15.6e\n", see_4(1,3,jr,il),
	    see_4(2,3,jr,il), see_4(3,3,jr,il), see_4(4,3,jr,il) );
  }

  track ("tick    ");
  return tick_res;

 L_911:
  fprintf( iou,"FATAL(TICK): cant measure overhead time of subr. TEST\n");
  fprintf( iou,"\n            using SECOND:  elapst=%20.8e      J=%4ld\n", elapst, j);
  where(0);
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
  return tick_res;
}


void tile( REAL *sm, REAL *si, REAL ox[],long ix[],REAL w[],
	  REAL ew,REAL t,REAL tiles,long n)
{
  /************************************************

         TILE       computes  m-tile value and corresponding index

         sm      -  RESULT VALUE  IS m-TILE VALUE
         si      -  RESULT VALUE  IS CORRESPONDING INDEX.r IN W

         OX      -  INPUT  ARRAY  OF ORDERED (DECENDING) Xs.
         IX      -  INPUT  ARRAY  OF INDEX LIST MAPS X TO OX.
         W       -  INPUT  ARRAY  OF INPUT  WEIGHTS.
         ew      -  INPUT  VALUE  FLAGS EQUAL WEIGHTS= 1.0; ELSE 0.0d0
         T       -  INPUT  VALUE  IS SUM OF WEIGHTS
         tiles   -  INPUT  VALUE  IS FRACTION OF RANGE, E.G. 0.25
         n       -  INPUT  NUMBER OF INPUT  VALUES IN X.

    **********************************************/

#undef w_1
#define w_1(a1) w[a1-1]
#undef ox_1
#define ox_1(a1) ox[a1-1]
#undef ix_1
#define ix_1(a1) ix[a1-1]

  REAL y, z, s, r, thresh;
  long k;
  /*-----end-of-declarations-----*/
  trace ("tile    ");

  thresh= tiles*t + 0.50e0*ew*w_1(1);
  r= 0.0e0;
  s= r;
  for (k=1 ; k<=n ; k++) {
    s= r;
    r += w_1( ix_1(k));
    if( r > thresh )  goto L_7;
  }
  k= n;
 L_7:       z= 0.0e0;
  y= 0.0e0;
  if( k>1 )    y =   ox_1(k-1);
  if( r!=s )    z = ( thresh - s)/( r - s);
  *sm= y         + z * ( ox_1(k)  - y);
  *si= (REAL)(k-1) + z;

  track ("tile    ");
  return;
}


void trace ( char *name )
{
  /************************************************

          Records /DEBUG/ info: sequence of called subroutine names
          Enters current subroutine name on top of /DEBUG/ stack
          NOT NECESSARY FOR PERFORMANCE TEST, MAY BE DUMMIED.

          name   -  Input;  Callers name
          ISTACK -  Contains names of subroutines in active linkage chain.

                    Interrupt shows active chain of subr. names in ISTACK:
          bkp kernel
          run
          BREAKPOINT REACHED AT 00417457PB = KERNEL:KERNEL+201PB
          bkp trap
          run
          BREAKPOINT REACHED AT 00450122PB = TRAP:TRAP+45PB
          sub= tracks  bcd  istack,10  dec  nstack,10

          ISTACK(1) = "IQRANF  VALUES  TEST    KERNEL   MAIN.  "
          NSTACK(1) =  164  162  157  65  1  0  0  0  0  0

                    Setting TRACE  call 5317 causes CALL to STOPS:
          sub= tracks
          set match= 5317
          rel all.
          bkp trace
          run
          BREAKPOINT REACHED AT 00440575PB = STOPS:STOPS+6PB
          sub= tracks  bcd  istack,10  dec  nstack,10

          ISTACK(1) = "SORDID  STATW   SENSIT  REPORT   MAIN.  "
          NSTACK(1) =  5317  5316  5308  5282  1  0  0  0  0  0

    **********************************************/

#ifdef DO_DEBUG
#define SET_VARS
#include "c_debug.h"
#include "c_order.h"

  /*              pushdown stack of subroutine names and call nrs.*/
  long k;
  /*-----end-of-declarations-----*/
  for (k=10 ; k>=2 ; k--) {
    nstack_1(k)= nstack_1(k-1);
    str8cpy(istack_1(k), istack_1(k-1));
  }

  inseq++;
  nstack_1(1)= inseq;
  str8cpy(istack_1(1), name);
  isave= inseq;
  printf( " %6ld    %8s%8s%8s%8s%8s%8s%8s\n", inseq, istack_1(1),
	 istack_1(2), istack_1(3), istack_1(4), istack_1(5),
	 istack_1(6), istack_1(7) );
  printf(" %6ld    %8s%8s%8s%8s%8s%8s%8s\n",
	 inseq, istack_1(1), istack_1(2), istack_1(3), istack_1(4),
	 istack_1(5), istack_1(6), istack_1(7));
  if( inseq==match ) {
    stops();
  }

  watch(1);
#undef SET_VARS
#include "c_debug.h"
#include "c_order.h"
#endif
}


void stops(void)
{
  /************************************************

         This routine is a convenient program break-point which is
         selected by pre-setting:  match in COMMON /ORDER/  or by data
         loading in BLOCK DATA  to equal the serial index of a
         particular call to TRACE , as previously recorded in NSTACK.
         The call to STOPS is selected in subroutine TRACE .

  */
  return;
}


void track( char *name )
{
  /************************************************

          Releases current subroutine name from top of /DEBUG/ stack
          NOT NECESSARY FOR PERFORMANCE TEST, MAY BE DUMMIED.

    **********************************************/

#ifdef DO_DEBUG
#define SET_VARS
#include "c_debug.h"
#include "c_order.h"

  long k;
  /*-----end-of-declarations-----*/
  iret++;
  watch(2);
  /*                             pop stack of subroutine names*/
  if( !str8cmp(name, istack_1(1)) )  {
    for (k=1 ; k<=9 ; k++) {
      nstack_1(k)= nstack_1(k+1);
      str8cpy(istack_1(k), istack_1(k+1));
    }
  } else {
    str8cpy(istack_1(20), name);
    where(12);
  }
#undef SET_VARS
#include "c_debug.h"
#include "c_order.h"
#endif
}


void trap( long i[], char *name, long mini, long maxi,long meff)
{
  /************************************************

          Checks that Index List values are in valid domain

         I     - ARRAY  OF INPUT INDEX-LIST
         name  -           INPUT CALLERS name
         mini  - INPUT SMALLEST INDEX VALUE
         maxi  - INPUT LARGEST  INDEX VALUE
         meff  - NUMBER OF INPUT VALUES IN I.

    **********************************************/

#define SET_VARS
#include "c_cernel.h"

#undef i_1
#define i_1(a1) i[a1-1]

  long k, lx;
  FILE *io;
  /*-----end-of-declarations-----*/
  trace ("trap    ");

  lx= 0;
  for (k=1 ; k<=meff ; k++) {
    if( i_1(k)<mini || i_1(k)>maxi )  lx= k;
  }

  if( lx!=0 )   {
    if( ion_flag <= 0 ) {
      io= stdout;
    } else {
      io= ion;
    }
    fprintf( io,"\n\n\n\ntrap: error in index-list(%4ld)  in subr:  %8s",  lx, name);
    for(k= 1; k <= meff; k++) {
      fprintf( io,"%6ld", i_1(k));
    }
    fprintf(io, "\n");

    where(0);
  }

  track ("trap    ");
#undef SET_VARS
#include "c_cernel.h"
}


long modi(long ii, long mm)
{
  return (long) (mod( abs(ii)-1, mm) + 1);
}


long nper(long ii, long mm)
{
  return (long) ((abs(ii)-1+mm)/(mm));
}


void trial( FILE *iou, long i, REAL t0, REAL tj )
{
  /************************************************

         TRIAL - validates checksums of current run for endurance trial

    *********************************************************************
    */
#undef mall
#define mall (24 * 3)
#undef id_1
#define id_1(a1) id[a1-1]
#undef ld_1
#define ld_1(a1) ld[a1-1]
#undef cs2_1
#define cs2_1(a1) cs2[a1-1]
#undef cs1_1
#define cs1_1(a1) cs1[a1-1]

#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"

  static long id[mall], ld[mall];
  static REAL cs1[mall],  cs2[mall];
  static REAL fail, pass, chksum, trial, estime;
  static long k, j, lx, ijk;
  static long isum;
  /*-----end-of-declarations-----*/

  trace ("trial   ");

  if( i==1 ) {
    estime= (tj-t0) + (REAL)mruns *( second(0) - tj);
    fprintf( iou, "\nESTIMATED TOTAL JOB CPU-TIME:=%10.3f sec.   ( nruns=%8ld trials)\n", estime, nruns);
    printf("\nestimated total job cpu-time:=%10.3f sec.   ( nruns=%8ld trials)\n", estime, nruns);
  }

  ijk= 4;
  if( multi<=   1 )  ijk= 1;
  if( multi==  10 )  ijk= 2;
  if( multi==  50 )  ijk= 3;
  if( multi>= 100 )  ijk= 4;

  lx= 0;
  for (j=im ; j<=ml ; j++) {
    for (k=1 ; k<=24 ; k++) {
      lx++;
      cs1_1(lx)= csums_3(jr,j,k);
      cs2_1(lx)= sums_3(k,j,ijk);
    }
  }

  seqdig( id, &isum, cs1, cs2, mall);

  if( i==1 ) {

    for (k=1 ; k<=mall ; k++) {
      ld_1(k)= id_1(k);
    }
  } else {
    if( isum==last && isum>200 ) {
      npass++;
    } else {
      nfail++;

      for (k=1 ; k<=mall ; k++) {
	if( id_1(k) != ld_1(k))  {
	  fprintf( iou,"TRIAL:%7ld      Kernel=%5ld      j= %7ld      ERROR%7ld%7ld\n", i, modi(k,24), nper(k,24), id_1(k), ld_1(k) );
	}
      }
    }
  }


  if( i<=7 || modi(i,7)==1 )  {
    fprintf( iou,"Trial=%7ld             ChkSum=%5ld    Pass=%7ld     Fail=%7ld\n", i, isum, npass, nfail);
     printf( "Trial=%7ld             ChkSum=%5ld    Pass=%7ld     Fail=%7ld\n", i, isum, npass, nfail);
  }
  last= isum;
  ibuf= 0;

  track ("trial   ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_proof.h"
}


void valid( REAL vx[],long map[],long *lx,REAL bl,REAL x[],
           REAL bu,long n )
{
  /************************************************

          Compress valid data sets;  form compression list.


         VX    - ARRAY  OF RESULT COMPRESSED Xs.
         MAP   - ARRAY  OF RESULT COMPRESSION INDICES
         LX    -           RESULT COMPRESSED LENGTH OF VX, MAP
               -
         BL    -           INPUT LOWER BOUND FOR VX
         X     - ARRAY  OF INPUT VALUES.
         BU    -           INPUT UPPER BOUND FOR VX
         n     - NUMBER OF INPUT VALUES IN X.

    **********************************************/

#undef vx_1
#define vx_1(a1) vx[a1-1]
#undef x_1
#define x_1(a1) x[a1-1]
#undef map_1
#define map_1(a1) map[a1-1]

  long k, m;
  /*-----end-of-declarations-----*/
  trace ("valid   ");

  m= 0;
  *lx= 0;
  if( n<=0 )  goto L_73;
  for (k=1 ; k<=n ; k++) {
    if( x_1(k)> bl && x_1(k)< bu ) {
      m++;
      map_1(m)= k;
      vx_1(m)= x_1(k);
    }
  }

  *lx= m;
  if( m>0 )  {
    trap( map, " valid  " , 1, n,m);
  }
 L_73:
  track ("valid   ");
  return;
}


void values(long i)
{
  /************************************************

                VALUES     initializes special values

         i    :=  kernel number

    ***************************************************************************/
#define SET_VARS
#include "c_cernel.h"
#include "c_base2.h"
#include "c_space3.h"


  /*
         *******************************************************************/
  static double ds, dw;  /* double even for a float run */
  static REAL sc, fw;
  static long lx, ii, lr, mc, mmax, mmin;
  static long j, k, ip1;
  /*-----end-of-declarations-----*/
  trace ("values  ");

  sizes (i);
  ip1= i;
  /*              Initialize the dummy  Cache-memory with never used data-set.*/
  for (k=1 ; k<=8192 ; k++) {
    cache_1(k)= 0.10;
  }

  supply( i);

  if( ip1!=13 ) goto L_14;
  ds= 1.000e0;
  dw= 0.500e0;
  for (j=1 ; j<=4 ; j++) {
    for (k=1 ; k<=512 ; k++) {
      p_2(j,k)  = ds;
      p0_2(j,k) = ds;
      ds += dw;
    }
  }

  for (j=1 ; j<=96 ; j++) {
    e_1(j) = 1;
    f_1(j) = 1;
  }

 L_14: if( ip1!=14) goto L_16;

  mmin= 1;
  mmax= 1001;
  iqranf( ix, mmin, mmax, 1001);

  dw= -100.000e0;
  for (j=1 ; j<=1001 ; j++) {
    dex_1(j) =  dw*dex_1(j);
    grd_1(j) = ix_1(j);
  }
  flx= 0.00100e0;

 L_16: if( ip1!=16 ) goto L_50;
  /*CONDITIONS:*/
  mc= 2;
  lr= n;
  ii= lr/3;
  fw= 1.000e-4;
  d_1(1)= 1.0198048642876400e0;
  for (k=2 ; k<=300 ; k++) {
    d_1(k)= d_1(k-1) + fw/d_1(k-1);
  }
  r= d_1(lr);
  fw= 1.000e0;
  for (lx=1 ; lx<=mc ; lx++) {
    m= (lr+lr)*(lx-1);
    for (j=1 ; j<=2 ; j++) {
      for (k=1 ; k<=lr ; k++) {
	m= m+1;
	s= (REAL)k;
	plan_1(m)= r*((s + fw)/s);
	zone_1(m)= k+k;
      }
    }
  }
  k= lr+lr+1;
  zone_1(k)= lr;
  s= d_1(lr-1);
  t= d_1(lr-2);

 L_50:
  /*               Clear the scalar Cache-memory with never used data-set.
    */
  j= 0;
  sc= 0.0e0;
  for (k=1 ; k<=8192 ; k++) {
    if( cache_1(k)== 0.0)  {
      j += k;
      sc += (REAL)(j*k);
    }
  }

  track ("values  ");
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_base2.h"
#include "c_space3.h"
}

void verify( FILE *iou )
{
  /************************************************************************
                                                                          *
          VERIFY     auxiliary test routine to check-out function SECOND  *
                     and to verify that sufficiently long Loop sizes are  *
                     defined in Subr. SIZES for accurate CPU timing.      *
                                                                          *
           iou    -  Logical Output Device Number                         *
                                                                          *
    **********************************************************************/

#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"

#undef ntmp
#define ntmp 100

/* undef macros not used here */
#undef sig
#undef time

#undef ter_1
#define ter_1(a1) ter[a1-1]
#undef tav_1
#define tav_1(a1) tav[a1-1]
#undef tum_1
#define tum_1(a1) tum[a1-1]
#undef tim_1
#define tim_1(a1) tim[a1-1]
#undef sig_1
#define sig_1(a1) sig[a1-1]
#undef tmx_1
#define tmx_1(a1) tmx[a1-1]
#undef len_1
#define len_1(a1) len[a1-1]

  static REAL tim[ntmp], tum[ntmp], tav[ntmp], ter[ntmp];
  static REAL tmx[ntmp], sig[ntmp];
  static long len[ntmp];
  static REAL ratemf, td, flops;
  static REAL passes, task, count, rperr, rt, tnn, repeat;
  static REAL timer, rterr;
  static REAL ttest, time, fuzz, cum;
  static long loiter, loops0, loop12, i2, nn;
  static long j, i, lo, nj, ilimit, nticks, nzd, k;
  static double dt, t0, t1, t2;
  REAL rdum;
  /*-----end-of-declarations-----*/
  /*
         CALL TRACE ("VERIFY  ")
    */
  for (k=1 ; k<=101 ; k++) {
    x_1(k)= 0.0e0;
    y_1(k)= 0.0e0;
    cx_2(1,k)= 0.0e0;
  }
  nzd= 0;
  /*
    *********************************************************************
         Measure tsecov:  Overhead time for calling SECOND
    *********************************************************************
    */
  tsecov = secovt( iou, 1);
  tic = tsecov;
  /*
    *********************************************************************
         Measure time resolution of cpu-timer;  tclock= MIN t
    *********************************************************************
    */
  fuzz= 1.00e-12;
  nticks= (int)( 1.00e0/( tsecov + fuzz ));
  if( nticks<1000 ) nticks= 1000;
  dt= 0.0;
  cum= 0.0;
  t1= second( cum);
  m= 0;

  for (k=1 ; k<=nticks ; k++) {
    t2= second( cum);
    if( t2 != t1 ) {
      m++;
      dt += t2 - t1;
      t1= t2;
      if( m >= 200 ) goto L_3;
    }
  }

 L_3:
  if( m<=2 ) {
    tclock= 1.00e0;
    printf("WARNING(VERIFY): POOR Cpu-timer resolution; REPLACE?\n");
    fprintf( iou,"WARNING(VERIFY): POOR Cpu-timer resolution; REPLACE?\n");
  } else {
    tclock= dt/( (REAL)m + fuzz );
  }
  printf("VERIFY:%10ld%12.4e =  Time Resolution of Cpu-timer\n", m, tclock);
  fprintf( iou,"VERIFY:%10ld%12.4e =  Time Resolution of Cpu-timer\n", m, tclock);
  /*
    **************************************************************************
             VERIFY ADEQUATE Loop SIZE VERSUS CPU CLOCK ACCURACY
    **************************************************************************

             VERIFY produced the following output on CRAY-XMP4 in a
             fully loaded, multi-processing, multi-programming system:

             VERIFY ADEQUATE Loop SIZE VERSUS CPU CLOCK ACCURACY
             -----     -------     -------    -------   --------
             EXTRA     MAXIMUM     DIGITAL    DYNAMIC   RELATIVE
             Loop      CPUTIME     CLOCK      CLOCK     TIMING
             SIZE      SECONDS     ERROR      ERROR     ERROR
             -----     -------     -------    -------   --------
                 1  5.0000e-06      10.00%     17.63%     14.26%
                 2  7.0000e-06       7.14%      6.93%      4.79%
                 4  1.6000e-05       3.12%      6.56%      7.59%
                 8  2.8000e-05       1.79%      2.90%      2.35%
                16  6.1000e-05       0.82%      6.72%      4.50%
                32  1.1700e-04       0.43%      4.21%      4.62%
                64  2.2700e-04       0.22%      3.13%      2.41%
               128  4.4900e-04       0.11%      3.14%      0.96%
               256  8.8900e-04       0.06%      2.06%      2.50%
               512  1.7740e-03       0.03%      1.92%      1.59%
              1024  3.4780e-03       0.01%      0.70%      1.63%
              1360              Current Run:    MULTI=   10.000
              2048  7.0050e-03       0.01%      0.74%      1.28%
              4096  1.3823e-02       0.00%      1.35%      0.78%
             -----     -------     -------    -------   --------

              Approximate Serial Job Time=   2.5e+01 Sec.    ( Nruns= 7 RUNS)

    **************************************************************************
  */
  if(full_out) {
    fprintf( iou,"\n       VERIFY ADEQUATE Loop SIZE VERSUS CPU CLOCK ACCURACY\n");
    fprintf( iou,"       -----     -------     -------    -------   --------\n");
    fprintf( iou,"       EXTRA     MAXIMUM     DIGITAL    DYNAMIC   RELATIVE\n");
    fprintf( iou,"       Loop      CPUTIME     CLOCK      CLOCK     TIMING  \n");
    fprintf( iou,"       SIZE      SECONDS     ERROR      ERROR     ERROR   \n");
    fprintf( iou,"       -----     -------     -------    -------   --------\n");
  }
  /*
    **************************************************************************
         Measure Cpu Clock Timing Errors As A Function Of Loop Size(lo)
    **************************************************************************
    */
  ttest= 100.00e0 * tclock;
  ilimit= 30;
  nj= 5;
  lo= 128;
  i= 0;

 L_10:        i++;
  lo= lo + lo;
  for (j=1 ; j<=nj ; j++) {
    n= 100;
    cumtim_1(1)= 0.0e0;
    t0= second( cumtim_1(1));
    /*                                    Time Kernel 12*/
    rdum= kernel_12(lo,n,x);
    cumtim_1(1)= 0.0e0;
    tim_1(j)= second( cumtim_1(1)) - t0 - tsecov;
  }
  /*                                    Compute Dynamic Clock Error
    */
  stats( tum, tim, nj);
  rterr= 100.0*( tum_1(2)/( tum_1(1) + fuzz ));
  if( tum_1(1)<= 0.00e0)  rterr= 100.00e0;
  /*
                                        Compute Digital Clock Error
    */
  tdigit( &(sig_1(i)), &nzd, tum_1(4) );

  tav_1(i)= tum_1(1);
  tmx_1(i)= tum_1(4);
  ter_1(i)= rterr;
  len_1(i)= lo;
  if( i>ilimit && ( tum_1(1)<fuzz )) {
    printf("VERIFY:%12ld Repetitions.  Bad Timer=%14.5e sec.\n",  lo, tum_1(1) );
  }
  if( i<=8 ||  ( tum_1(1)<ttest && i<ntmp )) goto L_10;
  nn= i;
  /*
    *************************************************************************
         Compute Multiple-Pass Loop Counters MULTI and Loops2
         Such that:  each Kernel is run at least 100 ticks of Cpu-timer.
    *************************************************************************
    */
  i2= 2;
  multi= 1;
  mucho= 1;
  sizes(12);
  loop12= ipass_2(12,2) * mul_1(2);

  multi= (long) (lo/(loop12+fuzz)*(ttest/(tum_1(1)+fuzz)) );
  mucho= multi;
  /*
         If timing errors are too large, you must increase MULTI...
         When MULTI.GE.100 each kernel is executed over a million times
         and the time used to re-intialize overstored input variables
         is negligible.  Thus each kernel may be run arbitrarily many times
         (MULTI >> 100) without overflow and produce verifiable checksums.

         Each kernel"s results are automatically checksummed for  MULTI :=

         MULTI=   1      clock resolution << 0.01 SEC,  or Cpu << 1 Mflops
         MULTI=  10      clock resolution << 0.01 SEC,  or Cpu <  2 Mflops
         MULTI=  50      clock resolution <= 0.01 SEC,  or Cpu <  2 Mflops
         MULTI= 100      clock resolution <= 0.01 SEC,  or Cpu <  5 Mflops
         MULTI= 200      clock resolution <= 0.01 SEC,  or Cpu < 10 Mflops

         MULTI=   1
         MULTI=  10
         MULTI=  50
         MULTI= 100
         MULTI= 200
    */
  mpy= 1;
  loops2= 1;
  mpylim= loops2;
  if( multi<=  1 ) {
    multi =    1;
  } else if( multi<= 10 ) {
    multi =   10;
  } else if( multi<= 50 ) {
    multi =   50;
  } else if( multi<=100 ) {
    multi =  100;
  } else {
    loops2= (multi + 50)/100;
    mpylim= loops2;
    multi =  100;
  }

#ifdef FAST_RUN
  multi= 1;
#endif

  mucho= multi;
  loops0= loop12 * multi * loops2;
  repeat= (REAL) ( multi * loops2 );
  if( loop==1 ) repeat= 1.00e0/( (REAL)loop12 + fuzz);
  /*
    **************************************************************************
         Estimate Timing Error By Comparing Time Of Each Run With Longest Run
    **************************************************************************
    */
  m= 0;
  tnn= ( tav_1(nn) + 2.00e0* tav_1(nn-1))* 0.500e0;
  fuzz= 1.0e-12;
  if( tnn<fuzz)  tnn= fuzz;
  for (i=1 ; i<=nn ; i++) {
    rterr= ter_1(i);
    lo= len_1(i);
    /*                                    Compute Relative Clock Error
      */
    rt= 0.0e0;
    if( len_1(i)>= 0)     rt= len_1(nn)/len_1(i);
    rperr= 100.00e0;
    if( tnn>fuzz) rperr= 100.00e0*(abs( tnn - rt*tav_1(i))/tnn);
    if(full_out) {
      fprintf( iou,"     %7ld%12.4e%11.2f%%%10.2f%%%10.2f%%\n", lo, tmx_1(i), sig_1(i),rterr, rperr);
	}
	/*                Find loops0 Size Used
      */
    if( (loops0>=lo) && (loops0<=2*lo))  {
      m= lo;
      if(full_out) {
        fprintf( iou,"      %6ld       Repetition Count = MULTI * Loops2 = %12.3f\n",  loops0, repeat);
      }
	  printf("      %6ld       Repetition Count = MULTI * Loops2 = %12.3f\n",  loops0, repeat);
      if( rterr > 10.00e0)  {
        if(full_out) {
          fprintf( iou, "                                 VERIFY: POOR TIMING OR ERROR. NEED LONGER RUN\n");
          fprintf( iou, "                                 INCREASE:   MULTI  IN SUBR. VERIFY\n");
        }
		printf( "                                 VERIFY: POOR TIMING OR ERROR. NEED LONGER RUN\n");
        printf("                                 INCREASE:   MULTI  IN SUBR. VERIFY\n");
      }
    }

  }
  if( m<=0 )  {
    if(full_out) {
      fprintf( iou,"      %6ld       Repetition Count = MULTI * Loops2 = %12.3f\n",  loops0, repeat);
    }
	printf( "      %6ld       Repetition Count = MULTI * Loops2 = %12.3f\n",  loops0, repeat);
  }
  /*
    **************************************************************************
         Clock Calibration Test of Internal Cpu-timer SECOND;
               Verify 10 Internal SECOND Intervals using External Stopwatch
    **************************************************************************
    */
  fprintf( iou,"       -----     -------     -------    -------   --------\n");
  fprintf( iou,"\n\nCLOCK CALIBRATION TEST OF INTERNAL CPU-TIMER: SECOND\n");
  fprintf( iou,"MONOPROCESS THIS TEST, STANDALONE, NO TIMESHARING.\n");
  fprintf( iou,"VERIFY TIMED INTERVALS SHOWN BELOW USING EXTERNAL CLOCK\n");
  fprintf( iou,"START YOUR STOPWATCH NOW !\n");
  fprintf( iou,"\n          Verify  T or DT  observe external clock:\n\n");
  fprintf( iou,"          -------     -------      ------      -----\n");
  fprintf( iou,"          Total T ?   Delta T ?    Mflops ?    Flops\n");
  fprintf( iou,"          -------     -------      ------      -----\n");
  printf("\n\nCLOCK CALIBRATION TEST OF INTERNAL CPU-TIMER: SECOND\n");
  printf("MONOPROCESS THIS TEST, STANDALONE, NO TIMESHARING.\n");
  printf("VERIFY TIMED INTERVALS SHOWN BELOW USING EXTERNAL CLOCK\n");
  printf("START YOUR STOPWATCH NOW !\n");
  printf("\n          Verify  T or DT  observe external clock:\n\n");
  printf("          -------     -------      ------      -----\n");
  printf("          Total T ?   Delta T ?    Mflops ?    Flops\n");
  printf("          -------     -------      ------      -----\n");
  task= 10.00e0;
  passes= lo * ( task/( tnn + fuzz));
  loiter= (long) passes;
  flops= 0.00e0;
  cumtim_1(1)= 0.0e0;
  t1= second( cumtim_1(1));
  t2= 0.00e0;

  for (j=1 ; j<=4 ; j++) {
    n= 100;
    t0= t1;
    /*                                    Time Kernel 12*/
    rdum= kernel_12(loiter,n,x);

    cumtim_1(1)= 0.0e0;
    t1= second( cumtim_1(1));
    td= t1 - t0 -tsecov;
    t2 += td;
    flops += passes*n;
    ratemf= ( 1.00e-6 * flops )/( t2 + fuzz );
    printf("   %2ld%12.2f%12.2f%12.2f%15.5e\n",  j, t2, td, ratemf, flops);
    fprintf( iou,"   %2ld%12.2f%12.2f%12.2f%15.5e\n",  j, t2, td, ratemf, flops);
  }
  fprintf( iou,"          -------     -------      ------      -----\n");
  fprintf( iou,"END CALIBRATION TEST.\n\n");
  printf("          -------     -------      ------      -----\n");
  printf("END CALIBRATION TEST.\n\n");
  /*     CALL TRACK ("VERIFY  ")*/
  return;
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
}


void watch( long mode)
{
  /************************************************

      WATCH is called at every subroutine entry and exit point by TRACE .
      COMMON variables may be tested continually during execution(watched)
      for known error conditions so the occurance of the error is localized.
      WATCH may be used for programmable data-breakpoints to aid debugging.

    **********************************************/

#define SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_debug.h"
#include "c_order.h"

#undef ntests
#define ntests 14
#undef krs1
#define krs1 (24 + 1)
#undef ie_1
#define ie_1(a1) ie[a1-1]

  /*     LOGICAL BOUNDS
         BOUNDS(A,X,B,E)= ((((A)*(1.-E)).LE.(X)).AND.((X).LE.((B)*(1.+E))))

                                           Debug Trace Info*/
  static long ie[20];
  static char *name, *fmt111, *fmt112;
  static long k1, ierr, k;
  FILE *io;
  /*-----end-of-declarations-----*/
  name= "watch";
  /*                                       Domain Tests of Critical Variables*/
  for (k=1 ; k<=ntests ; k++) {
    ie_1(k)= 0;
  }
  if( testov != ticks ) ie_1(1)= 1;
  if( tsecov != tic ) ie_1(2)= 2;
  if( inseq<=0 || inseq!=isave || inseq>99999) ie_1(3)= 3;
  if( nruns<1 || nruns>8 ) ie_1(4)= 4;
  if(    il<1 || il>3    ) ie_1(5)= 5;
  if(    mk<1 || mk>24   ) ie_1(6)= 6;
  if(    ik<0 || ik>krs1  ) ie_1(7)= 7;
  if(    jr<1 || jr>8    ) ie_1(8)= 8;
  if(    loops2  < 1          ) ie_1(9)= 9;
  if(    loops2  != mpylim     ) ie_1(10)= 10;
  if(    multi   < 1          ) ie_1(11)= 11;
  if(    multi   != mucho      ) ie_1(12)= 12;
  if(    loop    < 1          ) ie_1(13)= 13;
  if(    loop    != lp         ) ie_1(14)= 14;
  /*
                            Insert your debug data tests here
    */
  ierr= 0;
  for (k=1 ; k<=ntests ; k++) {
    ierr += ie_1(k);
  }
  if( ierr!=0 ) {
    if( ion_flag <= 0 ) {
      io= stdout;
    } else {
      io= ion;
    }
    k1=0;
    k2=0;
    fmt111= "\nWATCH: STORAGE FAULT DETECTED.  IE=\n";
    fmt112= "%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld";
    printf(fmt111);
    for(k=1; k <= ntests; k++) {
      printf(fmt112, k);
      if((k%15) == 0) printf("\n");
    }
    printf(fmt112, k1,k2,inseq,nruns,il,mk,ik,jr,loops2,mpylim,multi,
           mucho,loop,lp);
    printf(fmt111);
    for(k=1; k <= ntests; k++) {
      printf(fmt112, k);
      if((k%15) == 0) printf("\n");
    }
    for(k=1; k <= ntests; k++) {
      printf(fmt112, ie_1(k));
      if((k%15) == 0) printf("\n");
    }
    printf(fmt112, k1,k2,inseq,nruns,il,mk,ik,jr,loops2,mpylim,multi,
           mucho,loop,lp);
    where( mode);
  }
#undef SET_VARS
#include "c_cernel.h"
#include "c_tau.h"
#include "c_debug.h"
#include "c_order.h"
}


void where( long mode)
{
  /************************************************

      Prints Subroutine names in the active linkage chain for debugging.

    **********************************************/

#define SET_VARS
#include "c_cernel.h"
#include "c_debug.h"
#include "c_order.h"

#undef insert
#define insert 2

  char *name;
  long k, made;
  FILE *io;
  /*-----end-of-declarations-----*/
  made= mod( mode,10);
  name= "internal";
  if( made==1 ) name= " entry  ";
  if( made==2 ) name= " return ";
  if( ion_flag <= 0 ) {
    io= stdout;
  } else {
    io= ion;
  }

  if( mode==12 ) {
    printf(" WHERE: SEQ.ERROR.  RETURN %8s .NE. CALL %8s\n",
           istack_1(20), istack_1(1) );
    fprintf(io," WHERE: SEQ.ERROR.  RETURN %8s .NE. CALL %8s\n",
            istack_1(20), istack_1(1) );
  }
  printf("\nWHERE:  ERROR detected at %s point in: %8s\n",
         name, istack_1(1) );
  fprintf(io,"\nWHERE:  ERROR detected at %s point in: %8s\n",
          name, istack_1(1) );
  if( made==1 || made==2 )  {
    /*                    Pushdown stack of subroutine names and call nrs.*/
    for (k=12 ; k>=insert+1 ; k--) {
      nstack_1(k)= nstack_1(k-insert);
      str8cpy(istack_1(k), istack_1(k-insert));
    }

    nstack_1(1)= inseq;
    str8cpy(istack_1(1), "watch   ");
    nstack_1(2)= inseq;
    str8cpy(istack_1(2), "trace   ");
    if( made==2 )  str8cpy(istack_1(2), "track   ");
  }
  printf("\nACTIVE SUBROUTINE LINKAGE CHAIN:");
  printf("          ----           -----------");
  printf("          name           call number");
  printf("          ----           -----------");
  for(k= 1; k <= 12; k++) {
    printf("         %8s    %8ld\n", istack_1(k), nstack_1(k) );
  }

  fprintf( io,"\nACTIVE SUBROUTINE LINKAGE CHAIN:");
  fprintf( io,"          ----           -----------");
  fprintf( io,"          name           call number");
  fprintf( io,"          ----           -----------");
  for(k= 1; k <= 12; k++) {
    fprintf(io,"         %8s    %8ld\n", istack_1(k), nstack_1(k));
  }
  for (k=1 ; k<=200 ; k++) {
    fprintf( io, "\n********* TERMINAL ERROR; FLUSH I/O BUFFER **********\n");
  }
#undef SET_VARS
#include "c_cernel.h"
#include "c_debug.h"
#include "c_order.h"
}
