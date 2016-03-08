/* Livermore Loops coded in C        Latest File Modification  27 Jul 90
 *
 * NOTE NOTE NOTE: Modified for use in the pure ANSI C version
 * of the LFK test program by Steven H. Langer.
 * Changes include calling sequence from Fortran to C and
 * minor changes in COMMON block arguments.
 * Feb. 14, 1995.
 *
 * Copyright (c) 1995-8.  The Regents of the University of California.
 *                  All rights reserved.
 *
 *
 * This file defines the equivalent of the common blocks used 
 * for communication by the Fortran version of the Livermore 
 * Fortran Kernels.
 *
 * The file is split up into sections for each "common block".
 * A function that needs to use a common block first #defines
 * the symbol for that common block(s) and then includes
 * this file. Any function that includes this file should end
 * with a #include of undef_common.h, which undefines all the
 * common block flags and then includes this file. The result 
 * is to undefine all the common block variables.
 *
 ************************************************************************
 *                                                                      *
 *     L. L. N. L.   " C "   K E R N E L S  T E S T:   M F L O P S      *
 *                                                                      *
 *     These kernels measure   " C "   numerical computation            *
 *     rates for  a  spectrum  of  cpu-limited computational            *
 *     structures or benchmarks.   Mathematical  through-put            *
 *     is measured  in  units  of millions of floating-point            *
 *     operations executed per second, called Megaflops/sec.            *
 *                                                                      *
 *     Fonzi's Law: There is not now and there never will be a language *
 *                  in which it is the least bit difficult to write     *
 *                  bad programs.                                       *
 *                                                                      *
 *Originally from  Greg Astfalk, AT&T, P.O.Box 900, Princeton, NJ. 08540*
 *by way of Frank McMahon, LLNL, PO Box 808, Livermore, CA, 94550. 1986 *
 *                                                                      *
 *    Changes made to correct many array subscripting problems,         *
 *      make more readable (added #define's), include the original      *
 *      FORTRAN versions of the runs as comments, and make more         *
 *      portable by Kelly O'Hair (LLNL) and Chuck Rasbold (LLNL)        *
 *           and by Mark Seager  (LLNL).                                *
 *                                                                      *
 *      please send copy of sdtout to:   MCMAHON3@LLNL.GOV              *
 *                                 or:   mcmahon@lll-crg.llnl.gov       *
 *                                                                      *
 ************************************************************************
 *                                                                      *
 *                               REFERENCE                              *
 *                                                                      *
 *              F.H.McMahon,   The Livermore Fortran Kernels:           *
 *              A Computer Test Of The Numerical Performance Range,     *
 *              Lawrence Livermore National Laboratory,                 *
 *              Livermore, California, UCRL-53745, December 1986.       *
 *                                                                      *
 *       from:  National Technical Information Service                  *
 *              U.S. Department of Commerce                             *
 *              5285 Port Royal Road                                    *
 *              Springfield, VA.  22161                                 *
 *                                                                      *
 *                                                                      *
 *                   (C) Copyright 1986 the Regents of the              *
 *               University of California. All Rights Reserved.         *
 *                                                                      *
 *              This work was produced under the sponsorship of         *
 *               the U.S. Department of Energy. The Government          *
 *                      retains certain rights therein.                 *
 *                                                                      *
 ************************************************************************
 */
 
#ifdef SET_VARS
 
/* Precision-specifiers for variables coming from the
 * Fortran common blocks.
 */
#ifndef REAL
#define REAL       float
#endif
 
#endif      /* end of SET_VARS test */
 
/* Type-specifiers for the structs that map to common.
   Defining ALLOC_COMMON before including this file will
   cause storage to be allocated. Otherwise, this file just
   defines variables.
 */
 
/* Names of structs (or common blocks) */
#define ALPHA  alpha
#define BETA   beta
#define SPACES spaces
#define SPACER spacer
#define SPACE0 space0
#define SPACEI spacei
#define ISPACE ispace
#define SPACE1 space1
#define SPACE2 space2


/* Define the structs (COMMON BLOCKS in original Fortran) */
 
