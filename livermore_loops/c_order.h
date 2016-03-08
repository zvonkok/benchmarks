/*
      COMMON /ORDER/ inseq, match, NSTACK(20), isave, iret

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_ORDER
typedef struct {
    long Inseq;
    long Match;
    long Nstack[20];
    long Isave;
    long Iret;
} block_order ;
extern block_order ORDER;
#define BLOCK_ORDER
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_ORDER_STORE
block_order ORDER;
#define BLOCK_ORDER_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define inseq ORDER.Inseq
#define match ORDER.Match
#define nstack ORDER.Nstack
#define isave ORDER.Isave
#define iret ORDER.Iret

#undef nstack_1
#define nstack_1(a1) nstack[a1-1]

#else

/* clear macros for variable names */
#undef inseq
#undef match
#undef nstack
#undef isave
#undef iret
#endif
