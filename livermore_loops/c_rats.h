/*
  common /rats/  rated(11,4)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_RATS
typedef struct {
  REAL Rated[4][11];
} block_rats ;
extern block_rats RATS;
#define BLOCK_RATS
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_RATS_STORE
block_rats RATS;
#define BLOCK_RATS_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define rated RATS.Rated

#undef rated_2
#define rated_2(a1,a2) rated[a2-1][a1-1]

#else

/* clear macros for variable names */
#undef rated
#undef rated_2
#endif