/*
      COMMON /ALPHA/ mk,ik,im,ml,il,Mruns,Nruns,jr,iovec,NPFS(8,3,47)
*/
#ifndef BLOCK_ALPHA
typedef struct {
    long Mk;
    long Ik;
    long Im;
    long Ml;
    long Il;
    long Mruns;
    long Nruns;
    long Jr;
    long Iovec;
    long Npfs[47][3][8];
} block_alpha ;
extern block_alpha ALPHA;
#define BLOCK_ALPHA
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_ALPHA_STORE
block_alpha ALPHA;
#define BLOCK_ALPHA_STORE
#endif
#endif

#ifdef SET_VARS
#define mk     ALPHA.Mk
#define ik     ALPHA.Ik
#define im     ALPHA.Im
#define ml     ALPHA.Ml
#define il     ALPHA.Il
#define mruns  ALPHA.Mruns
#define nruns  ALPHA.Nruns
#define jr     ALPHA.Jr
#define iovec  ALPHA.Iovec
#define npfs   ALPHA.Npfs
 
#undef npfs_3
#define npfs_3(a1,a2,a3) ALPHA.Npfs[a3-1][a2-1][a1-1]

#else

#undef mk
#undef ik
#undef im
#undef ml
#undef il
#undef mruns
#undef nruns
#undef jr
#undef iovec
#undef npfs
 
#undef npfs_3
#endif

/*
      COMMON /BETA / tic, TIMES(8,3,47), SEE(5,3,8,3),
     1              TERRS(8,3,47), CSUMS(8,3,47),
     2              FOPN(8,3,47), DOS(8,3,47)
*/
#ifndef BLOCK_BETA
typedef struct {
    REAL Tic;
    REAL Times[47][3][8];
    REAL See[3][8][3][5];
    REAL Terrs[47][3][8];
    REAL Csums[47][3][8];
    REAL Fopn[47][3][8];
    REAL Dos[47][3][8];
} block_beta ;
extern block_beta BETA;
#define BLOCK_BETA
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_BETA_STORE
block_beta BETA;
#define BLOCK_BETA_STORE
#endif
#endif

#ifdef SET_VARS
#define tic     BETA.Tic
#define times BETA.Times
#define see     BETA.See
#define terrs   BETA.Terrs
#define csums   BETA.Csums
#define fopn    BETA.Fopn
#define dos     BETA.Dos

#undef dos_3
#define dos_3(a1,a2,a3) BETA.Dos[a3-1][a2-1][a1-1]
#undef fopn_3
#define fopn_3(a1,a2,a3) BETA.Fopn[a3-1][a2-1][a1-1]
#undef csums_3
#define csums_3(a1,a2,a3) BETA.Csums[a3-1][a2-1][a1-1]
#undef terrs_3
#define terrs_3(a1,a2,a3) BETA.Terrs[a3-1][a2-1][a1-1]
#undef see_4
#define see_4(a1,a2,a3,a4) BETA.See[a4-1][a3-1][a2-1][a1-1]
#undef times_3
#define times_3(a1,a2,a3) BETA.Times[a3-1][a2-1][a1-1]

#else

#undef tic
#undef times
#undef see
#undef terrs
#undef csums
#undef fopn
#undef dos

#undef dos_3
#undef fopn_3
#undef csums_3
#undef terrs_3
#undef see_4
#undef times_3
#endif
 
/*
      COMMON /SPACES/ ion,j5,k2,k3,MULTI,laps,Loop,m,kr,LP,n13h,ibuf,nx,
     1 L,npass,nfail,n,n1,n2,n13,n213,n813,n14,n16,n416,n21,nt1,nt2,
     2 last,idebug,mpy,Loops2,mucho,mpylim, intbuf(16)
*/
#ifndef BLOCK_SPACES
typedef struct {
    FILE *Ion;
    long Ion_flag;
    long J5;
    long K2;
    long K3;
    long MULTI;
    long Laps;
    long Loop;
    long M;
    long Kr;
    long Lp;
    long N13h;
    long Ibuf;
    long Nx;
    long L;
    long Npass;
    long Nfail;
    long N;
    long N1;
    long N2;
    long N13;
    long N213;
    long N813;
    long N14;
    long N16;
    long N416;
    long N21;
    long Nt1;
    long Nt2;
    long Last;
    long Idebug;
    long Mpy;
    long Loops2;
    long Mucho;
    long Mpylim;
    long Intbuf[16];
} block_spaces ;
extern block_spaces SPACES;
#define BLOCK_SPACES
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACES_STORE
block_spaces SPACES;
#define BLOCK_SPACES_STORE
#endif
#endif

