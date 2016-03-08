/*
      COMMON /PROOF/  SUMS(24,3,8)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

/* NOTE: Sums must be double precision independent of 
   whether this is a single or double precision run */

#ifndef BLOCK_PROOF
typedef struct {
    double Sums[8][3][24];
} block_proof ;
extern block_proof PROOF;
#define BLOCK_PROOF
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_PROOF_STORE
block_proof PROOF;
#define BLOCK_PROOF_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define sums PROOF.Sums

#undef sums_3
#define sums_3(a1,a2,a3) sums[a3-1][a2-1][a1-1]

#else

/* clear macros for variable names */
#undef sums
#undef sums_3
#endif
