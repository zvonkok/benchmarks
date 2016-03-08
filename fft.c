/***************************************************************/
/* Timer options. You MUST uncomment one of the options below  */
/* or compile, for example, with the '-DUNIX' option.          */
/* FOR EXAMPLE:                                                */
/* gcc -DUNIX   -O2 fft.c -o fft                               */
/* gcc -DGTODay -O2 fft.c -o fft                               */
/* gcc -DCTimer -O2 fft.c -o fft                               */
/* These should all give the same timing result on UCB BSD 4.3 */
/* systems and on some SVR4 systems (e.g., SunOS 5.0)          */
/***************************************************************/
/* #define Amiga       */
/* #define UNIX        */
/* #define UNIX_Old    */
/* #define VMS         */
/* #define BORLAND_C   */
/* #define MSC         */
/* #define MAC         */
/* #define IPSC        */
/* #define FORTRAN_SEC */
/* #define GTODay      */
/* #define CTimer      */
/* #define UXPM        */
/* #define MAC_TMgr    */
/* #define PARIX       */
/* #define POSIX       */
/* #define WIN32       */
/* #define POSIX1      */
/***********************/

#include <stdio.h>
#include <math.h>
#include <bench.h>

#define REAL TYPE

REAL real[262145];
REAL imag[262145];

void fft_(void)
{
	long num=0,i,j;
	long status;
	REAL ssq=0.0;
	REAL scale;
	REAL starttime, runtime;

#define MSC
	double dtime();

	num = WORKLOAD;
	
	if (num > 262144)
	{
		printf("\n");
		printf("Error:\n");
		printf("Number of Points %d exceeds alloted array size\n\n",num);
		return;
	}

	for (i=0;i<num;i++)
	{
		real[i] = (REAL)i;
		imag[i] = 0.0;
	}

	scale = 1.0 / (REAL)num;

	printf("\n");
	printf("FFT-Mayer: 04 Oct 1994\n");
	printf("FFT  Size: %6d\n",num);

	fft(num, real, imag);
	ifft(num, real, imag);
	for (j=0;j<num*2;j++)
	{
		real[j]*=scale;
		imag[j]*=scale;
	}

	starttime = dtime();
	fft(num, real, imag);
	ifft(num, real, imag);
	runtime   = dtime() - starttime;
	for (j=0;j<num*2;j++)
	{
		real[j]*=scale;
		imag[j]*=scale;
	}

	runtime = runtime / 2.0;
	printf("Run Time (sec) = %11.5f\n",runtime);

	for (ssq=0,i=0;i<num;i++)
		ssq+=(real[i]-(REAL)i)*(real[i]-(REAL)i);
	printf("ssq errors %#.2g\n\n",ssq);

}


/*
** FFT and FHT routines
**  Copyright 1988, 1993; Ron Mayer
**  
**  fht(fz,n);
**      Does a hartley transform of "n" points in the array "fz".
**  fft(n,real,imag)
**      Does a fourier transform of "n" points of the "real" and
**      "imag" arrays.
**  ifft(n,real,imag)
**      Does an inverse fourier transform of "n" points of the "real"
**      and "imag" arrays.
**  realfft(n,real)
**      Does a real-valued fourier transform of "n" points of the
**      "real" and "imag" arrays.  The real part of the transform ends
**      up in the first half of the array and the imaginary part of the
**      transform ends up in the second half of the array.
**  realifft(n,real)
**      The inverse of the realfft() routine above.
**      
**      
** NOTE: This routine uses at least 2 patented algorithms, and may be
**       under the restrictions of a bunch of different organizations.
**       Although I wrote it completely myself; it is kind of a derivative
**       of a routine I once authored and released under the GPL, so it
**       may fall under the free software foundation's restrictions;
**       it was worked on as a Stanford Univ project, so they claim
**       some rights to it; it was further optimized at work here, so
**       I think this company claims parts of it.  The patents are
**       held by R. Bracewell (the FHT algorithm) and O. Buneman (the
**       trig generator), both at Stanford Univ.
**       If it were up to me, I'd say go do whatever you want with it;
**       but it would be polite to give credit to the following people
**       if you use this anywhere:
**           Euler     - probable inventor of the fourier transform.
**           Gauss     - probable inventor of the FFT.
**           Hartley   - probable inventor of the hartley transform.
**           Buneman   - for a really cool trig generator
**           Mayer(me) - for authoring this particular version and
**                       including all the optimizations in one package.
**       Thanks,
**       Ron Mayer; mayer@acuson.com
**
*/

