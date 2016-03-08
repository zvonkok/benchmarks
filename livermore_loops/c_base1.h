/*

   Copyright (c) 1998.  The Regents of the University of California.
                    All rights reserved.

        COMMON /BASE1/ BUFU(19977)
      COMMON /BASE1/ U0(1001), V0(1001), W0(1001),
     1  X0(1001), Y0(1001), Z0(1001), G0(1001),
     2  DU10(101), DU20(101), DU30(101), GRD0(1001), DEX0(1001),
     3  XI0(1001), EX0(1001), EX10(1001), DEX10(1001),
     4  VX0(1001), XX0(1001), RX0(1001), RH0(2048),
     5  VSP0(101), VSTP0(101), VXNE0(101), VXND0(101),
     6  VE30(101), VLR0(101), VLIN0(101), B50(101),
     7  PLAN0(300), D0(300), SA0(101), SB0(101)
*/

#ifndef BLOCK_BASE1
typedef struct {
  REAL U0[1001];
  REAL V0[1001];
  REAL W0[1001];
  REAL X0[1001];
  REAL Y0[1001];
  REAL Z0[1001];
  REAL G0[1001];
  REAL Du10[101];
  REAL Du20[101];
  REAL Du30[101];
  REAL Grd0[1001];
  REAL Dex0[1001];
  REAL Xi0[1001];
  REAL Ex0[1001];
  REAL Ex10[1001];
  REAL Dex10[1001];
  REAL Vx0[1001];
  REAL Xx0[1001];
  REAL Rx0[1001];
  REAL Rh0[2048];
  REAL Vsp0[101];
  REAL Vstp0[101];
  REAL Vxne0[101];
  REAL Vxnd0[101];
  REAL Ve30[101];
  REAL Vlr0[101];
  REAL Vlin0[101];
  REAL B50[101];
  REAL Plan0[300];
  REAL D0[300];
  REAL Sa0[101];
  REAL Sb0[101];
} block_base1 ;
extern block_base1 BASE1;
extern REAL *base1_bufu;
#define BLOCK_BASE1
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_BASE1_STORE
block_base1 BASE1;
REAL *base1_bufu= (REAL *)&BASE1;
#define BLOCK_BASE1_STORE
#endif
#endif

#ifdef SET_VARS
/* set macros for variable names */
#define u0 BASE1.U0
#define v0 BASE1.V0
#define w0 BASE1.W0
#define x0 BASE1.X0
#define y0 BASE1.Y0
#define z0 BASE1.Z0
#define g0 BASE1.G0
#define du10 BASE1.Du10
#define du20 BASE1.Du20
#define du30 BASE1.Du30
#define grd0 BASE1.Grd0
#define dex0 BASE1.Dex0
#define xi0 BASE1.Xi0
#define ex0 BASE1.Ex0
#define ex10 BASE1.Ex10
#define dex10 BASE1.Dex10
#define vx0 BASE1.Vx0
#define xx0 BASE1.Xx0
#define rx0 BASE1.Rx0
#define rh0 BASE1.Rh0
#define vsp0 BASE1.Vsp0
#define vstp0 BASE1.Vstp0
#define vxne0 BASE1.Vxne0
#define vxnd0 BASE1.Vxnd0
#define ve30 BASE1.Ve30
#define vlr0 BASE1.Vlr0
#define vlin0 BASE1.Vlin0
#define b50 BASE1.B50
#define plan0 BASE1.Plan0
#define d0 BASE1.D0
#define sa0 BASE1.Sa0
#define sb0 BASE1.Sb0
#define bufu base1_bufu

#undef bufu_1
#define bufu_1(a1) bufu[a1-1]

#undef u0_1
#define u0_1(a1) u0[a1-1]
#undef v0_1
#define v0_1(a1) v0[a1-1]
#undef w0_1
#define w0_1(a1) w0[a1-1]
#undef x0_1
#define x0_1(a1) x0[a1-1]
#undef y0_1
#define y0_1(a1) y0[a1-1]
#undef z0_1
#define z0_1(a1) z0[a1-1]
#undef g0_1
#define g0_1(a1) g0[a1-1]
#undef du10_1
#define du10_1(a1) du10[a1-1]
#undef du20_1
#define du20_1(a1) du20[a1-1]
#undef du30_1
#define du30_1(a1) du30[a1-1]
#undef grd0_1
#define grd0_1(a1) grd0[a1-1]
#undef dex0_1
#define dex0_1(a1) dex0[a1-1]
#undef xi0_1
#define xi0_1(a1) xi0[a1-1]
#undef ex0_1
#define ex0_1(a1) ex0[a1-1]
#undef ex10_1
#define ex10_1(a1) ex10[a1-1]
#undef dex10_1
#define dex10_1(a1) dex10[a1-1]
#undef vx0_1
#define vx0_1(a1) vx0[a1-1]
#undef xx0_1
#define xx0_1(a1) xx0[a1-1]
#undef rx0_1
#define rx0_1(a1) rx0[a1-1]
#undef rh0_1
#define rh0_1(a1) rh0[a1-1]
#undef vsp0_1
#define vsp0_1(a1) vsp0[a1-1]
#undef vstp0_1
#define vstp0_1(a1) vstp0[a1-1]
#undef vxnep0_1
#define vxne0_1(a1) vxne0[a1-1]
#undef vxndp0_1
#define vxnd0_1(a1) vxnd0[a1-1]
#undef ve30_1
#define ve30_1(a1) ve30[a1-1]
#undef vlr0_1
#define vlr0_1(a1) vlr0[a1-1]
#undef vlin0_1
#define vlin0_1(a1) vlin0[a1-1]
#undef b50_1
#define b50_1(a1) b50[a1-1]
#undef bplan0_1
#define plan0_1(a1) plan0[a1-1]
#undef d0_1
#define d0_1(a1) d0[a1-1]
#undef sa0_1
#define sa0_1(a1) sa0[a1-1]
#undef sb0_1
#define sb0_1(a1) sb0[a1-1]

#else

/* clear macros for variable names */
#undef u0
#undef v0
#undef w0
#undef x0
#undef y0
#undef z0
#undef g0
#undef du10
#undef du20
#undef du30
#undef grd0
#undef dex0
#undef xi0
#undef ex0
#undef ex10
#undef dex10
#undef vx0
#undef xx0
#undef rx0
#undef rh0
#undef vsp0
#undef vstp0
#undef vxnep0
#undef vxndp0
#undef ve30
#undef vlr0
#undef vlin0
#undef b50
#undef bplan0
#undef d0
#undef sa0
#undef sb0
#undef bufu

#undef bufu_1
#undef u0_1
#undef v0_1
#undef w0_1
#undef x0_1
#undef y0_1
#undef z0_1
#undef g0_1
#undef du10_1
#undef du20_1
#undef du30_1
#undef grd0_1
#undef dex0_1
#undef xi0_1
#undef ex0_1
#undef ex10_1
#undef dex10_1
#undef vx0_1
#undef xx0_1
#undef rx0_1
#undef rh0_1
#undef vsp0_1
#undef vstp0_1
#undef vxnep0_1
#undef vxndp0_1
#undef ve30_1
#undef vlr0_1
#undef vlin0_1
#undef b50_1
#undef bplan0_1
#undef d0_1
#undef sa0_1
#undef sb0_1
#endif
