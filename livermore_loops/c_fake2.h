/*
      COMMON /FAKE2/ tcum(20)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_FAKE2
typedef struct {
  REAL Tcum[20];
} block_fake2 ;
extern block_fake2 FAKE2;
#define BLOCK_FAKE2
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_FAKE2_STORE
block_fake2 FAKE2;
#define BLOCK_FAKE2_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define tcum FAKE2.Tcum

#undef tcum_1
#define tcum_1(a1) tcum[a1-1]

#else

/* clear macros for variable names */
#undef tcum
#undef tcum_1
#endif
