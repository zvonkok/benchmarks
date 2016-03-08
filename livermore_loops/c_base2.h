/*

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

        COMMON /BASE2/ BUFP(34132)
      COMMON /BASE2/ P0(4,512), PX0(25,101), CX0(25,101),
     1  VY0(101,25), VH0(101,7), VF0(101,7), VG0(101,7), VS0(101,7),
     2  ZA0(101,7)  , ZP0(101,7), ZQ0(101,7), ZR0(101,7), ZM0(101,7),
     3  ZB0(101,7)  , ZU0(101,7), ZV0(101,7), ZZ0(101,7),
     4  B0(64,64), CC0(64,64), H0(64,64),
     5  U10(5,101,2),  U20(5,101,2),  U30(5,101,2)
*/

#ifndef BLOCK_BASE2
typedef struct {
  REAL P0[512][4];
  REAL Px0[101][25];
  REAL Cx0[101][25];
  REAL Vy0[25][101];
  REAL Vh0[7][101];
  REAL Vf0[7][101];
  REAL Vg0[7][101];
  REAL Vs0[7][101];
  REAL Za0[7][101];
  REAL Zp0[7][101];
  REAL Zq0[7][101];
  REAL Zr0[7][101];
  REAL Zm0[7][101];
  REAL Zb0[7][101];
  REAL Zu0[7][101];
  REAL Zv0[7][101];
  REAL Zz0[7][101];
  REAL Bb0[64][64];
  REAL Cc0[64][64];
  REAL H0[64][64];
  REAL U10[2][101][5];
  REAL U20[2][101][5];
  REAL U30[2][101][5];
} block_base2 ;
extern block_base2 BASE2;
extern REAL *base2_bufp;
#define BLOCK_BASE2
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_BASE2_STORE
block_base2 BASE2;
REAL *base2_bufp= (REAL *)&BASE2;
#define BLOCK_BASE2_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define u30 BASE2.U30
#define u20 BASE2.U20
#define u10 BASE2.U10
#define h0 BASE2.H0
#define cc0 BASE2.Cc0
#define b0 BASE2.B0
#define zz0 BASE2.Zz0
#define zv0 BASE2.Zv0
#define zu0 BASE2.Zu0
#define zb0 BASE2.Zb0
#define zm0 BASE2.Zm0
#define zr0 BASE2.Zr0
#define zq0 BASE2.Zq0
#define zp0 BASE2.Zp0
#define za0 BASE2.Za0
#define vs0 BASE2.Vs0
#define vg0 BASE2.Vg0
#define vf0 BASE2.Vf0
#define vh0 BASE2.Vh0
#define vy0 BASE2.Vy0
#define cx0 BASE2.Cx0
#define px0 BASE2.Px0
#define p0 BASE2.P0
#define bufp base2_bufp

#undef bufp_1
#define bufp_1(a1) bufp[a1-1]

#undef u30_3
#define u30_3(a1,a2,a3) u30[a3-1][a2-1][a1-1]
#undef u20_3
#define u20_3(a1,a2,a3) u20[a3-1][a2-1][a1-1]
#undef u10_3
#define u10_3(a1,a2,a3) u10[a3-1][a2-1][a1-1]
#undef h0_2
#define h0_2(a1,a2) h0[a2-1][a1-1]
#undef cc0_2
#define cc0_2(a1,a2) cc0[a2-1][a1-1]
#undef b0_2
#define b0_2(a1,a2) b0[a2-1][a1-1]
#undef zz0_2
#define zz0_2(a1,a2) zz0[a2-1][a1-1]
#undef zv0_2
#define zv0_2(a1,a2) zv0[a2-1][a1-1]
#undef zu0_2
#define zu0_2(a1,a2) zu0[a2-1][a1-1]
#undef zb0_2
#define zb0_2(a1,a2) zb0[a2-1][a1-1]
#undef zm0_2
#define zm0_2(a1,a2) zm0[a2-1][a1-1]
#undef zr0_2
#define zr0_2(a1,a2) zr0[a2-1][a1-1]
#undef zq0_2
#define zq0_2(a1,a2) zq0[a2-1][a1-1]
#undef zp0_2
#define zp0_2(a1,a2) zp0[a2-1][a1-1]
#undef za0_2
#define za0_2(a1,a2) za0[a2-1][a1-1]
#undef vs0_2
#define vs0_2(a1,a2) vs0[a2-1][a1-1]
#undef vg0_2
#define vg0_2(a1,a2) vg0[a2-1][a1-1]
#undef vf0_2
#define vf0_2(a1,a2) vf0[a2-1][a1-1]
#undef vh0_2
#define vh0_2(a1,a2) vh0[a2-1][a1-1]
#undef vy0_2
#define vy0_2(a1,a2) vy0[a2-1][a1-1]
#undef cx0_2
#define cx0_2(a1,a2) cx0[a2-1][a1-1]
#undef px0_2
#define px0_2(a1,a2) px0[a2-1][a1-1]
#undef p0_2
#define p0_2(a1,a2) p0[a2-1][a1-1]

#else

/* clear macros for variable names */
#undef u30
#undef u20
#undef u10
#undef h0
#undef cc0
#undef b0
#undef zz0
#undef zv0
#undef zu0
#undef zb0
#undef zm0
#undef zr0
#undef zq0
#undef zp0
#undef za0
#undef vs0
#undef vg0
#undef vf0
#undef vh0
#undef vy0
#undef cx0
#undef px0
#undef p0

#undef u30_3
#undef u20_3
#undef u10_3
#undef h0_2
#undef cc0_2
#undef b0_2
#undef zz0_2
#undef zv0_2
#undef zu0_2
#undef zb0_2
#undef zm0_2
#undef zr0_2
#undef zq0_2
#undef zp0_2
#undef za0_2
#undef vs0_2
#undef vg0_2
#undef vf0_2
#undef vh0_2
#undef vy0_2
#undef cx0_2
#undef px0_2
#undef p0_2
#endif
