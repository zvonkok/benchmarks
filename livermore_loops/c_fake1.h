/*
      COMMON /FAKE1/ t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11(20),t12(20)

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

*/

#ifndef BLOCK_FAKE1
typedef struct {
    REAL T0;
    REAL T1;
    REAL T2;
    REAL T3;
    REAL T4;
    REAL T5;
    REAL T6;
    REAL T7;
    REAL T8;
    REAL T9;
    REAL T10;
    REAL T11[20];
    REAL T12[20];
} block_fake1 ;
extern block_fake1 FAKE1;
#define BLOCK_FAKE1
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_FAKE1_STORE
block_fake1 FAKE1;
#define BLOCK_FAKE1_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define t_0 FAKE1.T0
#define t1 FAKE1.T1
#define t2 FAKE1.T2
#define t3 FAKE1.T3
#define t4 FAKE1.T4
#define t5 FAKE1.T5
#define t6 FAKE1.T6
#define t7 FAKE1.T7
#define t8 FAKE1.T8
#define t9 FAKE1.T9
#define t10 FAKE1.T10
#define t11 FAKE1.T11
#define t12 FAKE1.T12

#undef t12_1
#define t12_1(a1) t12[a1-1]
#undef t11_1
#define t11_1(a1) t11[a1-1]

#else

/* clear macros for variable names */
#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef t8
#undef t9
#undef t10
#undef t11
#undef t12
#undef t12_1
#undef t11_1
#endif





