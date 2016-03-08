/*      COMMON /SYSID/ Komput, Kontrl, Kompil, Kalend

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

 */

#ifndef BLOCK_SYSID
typedef struct {
  char *Komput;
  char *Kontrl;
  char *Kompil;
  char *Kalend;
  char *Koutfil;
} block_sysid ;
extern block_sysid SYSID;
#define BLOCK_SYSID
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SYSID_STORE
block_sysid SYSID;
#define BLOCK_SYSID_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define komput SYSID.Komput
#define kontrl SYSID.Kontrl
#define kompil SYSID.Kompil
#define kalend SYSID.Kalend
#define koutfil SYSID.Koutfil

#else

/* clear macros for variable names */
#undef komput
#undef kontrl
#undef kompil
#undef kalend
#undef koutfil
#endif
