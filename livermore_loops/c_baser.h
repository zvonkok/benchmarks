/*

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

      COMMON /BASER/ BUFA(39)
      COMMON /BASER/ A110,A120,A130,A210,A220,A230,A310,A320,A330,
     2                AR0,BR0,C00,CR0,DI0,DK0,
     3  DM220,DM230,DM240,DM250,DM260,DM270,DM280,DN0,E30,E60,EXPMAX0,
     4  FLX0,Q0,QA0,R0,RI0,S0,SCALE0,SIG0,STB50,T0,XNC0,XNEI0,XNM0
*/

#ifndef BLOCK_BASER
typedef struct {
  REAL A110;
  REAL A120;
  REAL A130;
  REAL A210;
  REAL A220;
  REAL A230;
  REAL A310;
  REAL A320;
  REAL A330;
  REAL Ar0;
  REAL Br0;
  REAL C00;
  REAL Cr0;
  REAL Di0;
  REAL Dk0;
  REAL Dm220;
  REAL Dm230;
  REAL Dm240;
  REAL Dm250;
  REAL Dm260;
  REAL Dm270;
  REAL Dm280;
  REAL Dn0;
  REAL E30;
  REAL E60;
  REAL Expmax0;
  REAL Flx0;
  REAL Q0;
  REAL Qa0;
  REAL R0;
  REAL Ri0;
  REAL S0;
  REAL Scale0;
  REAL Sig0;
  REAL Stb50;
  REAL T0;
  REAL Xxnc0;
  REAL Xnei0;
  REAL Xnm0;
} block_baser ;
extern block_baser BASER;
extern REAL *baser_bufa;
#define BLOCK_BASER
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_BASER_STORE
block_baser BASER;
REAL *baser_bufa= (REAL *)&BASER;
#define BLOCK_BASER_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define a110 BASER.A110
#define a120 BASER.A120
#define a130 BASER.A130
#define a210 BASER.A210
#define a220 BASER.A220
#define a230 BASER.A230
#define a310 BASER.A310
#define a320 BASER.A320
#define a330 BASER.A330
#define ar0 BASER.Ar0
#define br0 BASER.Br0
#define c00 BASER.C00
#define cr0 BASER.Cr0
#define di0 BASER.Di0
#define dk0 BASER.Dk0
#define dm220 BASER.Dm220
#define dm230 BASER.Dm230
#define dm240 BASER.Dm240
#define dm250 BASER.Dm250
#define dm260 BASER.Dm260
#define dm270 BASER.Dm270
#define dm280 BASER.Dm280
#define dn0 BASER.Dn0
#define e30 BASER.E30
#define e60 BASER.E60
#define expmax0 BASER.Expmax0
#define flx0 BASER.Flx0
#define q0 BASER.Q0
#define qa0 BASER.Qa0
#define r0 BASER.R0
#define ri0 BASER.Ri0
#define s0 BASER.S0
#define scale0 BASER.Scale0
#define sig0 BASER.Sig0
#define stb50 BASER.Stb50
#define t0 BASER.T0
#define xnc0 BASER.Xnc0
#define xnei0 BASER.Xnei0
#define xnm0 BASER.Xnm0
#define bufa baser_bufa

#undef bufa_1
#define bufa_1(a1) bufa[a1-1]

#else

/* clear macros for variable names */
#undef a110
#undef a120
#undef a130
#undef a210
#undef a220
#undef a230
#undef a310
#undef a320
#undef a330
#undef ar0
#undef br0
#undef c00
#undef cr0
#undef di0
#undef dk0
#undef dm220
#undef dm230
#undef dm240
#undef dm250
#undef dm260
#undef dm270
#undef dm280
#undef dn0
#undef e30
#undef e60
#undef expmax0
#undef fLx0
#undef q0
#undef qa0
#undef r0
#undef ri0
#undef s0
#undef scale0
#undef siG0
#undef stb50
#undef t0
#undef xnc0
#undef xnei0
#undef xnm0
#undef bufa
#undef bufa_1
#endif
