/*
      COMMON /SPACE3/ CACHE(8192)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_SPACE3
typedef struct {
  REAL Cache[8192];
} block_space3 ;
extern block_space3 SPACE3;
#define BLOCK_SPACE3
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACE3_STORE
block_space3 SPACE3;
#define BLOCK_SPACE3_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define cache SPACE3.Cache

#undef cache_1
#define cache_1(a1) cache[a1-1]

#else

/* clear macros for variable names */
#undef cache
#undef cache_1
#endif