#ifdef SET_VARS
#define  ion    SPACES.Ion
#define  ion_flag    SPACES.Ion_flag
#define  j5     SPACES.J5
#define  k2     SPACES.K2
#define  k3     SPACES.K3
#define  multi  SPACES.MULTI
#define  laps   SPACES.Laps
#define  loop   SPACES.Loop
#define  m    SPACES.M
#define  kr     SPACES.Kr
#define  lp     SPACES.Lp
#define  n13h   SPACES.N13h
#define  ibuf   SPACES.Ibuf
#define  nx     SPACES.Nx
#define  l   SPACES.L
#define  npass  SPACES.Npass
#define  nfail  SPACES.Nfail
#define  n   SPACES.N
#define  n1     SPACES.N1
#define  n2     SPACES.N2
#define  n13    SPACES.N13
#define  n213   SPACES.N213
#define  n813   SPACES.N813
#define  n14    SPACES.N14
#define  n16    SPACES.N16
#define  n416   SPACES.N416
#define  n21    SPACES.N21
#define  nt1    SPACES.Nt1
#define  nt2    SPACES.Nt2
#define  last   SPACES.Last
#define  idebug SPACES.Idebug
#define  mpy    SPACES.Mpy
#define  loops2 SPACES.Loops2
#define  mucho  SPACES.Mucho
#define  mpylim SPACES.Mpylim
#define  intbuf SPACES.Intbuf

#undef intbuf_1
#define intbuf_1(a1) SPACES.Intbuf[a1-1]

#else

#undef  ion
#undef  ion_flag
#undef  j5
#undef  k2
#undef  k3
#undef  multi
#undef  laps
#undef  loop
#undef  m
#undef  kr
#undef  lp
#undef  n13h
#undef  ibuf
#undef  nx
#undef  l
#undef  npass
#undef  nfail
#undef  n
#undef  n1
#undef  n2
#undef  n13
#undef  n213
#undef  n813
#undef  n14
#undef  n16
#undef  n416
#undef  n21
#undef  nt1
#undef  nt2
#undef  last
#undef  idebug
#undef  mpy
#undef  loops2
#undef  mucho
#undef  mpylim
#undef  intbuf
#undef intbuf_1
#endif

/*
      COMMON /SPACER/ A11,A12,A13,A21,A22,A23,A31,A32,A33,
     2                AR,BR,C0,CR,DI,DK,
     3  DM22,DM23,DM24,DM25,DM26,DM27,DM28,DN,E3,E6,EXPMAX,FLX,
     4  Q,QA,R,RI,S,SCALE,SIG,STB5,T,XNC,XNEI,XNM
*/
#ifndef BLOCK_SPACER
typedef struct {
    REAL A11;
    REAL A12;
    REAL A13;
    REAL A21;
    REAL A22;
    REAL A23;
    REAL A31;
    REAL A32;
    REAL A33;
    REAL Ar;
    REAL Br;
    REAL C0;
    REAL Cr;
    REAL Di;
    REAL Dk;
    REAL Dm22;
    REAL Dm23;
    REAL Dm24;
    REAL Dm25;
    REAL Dm26;
    REAL Dm27;
    REAL Dm28;
    REAL Dn;
    REAL E3;
    REAL E6;
    REAL Expmax;
    REAL Flx;
    REAL Q;
    REAL Qa;
    REAL R;
    REAL Ri;
    REAL S;
    REAL Scale;
    REAL Sig;
    REAL Stb5;
    REAL T;
    REAL Xnc;
    REAL Xnei;
    REAL Xnm;
} block_spacer ;
extern block_spacer SPACER;
extern REAL *spacer_aaa11;
#define BLOCK_SPACER
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACER_STORE
block_spacer SPACER;
REAL *spacer_aaa11= (REAL *)&SPACER;
#define BLOCK_SPACER_STORE
#endif
#endif