#define GOOD_TRIG
#include "trigtbl.h"
char fht_version[] = "Brcwl-Hrtly-Ron-dbld";

#define SQRT2_2   0.70710678118654752440084436210484
#define SQRT2   2*0.70710678118654752440084436210484

fht(fz,n)
int n;
REAL *fz;
{
	REAL a,b;
	REAL c1,s1,s2,c2,s3,c3,s4,c4;
	REAL f0,g0,f1,g1,f2,g2,f3,g3;
	int i,k,k1,k2,k3,k4,kx;
	REAL *fi,*fn,*gi;
	TRIG_VARS;

	for (k1=1,k2=0;k1<n;k1++)
	{
		REAL a;
		for (k=n>>1; (!((k2^=k)&k)); k>>=1)
			;
		if (k1>k2)
		{
			a=fz[k1];
			fz[k1]=fz[k2];
			fz[k2]=a;
		}
	}
	for ( k=0 ; (1<<k)<n ; k++ )
		;
	k  &= 1;
	if (k==0)
	{
		for (fi=fz,fn=fz+n;fi<fn;fi+=4)
		{
			REAL f0,f1,f2,f3;
			f1     = fi[0 ]-fi[1 ];
			f0     = fi[0 ]+fi[1 ];
			f3     = fi[2 ]-fi[3 ];
			f2     = fi[2 ]+fi[3 ];
			fi[2 ] = (f0-f2);
			fi[0 ] = (f0+f2);
			fi[3 ] = (f1-f3);
			fi[1 ] = (f1+f3);
		}
	}
	else
	{
		for (fi=fz,fn=fz+n,gi=fi+1;fi<fn;fi+=8,gi+=8)
		{
			REAL s1,c1,s2,c2,s3,c3,s4,c4,g0,f0,f1,g1,f2,g2,f3,g3;
			c1     = fi[0 ] - gi[0 ];
			s1     = fi[0 ] + gi[0 ];
			c2     = fi[2 ] - gi[2 ];
			s2     = fi[2 ] + gi[2 ];
			c3     = fi[4 ] - gi[4 ];
			s3     = fi[4 ] + gi[4 ];
			c4     = fi[6 ] - gi[6 ];
			s4     = fi[6 ] + gi[6 ];
			f1     = (s1 - s2);
			f0     = (s1 + s2);
			g1     = (c1 - c2);
			g0     = (c1 + c2);
			f3     = (s3 - s4);
			f2     = (s3 + s4);
			g3     = SQRT2*c4;
			g2     = SQRT2*c3;
			fi[4 ] = f0 - f2;
			fi[0 ] = f0 + f2;
			fi[6 ] = f1 - f3;
			fi[2 ] = f1 + f3;
			gi[4 ] = g0 - g2;
			gi[0 ] = g0 + g2;
			gi[6 ] = g1 - g3;
			gi[2 ] = g1 + g3;
		}
	}
	if (n<16)
		return;

	do
	{
		REAL s1,c1;
		k  += 2;
		k1  = 1  << k;
		k2  = k1 << 1;
		k4  = k2 << 1;
		k3  = k2 + k1;
		kx  = k1 >> 1;
		fi  = fz;
		gi  = fi + kx;
		fn  = fz + n;
		do
		{
			REAL g0,f0,f1,g1,f2,g2,f3,g3;
			f1      = fi[0 ] - fi[k1];
			f0      = fi[0 ] + fi[k1];
			f3      = fi[k2] - fi[k3];
			f2      = fi[k2] + fi[k3];
			fi[k2]  = f0         - f2;
			fi[0 ]  = f0         + f2;
			fi[k3]  = f1         - f3;
			fi[k1]  = f1         + f3;
			g1      = gi[0 ] - gi[k1];
			g0      = gi[0 ] + gi[k1];
			g3      = SQRT2  * gi[k3];
			g2      = SQRT2  * gi[k2];
			gi[k2]  = g0         - g2;
			gi[0 ]  = g0         + g2;
			gi[k3]  = g1         - g3;
			gi[k1]  = g1         + g3;
			gi     += k4;
			fi     += k4;
		}
		while (fi<fn);
		TRIG_INIT(k,c1,s1);
		for (i=1;i<kx;i++)
		{
			REAL c2,s2;
			TRIG_NEXT(k,c1,s1);
			c2 = c1*c1 - s1*s1;
			s2 = 2*(c1*s1);
			fn = fz + n;
			fi = fz +i;
			gi = fz +k1-i;
			do
			{
				REAL a,b,g0,f0,f1,g1,f2,g2,f3,g3;
				b       = s2*fi[k1] - c2*gi[k1];
				a       = c2*fi[k1] + s2*gi[k1];
				f1      = fi[0 ]    - a;
				f0      = fi[0 ]    + a;
				g1      = gi[0 ]    - b;
				g0      = gi[0 ]    + b;
				b       = s2*fi[k3] - c2*gi[k3];
				a       = c2*fi[k3] + s2*gi[k3];
				f3      = fi[k2]    - a;
				f2      = fi[k2]    + a;
				g3      = gi[k2]    - b;
				g2      = gi[k2]    + b;
				b       = s1*f2     - c1*g3;
				a       = c1*f2     + s1*g3;
				fi[k2]  = f0        - a;
				fi[0 ]  = f0        + a;
				gi[k3]  = g1        - b;
				gi[k1]  = g1        + b;
				b       = c1*g2     - s1*f3;
				a       = s1*g2     + c1*f3;
				gi[k2]  = g0        - a;
				gi[0 ]  = g0        + a;
				fi[k3]  = f1        - b;
				fi[k1]  = f1        + b;
				gi     += k4;
				fi     += k4;
			}
			while (fi<fn);
		}
		TRIG_RESET(k,c1,s1);
	}
	while (k4<n);
}


