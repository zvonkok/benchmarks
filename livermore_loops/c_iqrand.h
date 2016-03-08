/*
      COMMON /IQRAND/ k0, k, k9

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_IQRAND
typedef struct {
  REAL K0;
  REAL K;
  REAL K9;
} block_iqrand ;
extern block_iqrand IQRAND;
#define BLOCK_IQRAND
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_IQRAND_STORE
block_iqrand IQRAND;
#define BLOCK_IQRAND_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define k0 IQRAND.K0
#define k IQRAND.K
#define k9 IQRAND.K9

#else

/* clear macros for variable names */
#undef k0
#undef k
#undef k9
#endif
