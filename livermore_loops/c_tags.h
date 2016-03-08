/*
  common /tags/  names(11,4)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_TAGS
typedef struct {
  char *Names[4][11];
} block_tags ;
extern block_tags TAGS;
#define BLOCK_TAGS
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_TAGS_STORE
block_tags TAGS;
#define BLOCK_TAGS_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define names TAGS.Names

#undef names_2
#define names_2(a1,a2) names[a2-1][a1-1]

#else

/* clear macros for variable names */
#undef names
#undef names_2
#endif