ifft(n,real,imag)
int n;
double *real,*imag;
{
	double a,b,c,d;
	double q,r,s,t;
	int i,j,k;
	fht(real,n);
	fht(imag,n);
	for (i=1,j=n-1,k=n/2;i<k;i++,j--)
	{
		a = real[i];
		b = real[j];
		q=a+b;
		r=a-b;
		c = imag[i];
		d = imag[j];
		s=c+d;
		t=c-d;
		imag[i] = (s+r)*0.5;
		imag[j] = (s-r)*0.5;
		real[i] = (q-t)*0.5;
		real[j] = (q+t)*0.5;
	}
}

realfft(n,real)
int n;
double *real;
{
	double a,b,c,d;
	int i,j,k;
	fht(real,n);
	for (i=1,j=n-1,k=n/2;i<k;i++,j--)
	{
		a = real[i];
		b = real[j];
		real[j] = (a-b)*0.5;
		real[i] = (a+b)*0.5;
	}
}

fft(n,real,imag)
int n;
double *real,*imag;
{
	double a,b,c,d;
	double q,r,s,t;
	int i,j,k;
	for (i=1,j=n-1,k=n/2;i<k;i++,j--)
	{
		a = real[i];
		b = real[j];
		q=a+b;
		r=a-b;
		c = imag[i];
		d = imag[j];
		s=c+d;
		t=c-d;
		real[i] = (q+t)*.5;
		real[j] = (q-t)*.5;
		imag[i] = (s-r)*.5;
		imag[j] = (s+r)*.5;
	}
	fht(real,n);
	fht(imag,n);
}

realifft(n,real)
int n;
double *real;
{
	double a,b,c,d;
	int i,j,k;
	for (i=1,j=n-1,k=n/2;i<k;i++,j--)
	{
		a = real[i];
		b = real[j];
		real[j] = (a-b);
		real[i] = (a+b);
	}
	fht(real,n);
}

/*****************************************************/
/* Various timer routines.                           */
/* Al Aburto, aburto@nosc.mil, 18 Feb 1997           */
/*                                                   */
/* t = dtime() outputs the current time in seconds.  */
/* Use CAUTION as some of these routines will mess   */
/* up when timing across the hour mark!!!            */
/*                                                   */
/* For timing I use the 'user' time whenever         */
/* possible. Using 'user+sys' time is a separate     */
/* issue.                                            */
/*                                                   */
/* Example Usage:                                    */
/* [timer options added here]                        */
/* main()                                            */
/* {                                                 */
/* double starttime,benchtime,dtime();               */
/*                                                   */
/* starttime = dtime();                              */
/* [routine to time]                                 */
/* benchtime = dtime() - starttime;                  */
/* }                                                 */
/*                                                   */
/* [timer code below added here]                     */
/*****************************************************/

