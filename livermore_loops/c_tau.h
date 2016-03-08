/*
      COMMON /TAU/   tclock, tsecov, testov, cumtim(4)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_TAU
typedef struct {
  REAL Tclock;
  REAL Tsecov;
  REAL Testov;
  REAL Cumtim[4];
} block_tau ;
extern block_tau TAU;
#define BLOCK_TAU
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_TAU_STORE
block_tau TAU;
#define BLOCK_TAU_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define tclock TAU.Tclock
#define tsecov TAU.Tsecov
#define testov TAU.Testov
#define cumtim TAU.Cumtim

#undef cumtim_1
#define cumtim_1(a1) TAU.Cumtim[a1-1]

#else

/* clear macros for variable names */
#undef tclock
#undef tsecov
#undef testov
#undef cumtim
#undef cumtim_1
#endif