#ifdef SET_VARS
#define  a11     SPACER.A11
#define  a12     SPACER.A12
#define  a13     SPACER.A13
#define  a21     SPACER.A21
#define  a22     SPACER.A22
#define  a23     SPACER.A23
#define  a31     SPACER.A31
#define  a32     SPACER.A32
#define  a33     SPACER.A33
#define  ar      SPACER.Ar
#define  br      SPACER.Br
#define  c0      SPACER.C0
#define  cr      SPACER.Cr
#define  di      SPACER.Di
#define  dk      SPACER.Dk
#define  dm22    SPACER.Dm22
#define  dm23    SPACER.Dm23
#define  dm24    SPACER.Dm24
#define  dm25    SPACER.Dm25
#define  dm26    SPACER.Dm26
#define  dm27    SPACER.Dm27
#define  dm28    SPACER.Dm28
#define  dn      SPACER.Dn
#define  e3      SPACER.E3
#define  e6      SPACER.E6
#define  expmax  SPACER.Expmax
#define  flx     SPACER.Flx
#define  q    SPACER.Q
#define  qa      SPACER.Qa
#define  r    SPACER.R
#define  ri      SPACER.Ri
#define  s    SPACER.S
#define  scale SPACER.Scale
#define  sig     SPACER.Sig
#define  stb5    SPACER.Stb5
#define  t    SPACER.T
#define  xnc     SPACER.Xnc
#define  xnei    SPACER.Xnei
#define  xnm     SPACER.Xnm
#define  aaa11   spacer_aaa11

#undef aaa11_1
#define aaa11_1(a1) aaa11[a1-1]

#else

#undef  a11
#undef  a12
#undef  a13
#undef  a21
#undef  a22
#undef  a23
#undef  a31
#undef  a32
#undef  a33
#undef  ar
#undef  br
#undef  c0
#undef  cr
#undef  di
#undef  dk
#undef  dm22
#undef  dm23
#undef  dm24
#undef  dm25
#undef  dm26
#undef  dm27
#undef  dm28
#undef  dn
#undef  e3
#undef  e6
#undef  expmax
#undef  flx
#undef  q
#undef  qa
#undef  r
#undef  ri
#undef  s
#undef  scale
#undef  sig
#undef  stb5
#undef  t
#undef  xnc
#undef  xnei
#undef  xnm
#undef aaa11
#undef aaa11_1
#endif


/*
      COMMON /SPACE0/ TIME(47), CSUM(47), WW(47), WT(47), ticks,
     1                FR(9), TERR1(47), SUMW(7), START,
     2              SKALE(47), BIAS(47), WS(95), TOTAL(47), FLOPN(47),
     3                IQ(7), NPF, NPFS1(47)
*/
#ifndef BLOCK_SPACE0
typedef struct {
    REAL Time[47];
    REAL Csum[47];
    REAL Ww[47];
    REAL Wt[47];
    REAL Ticks;
    REAL Fr[9];
    REAL Terr1[47];
    REAL Sumw[7];
    REAL Start;
    REAL Skale[47];
    REAL Bias[47];
    REAL Ws[95];
    REAL Total[47];
    REAL Flopn[47];
    long Iq[7];
    long Npf;
    long Npfs1[47];
} block_space0 ;
extern block_space0 SPACE0;
#define BLOCK_SPACE0
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACE0_STORE
block_space0 SPACE0;
#define BLOCK_SPACE0_STORE
#endif
#endif

#ifdef SET_VARS
#define  time    SPACE0.Time
#define  csum    SPACE0.Csum
#define  ww      SPACE0.Ww
#define  wt      SPACE0.Wt
#define  ticks   SPACE0.Ticks
#define  fr      SPACE0.Fr
#define  terr1   SPACE0.Terr1
#define  sumw    SPACE0.Sumw
#define  start   SPACE0.Start
#define  skale   SPACE0.Skale
#define  bias SPACE0.Bias
#define  ws      SPACE0.Ws
#define  total   SPACE0.Total
#define  flopn   SPACE0.Flopn
#define  iq      SPACE0.Iq
#define  npf     SPACE0.Npf
#define  npfs1   SPACE0.Npfs1
 
#undef flopn_1
#define flopn_1(a1) SPACE0.Flopn[a1-1]
#undef total_1
#define total_1(a1) SPACE0.Total[a1-1]
#undef ws_1
#define ws_1(a1) SPACE0.Ws[a1-1]
#undef bias_1
#define bias_1(a1) SPACE0.Bias[a1-1]
#undef skale_1
#define skale_1(a1) SPACE0.Skale[a1-1]
#undef sumw_1
#define sumw_1(a1) SPACE0.Sumw[a1-1]
#undef terr1_1
#define terr1_1(a1) SPACE0.Terr1[a1-1]
#undef fr_1
#define fr_1(a1) SPACE0.Fr[a1-1]
#undef wt_1
#define wt_1(a1) SPACE0.Wt[a1-1]
#undef ww_1
#define ww_1(a1) SPACE0.Ww[a1-1]
#undef csum_1
#define csum_1(a1) SPACE0.Csum[a1-1]
#undef time_1
#define time_1(a1) SPACE0.Time[a1-1]