/*********************************/
/* Timer code.                   */
/*********************************/
/*******************/
/*  Amiga dtime()  */
/*******************/
#ifdef Amiga
#include <ctype.h>
#define HZ 50

double dtime()
{
	double q;

	struct tt
	{
		long  days;
		long  minutes;
		long  ticks;
	}
	tt;

	DateStamp(&tt);

	q = ((double)(tt.ticks + (tt.minutes * 60L * 50L))) / (double)HZ;

	return q;
}
#endif

/*****************************************************/
/*  UNIX dtime(). This is the preferred UNIX timer.  */
/*  Provided by: Markku Kolkka, mk59200@cc.tut.fi    */
/*  HP-UX Addition by: Bo Thide', bt@irfu.se         */
/*****************************************************/
#ifdef UNIX
#include <sys/time.h>
#include <sys/resource.h>

#ifdef hpux
#include <sys/syscall.h>
#define getrusage(a,b) syscall(SYS_getrusage,a,b)
#endif

struct rusage rusage;

double dtime()
{
	double q;

	getrusage(RUSAGE_SELF,&rusage);

	q = (double)(rusage.ru_utime.tv_sec);
	q = q + (double)(rusage.ru_utime.tv_usec) * 1.0e-06;

	return q;
}
#endif

/***************************************************/
/*  UNIX_Old dtime(). This is the old UNIX timer.  */
/*  Make sure HZ is properly defined in param.h !! */
/***************************************************/
#ifdef UNIX_Old
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifndef HZ
#define HZ 60
#endif

struct tms tms;

double dtime()
{
	double q;

	times(&tms);

	q = (double)(tms.tms_utime) / (double)HZ;

	return q;
}
#endif

/*********************************************************/
/*  VMS dtime() for VMS systems.                         */
/*  Provided by: RAMO@uvphys.phys.UVic.CA                */
/*  Some people have run into problems with this timer.  */
/*********************************************************/
#ifdef VMS
#include time

#ifndef HZ
#define HZ 100
#endif

struct tbuffer_t
{
	int proc_user_time;
	int proc_system_time;
	int child_user_time;
	int child_system_time;
};

struct tbuffer_t tms;

double dtime()
{
	double q;

	times(&tms);

	q = (double)(tms.proc_user_time) / (double)HZ;

	return q;
}
#endif

/******************************/
/*  BORLAND C dtime() for DOS */
/******************************/
#ifdef BORLAND_C
#include <ctype.h>
#include <dos.h>
#include <time.h>

#define HZ 100
struct time tnow;

double dtime()
{
	double q;

	gettime(&tnow);

	q = 60.0 * (double)(tnow.ti_min);
	q = q + (double)(tnow.ti_sec);
	q = q + (double)(tnow.ti_hund)/(double)HZ;

	return q;
}
#endif

/**************************************/
/*  Microsoft C (MSC) dtime() for DOS */
/**************************************/
#ifdef MSC
#include <time.h>
#include <ctype.h>

#define HZ CLOCKS_PER_SEC
clock_t tnow;

double dtime()
{
	double q;

	tnow = clock();

	q = (double)tnow / (double)HZ;

	return q;
}
#endif

/*************************************/
/*  Macintosh (MAC) Think C dtime()  */
/*************************************/
#ifdef MAC
#include <time.h>

#define HZ 60

double dtime()
{
	double q;

	q = (double)clock() / (double)HZ;

	return q;
}
#endif

/************************************************************/
/*  iPSC/860 (IPSC) dtime() for i860.                       */
/*  Provided by: Dan Yergeau, yergeau@gloworm.Stanford.EDU  */
/************************************************************/
#ifdef IPSC
extern double dclock();

double dtime()
{
	double q;

	q = dclock();

	return q;
}
#endif

/**************************************************/
/*  FORTRAN dtime() for Cray type systems.        */
/*  This is the preferred timer for Cray systems. */
/**************************************************/
#ifdef FORTRAN_SEC

fortran double second();

double dtime()
{
	double q;

	second(&q);

	return q;
}
#endif

/***********************************************************/
/*  UNICOS C dtime() for Cray UNICOS systems.  Don't use   */
/*  unless absolutely necessary as returned time includes  */
/*  'user+system' time.  Provided by: R. Mike Dority,      */
/*  dority@craysea.cray.com                                */
/***********************************************************/
#ifdef CTimer
#include <time.h>

double dtime()
{
	double    q;
	clock_t   clock(void);

	q = (double)clock() / (double)CLOCKS_PER_SEC;

	return q;
}
#endif

