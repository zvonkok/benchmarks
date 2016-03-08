/* header file for the double precision ANSI C version of
   the Livermore Fortran kernels benchmark

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.
   */

/* choose between float and real versions
#define REAL double
#define REAL float
*/
#define REAL double

/* Defining USE_FANCY_CLOCK builds a version with a timer that is very 
   accurate on single processor Windows 95/98/NT computers, but it 
   gives incorrect answers on multi-processor NT systems.
#define USE_FANCY_CLOCK
#undef USE_FANCY_CLOCK
*/
#define USE_FANCY_CLOCK

/* define FAST_RUN to force multi to be 1 - times will
   be very inaccurate, but the run will be quicker */
#undef FAST_RUN