#undef npfs1_1
#define npfs1_1(a1) SPACE0.Npfs1[a1-1]
#undef iq_1
#define iq_1(a1) SPACE0.Iq[a1-1]

#else

#undef  time
#undef  csum
#undef  ww
#undef  wt
#undef  ticks
#undef  fr
#undef  terr1
#undef  sumw
#undef  start
#undef  skale
#undef  bias
#undef  ws
#undef  total
#undef  flopn
#undef  iq
#undef  npf
#undef  npfs1
#undef flopn_1
#undef total_1
#undef ws_1
#undef bias_1
#undef skale_1
#undef sumw_1
#undef terr1_1
#undef fr_1
#undef wt_1
#undef ww_1
#undef csum_1
#undef time_1
#undef npfs1_1
#undef iq_1
#endif

/*
      COMMON /SPACEI/ WTP(3), MUL(3), ISPAN(47,3), IPASS(47,3)
*/
#ifndef BLOCK_SPACEI
typedef struct {
  REAL Wtp[3];
  long Mul[3];
  long Ispan[3][47];
  long Ipass[3][47];
} block_spacei ;
extern block_spacei SPACEI;
#define BLOCK_SPACEI
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACEI_STORE
block_spacei SPACEI;
#define BLOCK_SPACEI_STORE
#endif
#endif

#ifdef SET_VARS
#define wtp    SPACEI.Wtp
#define mul    SPACEI.Mul
#define ispan  SPACEI.Ispan
#define ipass  SPACEI.Ipass
 
#undef wtp_1
#define wtp_1(a1) SPACEI.Wtp[a1-1]
#undef ipass_2
#define ipass_2(a1,a2) SPACEI.Ipass[a2-1][a1-1]
#undef ispan_2
#define ispan_2(a1,a2) SPACEI.Ispan[a2-1][a1-1]
#undef mul_1
#define mul_1(a1) SPACEI.Mul[a1-1]

#else

#undef wtp
#undef mul
#undef ispan
#undef ipass
#undef wtp_1
#undef ipass_2
#undef ispan_2
#undef mul_1
#endif


/*
      INTEGER    E,F,ZONE
      COMMON /ISPACE/ E(96), F(96),
     1  IX(1001), IR(1001), ZONE(300)
*/
#ifndef BLOCK_ISPACE
typedef struct {
    long E[96];
    long F[96];
    long Ix[1001];
    long Ir[1001];
    long Zone[300];
} block_ispace ;
extern block_ispace ISPACE;
#define BLOCK_ISPACE
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_ISPACE_STORE
block_ispace ISPACE;
#define BLOCK_ISPACE_STORE
#endif
#endif

#ifdef SET_VARS
#define e ISPACE.E
#define f ISPACE.F
#define ix   ISPACE.Ix
#define ir   ISPACE.Ir
#define zone ISPACE.Zone
 
#undef zone_1
#define zone_1(a1) ISPACE.Zone[a1-1]
#undef f_1
#define f_1(a1) ISPACE.F[a1-1]
#undef e_1
#define e_1(a1) ISPACE.E[a1-1]
#undef ir_1
#define ir_1(a1) ISPACE.Ir[a1-1]
#undef ix_1
#define ix_1(a1) ISPACE.Ix[a1-1]

#else

#undef e
#undef f
#undef ix
#undef ir
#undef zone
#undef zone_1
#undef f_1
#undef e_1
#undef ir_1
#undef ix_1
#endif


