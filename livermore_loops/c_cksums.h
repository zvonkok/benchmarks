/*
      COMMON /CKSUMS/ cksumu,ckoldu, cksump,ckoldp, cksuma,ckolda

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_CKSUMS
typedef struct {
    double Cksumu;
    double Ckoldu;
    double Cksump;
    double Ckoldp;
    double Cksuma;
    double Ckolda;
} block_cksums ;
extern block_cksums CKSUMS;
#define BLOCK_CKSUMS
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_CKSUMS_STORE
block_cksums CKSUMS;
#define BLOCK_CKSUMS_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define cksumu CKSUMS.Cksumu
#define ckoldu CKSUMS.Ckoldu
#define cksump CKSUMS.Cksump
#define ckoldp CKSUMS.Ckoldp
#define cksuma CKSUMS.Cksuma
#define ckolda CKSUMS.Ckolda

#else

/* clear macros for variable names */
#undef cksumu
#undef ckoldu
#undef cksump
#undef ckoldp
#undef cksuma
#undef ckolda
#endif
