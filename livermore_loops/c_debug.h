/*
      COMMON /DEBUG/     ISTACK(20)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_DEBUG
typedef struct {
    char *Istack[20];
} block_debug ;
extern block_debug DEBUG;
#define BLOCK_DEBUG
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_DEBUG_STORE
block_debug DEBUG;
#define BLOCK_DEBUG_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define istack DEBUG.Istack

#undef istack_1
#define istack_1(a1) istack[a1-1]

#else

/* clear macros for variable names */
#undef istack
#undef istack_1
#endif