/*
      COMMON /SPACE1/ U(1001), V(1001), W(1001),
     1  X(1001), Y(1001), Z(1001), G(1001),
     2  DU1(101), DU2(101), DU3(101), GRD(1001), DEX(1001),
     3  XI(1001), EX(1001), EX1(1001), DEX1(1001),
     4  VX(1001), XX(1001), RX(1001), RH(2048),
     5  VSP(101), VSTP(101), VXNE(101), VXND(101),
     6  VE3(101), VLR(101), VLIN(101), B5(101),
     7  PLAN(300), D(300), SA(101), SB(101)
*/
#ifndef BLOCK_SPACE1
typedef struct {
    REAL U[1001];
    REAL V[1001];
    REAL W[1001];
    REAL X[1001];
    REAL Y[1001];
    REAL Z[1001];
    REAL G[1001];
    REAL Du1[101];
    REAL Du2[101];
    REAL Du3[101];
    REAL Grd[1001];
    REAL Dex[1001];
    REAL Xi[1001];
    REAL Ex[1001];
    REAL Ex1[1001];
    REAL Dex1[1001];
    REAL Vx[1001];
    REAL Xx[1001];
    REAL Rx[1001];
    REAL Rh[2048];
    REAL Vsp[101];
    REAL Vstp[101];
    REAL Vxne[101];
    REAL Vxnd[101];
    REAL Ve3[101];
    REAL Vlr[101];
    REAL Vlin[101];
    REAL B5[101];
    REAL Plan[300];
    REAL D[300];
    REAL Sa[101];
    REAL Sb[101];
} block_space1 ;
extern block_space1 SPACE1;
extern REAL *space1_uuu;
#define BLOCK_SPACE1
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACE1_STORE
block_space1 SPACE1;
REAL *space1_uuu= (REAL *)&SPACE1;
#define BLOCK_SPACE1_STORE
#endif
#endif

#ifdef SET_VARS
#define  u SPACE1.U
#define  v SPACE1.V
#define  w SPACE1.W
#define  x SPACE1.X
#define  y SPACE1.Y
#define  z SPACE1.Z
#define  g SPACE1.G
#define  du1  SPACE1.Du1
#define  du2  SPACE1.Du2
#define  du3  SPACE1.Du3
#define  grd  SPACE1.Grd
#define  dex  SPACE1.Dex
#define  xi   SPACE1.Xi
#define  ex   SPACE1.Ex
#define  ex1  SPACE1.Ex1
#define  dex1 SPACE1.Dex1
#define  vx   SPACE1.Vx
#define  xx   SPACE1.Xx
#define  rx   SPACE1.Rx
#define  rh   SPACE1.Rh
#define  vsp  SPACE1.Vsp
#define  vstp SPACE1.Vstp
#define  vxne SPACE1.Vxne
#define  vxnd SPACE1.Vxnd
#define  ve3  SPACE1.Ve3
#define  vlr  SPACE1.Vlr
#define  vlin SPACE1.Vlin
#define  b5   SPACE1.B5
#define  plan SPACE1.Plan
#define  d SPACE1.D
#define  sa   SPACE1.Sa
#define  sb   SPACE1.Sb
#define  uuu  space1_uuu

#undef uuu_1
#define uuu_1(a1) uuu[a1-1]

#undef sb_1
#define sb_1(a1) SPACE1.Sb[a1-1]
#undef sa_1
#define sa_1(a1) SPACE1.Sa[a1-1]
#undef d_1
#define d_1(a1) SPACE1.D[a1-1]
#undef plan_1
#define plan_1(a1) SPACE1.Plan[a1-1]
#undef b5_1
#define b5_1(a1) SPACE1.B5[a1-1]
#undef vlin_1
#define vlin_1(a1) SPACE1.Vlin[a1-1]
#undef vlr_1
#define vlr_1(a1) SPACE1.Vlr[a1-1]
#undef ve3_1
#define ve3_1(a1) SPACE1.Ve3[a1-1]
#undef vxnd_1
#define vxnd_1(a1) SPACE1.Vxnd[a1-1]
#undef vxne_1
#define vxne_1(a1) SPACE1.Vxne[a1-1]
#undef vstp_1
#define vstp_1(a1) SPACE1.Vstp[a1-1]
#undef vsp_1
#define vsp_1(a1) SPACE1.Vsp[a1-1]
#undef rh_1
#define rh_1(a1) SPACE1.Rh[a1-1]
#undef rx_1
#define rx_1(a1) SPACE1.Rx[a1-1]
#undef xx_1
#define xx_1(a1) SPACE1.Xx[a1-1]
#undef vx_1
#define vx_1(a1) SPACE1.Vx[a1-1]
#undef dex1_1
#define dex1_1(a1) SPACE1.Dex1[a1-1]
#undef ex1_1
#define ex1_1(a1) SPACE1.Ex1[a1-1]
#undef ex_1
#define ex_1(a1) SPACE1.Ex[a1-1]
#undef xi_1
#define xi_1(a1) SPACE1.Xi[a1-1]
#undef dex_1
#define dex_1(a1) SPACE1.Dex[a1-1]
#undef grd_1
#define grd_1(a1) SPACE1.Grd[a1-1]
#undef du3_1
#define du3_1(a1) SPACE1.Du3[a1-1]
#undef du2_1
#define du2_1(a1) SPACE1.Du2[a1-1]
#undef du1_1
#define du1_1(a1) SPACE1.Du1[a1-1]
#undef g_1
#define g_1(a1) SPACE1.G[a1-1]
#undef z_1
#define z_1(a1) SPACE1.Z[a1-1]
#undef y_1
#define y_1(a1) SPACE1.Y[a1-1]
#undef x_1
#define x_1(a1) SPACE1.X[a1-1]
#undef w_1
#define w_1(a1) SPACE1.W[a1-1]
#undef v_1
#define v_1(a1) SPACE1.V[a1-1]
#undef u_1
#define u_1(a1) SPACE1.U[a1-1]