/********************************************/
/* Another UNIX timer using gettimeofday(). */
/* However, getrusage() is preferred.       */
/********************************************/
#ifdef GTODay
#include <sys/time.h>

struct timeval tnow;

double dtime()
{
	double q;

	gettimeofday(&tnow,NULL);
	q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

	return q;
}
#endif

/*****************************************************/
/*  Fujitsu UXP/M timer.                             */
/*  Provided by: Mathew Lim, ANUSF, M.Lim@anu.edu.au */
/*****************************************************/
#ifdef UXPM
#include <sys/types.h>
#include <sys/timesu.h>
struct tmsu rusage;

double dtime()
{
	double q;

	timesu(&rusage);

	q = (double)(rusage.tms_utime) * 1.0e-06;

	return q;
}
#endif

/**********************************************/
/*    Macintosh (MAC_TMgr) Think C dtime()    */
/*   requires Think C Language Extensions or  */
/*    #include <MacHeaders> in the prefix     */
/*  provided by Francis H Schiffer 3rd (fhs)  */
/*         skipschiffer@genie.geis.com        */
/**********************************************/
#ifdef MAC_TMgr
#include <Timer.h>
#include <stdlib.h>

static TMTask   mgrTimer;
static Boolean  mgrInited = false;
static double   mgrClock;

#define RMV_TIMER RmvTime( (QElemPtr)&mgrTimer )
#define MAX_TIME  1800000000L
/* MAX_TIME limits time between calls to */
/* dtime( ) to no more than 30 minutes   */
/* this limitation could be removed by   */
/* creating a completion routine to sum  */
/* 30 minute segments (fhs 1994 feb 9)   */

static void Remove_timer( )
{
	RMV_TIMER;
	mgrInited = false;
}

double  dtime( )
{
	if( mgrInited )
	{
		RMV_TIMER;
		mgrClock += (MAX_TIME + mgrTimer.tmCount)*1.0e-6;
	}
	else
	{
		if( _atexit( &Remove_timer ) == 0 )
			mgrInited = true;
		mgrClock = 0.0;
	}

	if ( mgrInited )
	{
		mgrTimer.tmAddr = NULL;
		mgrTimer.tmCount = 0;
		mgrTimer.tmWakeUp = 0;
		mgrTimer.tmReserved = 0;
		InsTime( (QElemPtr)&mgrTimer );
		PrimeTime( (QElemPtr)&mgrTimer, -MAX_TIME );
	}
	return( mgrClock );
}
#endif

/***********************************************************/
/*  Parsytec GCel timer.                                   */
/*  Provided by: Georg Wambach, gw@informatik.uni-koeln.de */
/***********************************************************/
#ifdef PARIX
#include <sys/time.h>

double dtime()
{
	double q;

	q = (double) (TimeNowHigh()) / (double) CLK_TCK_HIGH;

	return q;
}
#endif

/************************************************/
/*  Sun Solaris POSIX dtime() routine           */
/*  Provided by: Case Larsen, CTLarsen.lbl.gov  */
/************************************************/
#ifdef POSIX
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/rusage.h>

#ifdef __hpux
#include <sys/syscall.h>
#endif

struct rusage rusage;

double dtime()
{
	double q;

	getrusage(RUSAGE_SELF,&rusage);

	q = (double)(rusage.ru_utime.tv_sec);
	q = q + (double)(rusage.ru_utime.tv_nsec) * 1.0e-09;

	return q;
}
#endif


/****************************************************/
/*  Windows NT (32 bit) dtime() routine             */
/*  Provided by: Piers Haken, piersh@microsoft.com  */
/****************************************************/
#ifdef WIN32
#include <windows.h>

double dtime(void)
{
	double q;

	q = (double)GetTickCount() * 1.0e-03;

	return q;
}
#endif

/*****************************************************/
/* Time according to POSIX.1  -  <J.Pelan@qub.ac.uk> */
/* Ref: "POSIX Programmer's Guide"  O'Reilly & Assoc.*/
/*****************************************************/
#ifdef POSIX1
#define _POSIX_SOURCE 1
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>

struct tms tms;

double dtime()
{
	double q;
	times(&tms);
	q = (double)tms.tms_utime / (double)CLK_TCK;
	return q;
}
#endif

/* ---------- Say goodnight Carol! 02 Mar 1997 ---------- */

SINGLE_BENCHMARK(fft_())