#else

#undef  u
#undef  v
#undef  w
#undef  x
#undef  y
#undef  z
#undef  g
#undef  du1
#undef  du2
#undef  du3
#undef  grd
#undef  dex
#undef  xi
#undef  ex
#undef  ex1
#undef  dex1
#undef  vx
#undef  xx
#undef  rx
#undef  rh
#undef  vsp
#undef  vstp
#undef  vxne
#undef  vxnd
#undef  ve3
#undef  vlr
#undef  vlin
#undef  b5
#undef  plan
#undef  d
#undef  sa
#undef  sb
#undef uuu_1
#undef sb_1
#undef sa_1
#undef d_1
#undef plan_1
#undef b5_1
#undef vlin_1
#undef vlr_1
#undef ve3_1
#undef vxnd_1
#undef vxne_1
#undef vstp_1
#undef vsp_1
#undef rh_1
#undef rx_1
#undef xx_1
#undef vx_1
#undef dex1_1
#undef ex1_1
#undef ex_1
#undef xi_1
#undef dex_1
#undef grd_1
#undef du3_1
#undef du2_1
#undef du1_1
#undef g_1
#undef z_1
#undef y_1
#undef x_1
#undef w_1
#undef v_1
#undef u_1
#endif

/*
      COMMON /SPACE2/ P(4,512), PX(25,101), CX(25,101),
     1  VY(101,25), VH(101,7), VF(101,7), VG(101,7), VS(101,7),
     2  ZA(101,7)  , ZP(101,7), ZQ(101,7), ZR(101,7), ZM(101,7),
     3  ZB(101,7)  , ZU(101,7), ZV(101,7), ZZ(101,7),
     4  B(64,64), C(64,64), H(64,64),
     5  U1(5,101,2),  U2(5,101,2),  U3(5,101,2)
*/
#ifndef BLOCK_SPACE2
typedef struct {
    REAL P[512][4];
    REAL Px[101][25];
    REAL Cx[101][25];
    REAL Vy[25][101];
    REAL Vh[7][101];
    REAL Vf[7][101];
    REAL Vg[7][101];
    REAL Vs[7][101];
    REAL Za[7][101];
    REAL Zp[7][101];
    REAL Zq[7][101];
    REAL Zr[7][101];
    REAL Zm[7][101];
    REAL Zb[7][101];
    REAL Zu[7][101];
    REAL Zv[7][101];
    REAL Zz[7][101];
    REAL B[64][64];
    REAL C[64][64];
    REAL H[64][64];
    REAL U1[2][101][5];
    REAL U2[2][101][5];
    REAL U3[2][101][5];
} block_space2 ;
extern block_space2 SPACE2; 
extern REAL *space2_ppp;
#define BLOCK_SPACE2
#endif

#ifdef ALLOC_COMMON
#ifndef BLOCK_SPACE2_STORE
block_space2 SPACE2; 
REAL *space2_ppp= (REAL *)&SPACE2;
#define BLOCK_SPACE2_STORE
#endif
#endif

#ifdef SET_VARS
#define  p    SPACE2.P
#define  px      SPACE2.Px
#define  cx      SPACE2.Cx
#define  vy      SPACE2.Vy
#define  vh      SPACE2.Vh
#define  vf      SPACE2.Vf
#define  vg      SPACE2.Vg
#define  vs      SPACE2.Vs
#define  za      SPACE2.Za
#define  zp      SPACE2.Zp
#define  zq      SPACE2.Zq
#define  zr      SPACE2.Zr
#define  zm      SPACE2.Zm
#define  zb      SPACE2.Zb
#define  zu      SPACE2.Zu
#define  zv      SPACE2.Zv
#define  zz      SPACE2.Zz
#define  b    SPACE2.B
#define  c    SPACE2.C
#define  h    SPACE2.H
#define  u1      SPACE2.U1
#define  u2      SPACE2.U2
#define  u3      SPACE2.U3
#define  ppp     space2_ppp

#undef ppp_1
#define ppp_1(a1) ppp[a1-1]

#undef u3_3
#define u3_3(a1,a2,a3) SPACE2.U3[a3-1][a2-1][a1-1]
#undef u2_3
#define u2_3(a1,a2,a3) SPACE2.U2[a3-1][a2-1][a1-1]
#undef u1_3
#define u1_3(a1,a2,a3) SPACE2.U1[a3-1][a2-1][a1-1]
#undef h_2
#define h_2(a1,a2) SPACE2.H[a2-1][a1-1]
#undef c_2
#define c_2(a1,a2) SPACE2.C[a2-1][a1-1]
#undef b_2
#define b_2(a1,a2) SPACE2.B[a2-1][a1-1]
#undef zz_2
#define zz_2(a1,a2) SPACE2.Zz[a2-1][a1-1]
#undef zv_2
#define zv_2(a1,a2) SPACE2.Zv[a2-1][a1-1]
#undef zu_2
#define zu_2(a1,a2) SPACE2.Zu[a2-1][a1-1]
#undef zb_2
#define zb_2(a1,a2) SPACE2.Zb[a2-1][a1-1]
#undef zm_2
#define zm_2(a1,a2) SPACE2.Zm[a2-1][a1-1]
#undef zr_2
#define zr_2(a1,a2) SPACE2.Zr[a2-1][a1-1]
#undef zq_2
#define zq_2(a1,a2) SPACE2.Zq[a2-1][a1-1]
#undef zp_2
#define zp_2(a1,a2) SPACE2.Zp[a2-1][a1-1]
#undef za_2
#define za_2(a1,a2) SPACE2.Za[a2-1][a1-1]
#undef vs_2
#define vs_2(a1,a2) SPACE2.Vs[a2-1][a1-1]
#undef vg_2
#define vg_2(a1,a2) SPACE2.Vg[a2-1][a1-1]
#undef vf_2
#define vf_2(a1,a2) SPACE2.Vf[a2-1][a1-1]
#undef vh_2
#define vh_2(a1,a2) SPACE2.Vh[a2-1][a1-1]
#undef vy_2
#define vy_2(a1,a2) SPACE2.Vy[a2-1][a1-1]
#undef cx_2
#define cx_2(a1,a2) SPACE2.Cx[a2-1][a1-1]
#undef px_2
#define px_2(a1,a2) SPACE2.Px[a2-1][a1-1]
#undef p_2
#define p_2(a1,a2) SPACE2.P[a2-1][a1-1]

#else

#undef  p
#undef  px
#undef  cx
#undef  vy
#undef  vh
#undef  vf
#undef  vg
#undef  vs
#undef  za
#undef  zp
#undef  zq
#undef  zr
#undef  zm
#undef  zb
#undef  zu
#undef  zv
#undef  zz
#undef  b
#undef  c
#undef  h
#undef  u1
#undef  u2
#undef  u3
#undef ppp_1
#undef u3_3
#undef u2_3
#undef u1_3
#undef h_2
#undef c_2
#undef b_2
#undef zz_2
#undef zv_2
#undef zu_2
#undef zb_2
#undef zm_2
#undef zr_2
#undef zq_2
#undef zp_2
#undef za_2
#undef vs_2
#undef vg_2
#undef vf_2
#undef vh_2
#undef vy_2
#undef cx_2
#undef px_2
#undef p_2
#endif

 
/* End of File */
 
