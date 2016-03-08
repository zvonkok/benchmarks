/* Livermore Loops coded in C        Latest File Modification  27 Jul 90
 *
 * NOTE NOTE NOTE: Modified for use in the pure ANSI C version
 * of the LFK test program by Steven H. Langer.
 * Changes include calling sequence from Fortran to C and
 * minor changes in COMMON block arguments.
 * Split into separate header and source code files for convenience
 * in converting the main program to C.
 * Feb. 14, 1995.
 *
 * Copyright (c) 1995.  The Regents of the University of California.
 *                  All rights reserved.
 *
 *
 *     SUBROUTINE KERNEL( TK)  replaces the Fortran routine in LFK Test program.
 ************************************************************************
 *                                                                      *
 *          KERNEL     executes 24 samples of "C" numerical computation *
 *                                                                      *
 *                TK(1) - total cpu time to execute only the 24 kernels.*
 *                TK(2) - total Flops executed by the 24 Kernels        *
 *                                                                      *
 *   Link this C module with the rest of LFK Test compiled with Fortran *
 *   using a version of the LFK Test dated April 1990 or later.         *
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

#include <math.h>
#include "cflops.h"

/* set flags so as to get all common blocks in the file.
   Note that this invocation does not create storage for
   the common blocks.
*/
#undef ALLOC_COMMON

#define SET_VARS
#include "c_cernel.h"


/* KERNEL routine */

void kernel( REAL TK[] )
{

/* #pragma nodyneqv */
/* #pragma o=i */

    long ntest , k , ipnt , ipntp , i;
    long lw , j , nl1 , nl2 , kx , ky , ip , kn;
    long i1 , j1 , i2 , j2 , nz , ink , jn , kb5i;
    long ii , lb , j4 , ng;
    REAL tmp , temp;

    /*
     *   Prologue
     */

    trace( "kernel  " );

    space();
           l = 1;
        loop = 1;
          lp = 1;
    ntest = 0;
    test( ntest );

    /*
     *******************************************************************
     *   Kernel 1 -- hydro fragment
     *******************************************************************
     *       DO 1 L = 1,Loop
     *       DO 1 k = 1,n
     *  1       X(k)= Q + Y(k)*(R*ZX(k+10) + T*ZX(k+11))
     */
    ntest = 1;

    do {
       for ( k=0 ; k<n ; k++ ) {
            x[k] = q + y[k]*( r*z[k+10] + t*z[k+11] );
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 2 -- ICCG excerpt (Incomplete Cholesky Conjugate Gradient)
     *******************************************************************
     *    DO 200  L= 1,Loop
     *        II= n
     *     IPNTP= 0
     *222   IPNT= IPNTP
     *     IPNTP= IPNTP+II
     *        II= II/2
     *         i= IPNTP+1
     CDIR$ IVDEP
     *    DO 2 k= IPNT+2,IPNTP,2
     *         i= i+1
     *  2   X(i)= X(k) - V(k)*X(k-1) - V(k+1)*X(k+1)
     *        IF( II.GT.1) GO TO 222
     *200 CONTINUE
     */
    ntest = 2;


    do {
       ii = n;
       ipntp = 0;
        do {
            ipnt = ipntp;
            ipntp += ii;
            ii /= 2;
            i = ipntp ;
/* #pragma nohazard */
            for ( k=ipnt+1 ; k<ipntp ; k=k+2 ) {
                i++;
                x[i] = x[k] - v[k  ]*x[k-1] - v[k+1]*x[k+1];
            }
        } while ( ii>0 );
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 3 -- inner product
     *******************************************************************
     *    DO 3 L= 1,Loop
     *         Q= 0.0
     *    DO 3 k= 1,n
     *  3      Q= Q + Z(k)*X(k)
     */
    ntest = 3;

    do {
       q = 0.0;
        for ( k=0 ; k<n ; k++ ) {
            q += z[k]*x[k];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 4 -- banded linear equations
     *******************************************************************
     *            m= (1001-7)/2
     *    DO 444  L= 1,Loop
     *    DO 444  k= 7,1001,m
     *           lw= k-6
     *         temp= X(k-1)
     CDIR$ IVDEP
     *    DO   4  j= 5,n,5
     *       temp  = temp   - XZ(lw)*Y(j)
     *  4        lw= lw+1
     *       X(k-1)= Y(5)*temp
     *444 CONTINUE
     */
    ntest = 4;

    m = ( 1001-7 )/2;
    do {
       for ( k=6 ; k<1001 ; k=k+m ) {
            lw = k - 6;
            temp = x[k-1];
/* #pragma nohazard */
            for ( j=4 ; j<n ; j=j+5 ) {
                temp -= x[lw]*y[j];
                lw++;
            }
            x[k-1] = y[4]*temp;
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 5 -- tri-diagonal elimination, below diagonal
     *******************************************************************
     *    DO 5 L = 1,Loop
     *    DO 5 i = 2,n
     *  5    X(i)= Z(i)*(Y(i) - X(i-1))
     */
    ntest = 5;

    do {
       for ( i=1 ; i<n ; i++ ) {
            x[i] = z[i]*( y[i] - x[i-1] );
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 6 -- general linear recurrence equations
     *******************************************************************
     *    DO  6  L= 1,Loop
     *    DO  6  i= 2,n
     *    DO  6  k= 1,i-1
     *        W(i)= W(i)  + B(i,k) * W(i-k)
     *  6 CONTINUE
     */
    ntest = 6;

    do {
       for ( i=1 ; i<n ; i++ ) {
/* NOTE NOTE: clearing of w[i] put in by Steve Langer to match
   Fortran source.
*/
            w[i]= 0.0100;
            for ( k=0 ; k<i ; k++ ) {
                w[i] += b[k][i] * w[(i-k)-1];
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 7 -- equation of state fragment
     *******************************************************************
     *    DO 7 L= 1,Loop
     *    DO 7 k= 1,n
     *      X(k)=     U(k  ) + R*( Z(k  ) + R*Y(k  )) +
     *   .        T*( U(k+3) + R*( U(k+2) + R*U(k+1)) +
     *   .        T*( U(k+6) + Q*( U(k+5) + Q*U(k+4))))
     *  7 CONTINUE
     */
    ntest = 7;

/* #pragma nohazard */
    do {
       for ( k=0 ; k<n ; k++ ) {
            x[k] = u[k] + r*( z[k] + r*y[k] ) +
                   t*( u[k+3] + r*( u[k+2] + r*u[k+1] ) +
                      t*( u[k+6] + q*( u[k+5] + q*u[k+4] ) ) );
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 8 -- ADI integration
     *******************************************************************
     *    DO  8      L = 1,Loop
     *             nl1 = 1
     *             nl2 = 2
     *    DO  8     kx = 2,3
     CDIR$ IVDEP
     *    DO  8     ky = 2,n
     *          DU1(ky)=U1(kx,ky+1,nl1)  -  U1(kx,ky-1,nl1)
     *          DU2(ky)=U2(kx,ky+1,nl1)  -  U2(kx,ky-1,nl1)
     *          DU3(ky)=U3(kx,ky+1,nl1)  -  U3(kx,ky-1,nl1)
     *    U1(kx,ky,nl2)=U1(kx,ky,nl1) +A11*DU1(ky) +A12*DU2(ky) +A13*DU3(ky)
     *   .       + SIG*(U1(kx+1,ky,nl1) -2.*U1(kx,ky,nl1) +U1(kx-1,ky,nl1))
     *    U2(kx,ky,nl2)=U2(kx,ky,nl1) +A21*DU1(ky) +A22*DU2(ky) +A23*DU3(ky)
     *   .       + SIG*(U2(kx+1,ky,nl1) -2.*U2(kx,ky,nl1) +U2(kx-1,ky,nl1))
     *    U3(kx,ky,nl2)=U3(kx,ky,nl1) +A31*DU1(ky) +A32*DU2(ky) +A33*DU3(ky)
     *   .       + SIG*(U3(kx+1,ky,nl1) -2.*U3(kx,ky,nl1) +U3(kx-1,ky,nl1))
     *  8 CONTINUE
     */
    ntest = 8;

    do {
       nl1 = 0;
        nl2 = 1;
        for ( kx=1 ; kx<3 ; kx++ ){
/* #pragma nohazard */
            for ( ky=1 ; ky<n ; ky++ ) {
               du1[ky] = u1[nl1][ky+1][kx] - u1[nl1][ky-1][kx];
               du2[ky] = u2[nl1][ky+1][kx] - u2[nl1][ky-1][kx];
               du3[ky] = u3[nl1][ky+1][kx] - u3[nl1][ky-1][kx];
               u1[nl2][ky][kx]=
                  u1[nl1][ky][kx]+a11*du1[ky]+a12*du2[ky]+a13*du3[ky] + sig*
                     (u1[nl1][ky][kx+1]-2.0*u1[nl1][ky][kx]+u1[nl1][ky][kx-1]);
               u2[nl2][ky][kx]=
                  u2[nl1][ky][kx]+a21*du1[ky]+a22*du2[ky]+a23*du3[ky] + sig*
                     (u2[nl1][ky][kx+1]-2.0*u2[nl1][ky][kx]+u2[nl1][ky][kx-1]);
               u3[nl2][ky][kx]=
                  u3[nl1][ky][kx]+a31*du1[ky]+a32*du2[ky]+a33*du3[ky] + sig*
                     (u3[nl1][ky][kx+1]-2.0*u3[nl1][ky][kx]+u3[nl1][ky][kx-1]);
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 9 -- integrate predictors
     *******************************************************************
     *    DO 9  L = 1,Loop
     *    DO 9  i = 1,n
     *    PX( 1,i)= DM28*PX(13,i) + DM27*PX(12,i) + DM26*PX(11,i) +
     *   .          DM25*PX(10,i) + DM24*PX( 9,i) + DM23*PX( 8,i) +
     *   .          DM22*PX( 7,i) +  C0*(PX( 5,i) +      PX( 6,i))+ PX( 3,i)
     *  9 CONTINUE
     */
    ntest = 9;

    do {
       for ( i=0 ; i<n ; i++ ) {
            px[i][0] = dm28*px[i][12] + dm27*px[i][11] + dm26*px[i][10] +
                       dm25*px[i][ 9] + dm24*px[i][ 8] + dm23*px[i][ 7] +
                       dm22*px[i][ 6] + c0*( px[i][ 4] + px[i][ 5]) + px[i][ 2];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 10 -- difference predictors
     *******************************************************************
     *    DO 10  L= 1,Loop
     *    DO 10  i= 1,n
     *    AR      =      CX(5,i)
     *    BR      = AR - PX(5,i)
     *    PX(5,i) = AR
     *    CR      = BR - PX(6,i)
     *    PX(6,i) = BR
     *    AR      = CR - PX(7,i)
     *    PX(7,i) = CR
     *    BR      = AR - PX(8,i)
     *    PX(8,i) = AR
     *    CR      = BR - PX(9,i)
     *    PX(9,i) = BR
     *    AR      = CR - PX(10,i)
     *    PX(10,i)= CR
     *    BR      = AR - PX(11,i)
     *    PX(11,i)= AR
     *    CR      = BR - PX(12,i)
     *    PX(12,i)= BR
     *    PX(14,i)= CR - PX(13,i)
     *    PX(13,i)= CR
     * 10 CONTINUE
     */
    ntest = 10;

    do {
       for ( i=0 ; i<n ; i++ ) {
            ar        =      cx[i][ 4];
            br        = ar - px[i][ 4];
            px[i][ 4] = ar;
            cr        = br - px[i][ 5];
            px[i][ 5] = br;
            ar        = cr - px[i][ 6];
            px[i][ 6] = cr;
            br        = ar - px[i][ 7];
            px[i][ 7] = ar;
            cr        = br - px[i][ 8];
            px[i][ 8] = br;
            ar        = cr - px[i][ 9];
            px[i][ 9] = cr;
            br        = ar - px[i][10];
            px[i][10] = ar;
            cr        = br - px[i][11];
            px[i][11] = br;
            px[i][13] = cr - px[i][12];
            px[i][12] = cr;
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 11 -- first sum
     *******************************************************************
     *    DO 11 L = 1,Loop
     *        X(1)= Y(1)
     *    DO 11 k = 2,n
     * 11     X(k)= X(k-1) + Y(k)
     */
    ntest = 11;

    do {
       x[0] = y[0];
        for ( k=1 ; k<n ; k++ ) {
            x[k] = x[k-1] + y[k];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 12 -- first difference
     *******************************************************************
     *    DO 12 L = 1,Loop
     *    DO 12 k = 1,n
     * 12     X(k)= Y(k+1) - Y(k)
     */
    ntest = 12;

    do {
       for ( k=0 ; k<n ; k++ ) {
            x[k] = y[k+1] - y[k];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 13 -- 2-D PIC (Particle In Cell)
     *******************************************************************
     *    DO  13     L= 1,Loop
     *    DO  13    ip= 1,n
     *              i1= P(1,ip)
     *              j1= P(2,ip)
     *              i1=        1 + MOD2N(i1,64)
     *              j1=        1 + MOD2N(j1,64)
     *         P(3,ip)= P(3,ip)  + B(i1,j1)
     *         P(4,ip)= P(4,ip)  + C(i1,j1)
     *         P(1,ip)= P(1,ip)  + P(3,ip)
     *         P(2,ip)= P(2,ip)  + P(4,ip)
     *              i2= P(1,ip)
     *              j2= P(2,ip)
     *              i2=            MOD2N(i2,64)
     *              j2=            MOD2N(j2,64)
     *         P(1,ip)= P(1,ip)  + Y(i2+32)
     *         P(2,ip)= P(2,ip)  + Z(j2+32)
     *              i2= i2       + E(i2+32)
     *              j2= j2       + F(j2+32)
     *        H(i2,j2)= H(i2,j2) + 1.0
     * 13 CONTINUE
     */
    ntest = 13;

    do {
       for ( ip=0 ; ip<n ; ip++ ) {
            i1 = (long) p[ip][0];
            j1 = (long) p[ip][1];
            i1 &= 64-1;
            j1 &= 64-1;
            p[ip][2] += b[j1][i1];
            p[ip][3] += c[j1][i1];
            p[ip][0] += p[ip][2];
            p[ip][1] += p[ip][3];
            i2 = (long) p[ip][0];
            j2 = (long) p[ip][1];
            i2 = ( i2 & 64-1 ) - 1 ;
            j2 = ( j2 & 64-1 ) - 1 ;
            p[ip][0] += y[i2+32];
            p[ip][1] += z[j2+32];
            i2 += e[i2+32];
            j2 += f[j2+32];
            h[j2][i2] += 1.0;
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 14 -- 1-D PIC (Particle In Cell)
     *******************************************************************
     *    DO   14   L= 1,Loop
     *    DO   141  k= 1,n
     *          VX(k)= 0.0
     *          XX(k)= 0.0
     *          IX(k)= INT(  GRD(k))
     *          XI(k)= REAL( IX(k))
     *         EX1(k)= EX   ( IX(k))
     *        DEX1(k)= DEX  ( IX(k))
     *41  CONTINUE
     *    DO   142  k= 1,n
     *          VX(k)= VX(k) + EX1(k) + (XX(k) - XI(k))*DEX1(k)
     *          XX(k)= XX(k) + VX(k)  + FLX
     *          IR(k)= XX(k)
     *          RX(k)= XX(k) - IR(k)
     *          IR(k)= MOD2N(  IR(k),2048) + 1
     *          XX(k)= RX(k) + IR(k)
     *42  CONTINUE
     *    DO  14    k= 1,n
     *    RH(IR(k)  )= RH(IR(k)  ) + 1.0 - RX(k)
     *    RH(IR(k)+1)= RH(IR(k)+1) + RX(k)
     *14  CONTINUE
     */
    ntest = 14;

    do {
       for ( k=0 ; k<n ; k++ ) {
            vx[k] = 0.0;
            xx[k] = 0.0;
            ix[k] = (long) grd[k];
            xi[k] = (REAL) ix[k];
            ex1[k] = ex[ ix[k] - 1 ];
            dex1[k] = dex[ ix[k] - 1 ];
        }
        for ( k=0 ; k<n ; k++ ) {
            vx[k] = vx[k] + ex1[k] + ( xx[k] - xi[k] )*dex1[k];
            xx[k] = xx[k] + vx[k]  + flx;
            ir[k] = (long) xx[k];
            rx[k] = xx[k] - ir[k];
            ir[k] = ( ir[k] & (2048-1) ) + 1;
            xx[k] = rx[k] + ir[k];
        }
        for ( k=0 ; k<n ; k++ ) {
            rh[ ir[k]-1 ] += 1.0 - rx[k];
            rh[ ir[k]   ] += rx[k];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 15 -- Casual Fortran.  Development version
     *******************************************************************
     *      DO 45  L = 1,Loop
     *             NG= 7
     *             NZ= n
     *             AR= 0.053
     *             BR= 0.073
     * 15   DO 45  j = 2,NG
     *      DO 45  k = 2,NZ
     *             IF( j-NG) 31,30,30
     * 30     VY(k,j)= 0.0
     *                 GO TO 45
     * 31          IF( VH(k,j+1) -VH(k,j)) 33,33,32
     * 32           T= AR
     *                 GO TO 34
     * 33           T= BR
     * 34          IF( VF(k,j) -VF(k-1,j)) 35,36,36
     * 35           R= MAX( VH(k-1,j), VH(k-1,j+1))
     *              S= VF(k-1,j)
     *                 GO TO 37
     * 36           R= MAX( VH(k,j),   VH(k,j+1))
     *              S= VF(k,j)
     * 37     VY(k,j)= SQRT( VG(k,j)**2 +R*R)*T/S
     * 38          IF( k-NZ) 40,39,39
     * 39     VS(k,j)= 0.
     *                 GO TO 45
     * 40          IF( VF(k,j) -VF(k,j-1)) 41,42,42
     * 41           R= MAX( VG(k,j-1), VG(k+1,j-1))
     *              S= VF(k,j-1)
     *              T= BR
     *                 GO TO 43
     * 42           R= MAX( VG(k,j),   VG(k+1,j))
     *              S= VF(k,j)
     *              T= AR
     * 43     VS(k,j)= SQRT( VH(k,j)**2 +R*R)*T/S
     * 45    CONTINUE
     */
    ntest = 15;

/* #pragma intrinsic sqrt */

    do {
       ng = 7;
        nz = n;
        ar = 0.053;
        br = 0.073;
        for ( j=1 ; j<ng ; j++ ) {
            for ( k=1 ; k<nz ; k++ ) {
                if ( (j+1) >= ng ) {
                    vy[j][k] = 0.0;
                    continue;
                }
                if ( vh[j+1][k] > vh[j][k] ) {
                    t = ar;
                }
                else {
                    t = br;
                }
                if ( vf[j][k] < vf[j][k-1] ) {
                    if ( vh[j][k-1] > vh[j+1][k-1] )
                        r = vh[j][k-1];
                    else
                        r = vh[j+1][k-1];
                    s = vf[j][k-1];
                }
                else {
                    if ( vh[j][k] > vh[j+1][k] )
                        r = vh[j][k];
                    else
                        r = vh[j+1][k];
                    s = vf[j][k];
                }
                vy[j][k] = sqrt( vg[j][k]*vg[j][k] + r*r )* t/s;
                if ( (k+1) >= nz ) {
                    vs[j][k] = 0.0;
                    continue;
                }
                if ( vf[j][k] < vf[j-1][k] ) {
                    if ( vg[j-1][k] > vg[j-1][k+1] )
                        r = vg[j-1][k];
                    else
                        r = vg[j-1][k+1];
                    s = vf[j-1][k];
                    t = br;
                }
                else {
                    if ( vg[j][k] > vg[j][k+1] )
                        r = vg[j][k];
                    else
                        r = vg[j][k+1];
                    s = vf[j][k];
                    t = ar;
                }
                vs[j][k] = sqrt( vh[j][k]*vh[j][k] + r*r )* t / s;
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 16 -- Monte Carlo search loop
     *******************************************************************
     *          II= n/3
     *          LB= II+II
     *          k2= 0
     *          k3= 0
     *    DO 485 L= 1,Loop
     *           m= 1
     *405       i1= m
     *410       j2= (n+n)*(m-1)+1
     *    DO 470 k= 1,n
     *          k2= k2+1
     *          j4= j2+k+k
     *          j5= ZONE(j4)
     *          IF( j5-n      ) 420,475,450
     *415       IF( j5-n+II   ) 430,425,425
     *420       IF( j5-n+LB   ) 435,415,415
     *425       IF( PLAN(j5)-R) 445,480,440
     *430       IF( PLAN(j5)-S) 445,480,440
     *435       IF( PLAN(j5)-T) 445,480,440
     *440       IF( ZONE(j4-1)) 455,485,470
     *445       IF( ZONE(j4-1)) 470,485,455
     *450       k3= k3+1
     *          IF( D(j5)-(D(j5-1)*(T-D(j5-2))**2+(S-D(j5-3))**2
     *   .                        +(R-D(j5-4))**2)) 445,480,440
     *455        m= m+1
     *          IF( m-ZONE(1) ) 465,465,460
     *460        m= 1
     *465       IF( i1-m) 410,480,410
     *470 CONTINUE
     *475 CONTINUE
     *480 CONTINUE
     *485 CONTINUE
     */
    ntest = 16;

    ii = n / 3;
    lb = ii + ii;
    k3 = k2 = 0;
    do {
       i1 = m = 1;
label410:
       j2 = ( n + n )*( m - 1 ) + 1;
        for ( k=1 ; k<=n ; k++ ) {
            k2++;
            j4 = j2 + k + k;
            j5 = zone[j4-1];
            if ( j5 < n ) {
                if ( j5+lb < n ) {              /* 420 */
                    tmp = plan[j5-1] - t;       /* 435 */
                } else {
                    if ( j5+ii < n ) {          /* 415 */
                        tmp = plan[j5-1] - s;   /* 430 */
                    } else {
                        tmp = plan[j5-1] - r;   /* 425 */
                    }
                }
            } else if( j5 == n ) {
                break;                          /* 475 */
            } else {
                k3++;                           /* 450 */
                tmp=(d[j5-1]-(d[j5-2]*(t-d[j5-3])*(t-d[j5-3])+(s-d[j5-4])*
                              (s-d[j5-4])+(r-d[j5-5])*(r-d[j5-5])));
            }
            if ( tmp < 0.0 ) {
                if ( zone[j4-2] < 0 )           /* 445 */
                    continue;                   /* 470 */
                else if ( !zone[j4-2] )
                    break;                      /* 480 */
            } else if ( tmp ) {
                if ( zone[j4-2] > 0 )           /* 440 */
                    continue;                   /* 470 */
                else if ( !zone[j4-2] )
                    break;                      /* 480 */
            } else break;                       /* 485 */
            m++;                                /* 455 */
            if ( m > zone[0] )
                m = 1;                          /* 460 */
            if ( i1-m )                         /* 465 */
                goto label410;
            else
                break;
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 17 -- implicit, conditional computation
     *******************************************************************
     *          DO 62 L= 1,Loop
     *                i= n
     *                j= 1
     *              INK= -1
     *            SCALE= 5./3.
     *              XNM= 1./3.
     *               E6= 1.03/3.07
     *                   GO TO 61
     *60             E6= XNM*VSP(i)+VSTP(i)
     *          VXNE(i)= E6
     *              XNM= E6
     *           VE3(i)= E6
     *                i= i+INK
     *               IF( i.EQ.j) GO TO  62
     *61             E3= XNM*VLR(i) +VLIN(i)
     *             XNEI= VXNE(i)
     *          VXND(i)= E6
     *              XNC= SCALE*E3
     *               IF( XNM .GT.XNC) GO TO  60
     *               IF( XNEI.GT.XNC) GO TO  60
     *           VE3(i)= E3
     *               E6= E3+E3-XNM
     *          VXNE(i)= E3+E3-XNEI
     *              XNM= E6
     *                i= i+INK
     *               IF( i.NE.j) GO TO 61
     * 62 CONTINUE
     */
    ntest = 17;

    do {
       i = n-1;
        j = 0;
        ink = -1;
        scale = 5.0 / 3.0;
        xnm = 1.0 / 3.0;
        e6 = 1.03 / 3.07;
        goto l61;
l60:    e6 = xnm*vsp[i] + vstp[i];
        vxne[i] = e6;
        xnm = e6;
        ve3[i] = e6;
        i += ink;
        if ( i==j ) goto l62;
l61:    e3 = xnm*vlr[i] + vlin[i];
        xnei = vxne[i];
        vxnd[i] = e6;
        xnc = scale*e3;
        if ( xnm > xnc ) goto l60;
        if ( xnei > xnc ) goto l60;
        ve3[i] = e3;
        e6 = e3 + e3 - xnm;
        vxne[i] = e3 + e3 - xnei;
        xnm = e6;
        i += ink;
        if ( i != j ) goto l61;
l62:;

    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 18 - 2-D explicit hydrodynamics fragment
     *******************************************************************
     *       DO 75  L= 1,Loop
     *              T= 0.0037
     *              S= 0.0041
     *             KN= 6
     *             JN= n
     *       DO 70  k= 2,KN
     *       DO 70  j= 2,JN
     *        ZA(j,k)= (ZP(j-1,k+1)+ZQ(j-1,k+1)-ZP(j-1,k)-ZQ(j-1,k))
     *   .            *(ZR(j,k)+ZR(j-1,k))/(ZM(j-1,k)+ZM(j-1,k+1))
     *        ZB(j,k)= (ZP(j-1,k)+ZQ(j-1,k)-ZP(j,k)-ZQ(j,k))
     *   .            *(ZR(j,k)+ZR(j,k-1))/(ZM(j,k)+ZM(j-1,k))
     * 70    CONTINUE
     *       DO 72  k= 2,KN
     *       DO 72  j= 2,JN
     *        ZU(j,k)= ZU(j,k)+S*(ZA(j,k)*(ZZ(j,k)-ZZ(j+1,k))
     *   .                    -ZA(j-1,k) *(ZZ(j,k)-ZZ(j-1,k))
     *   .                    -ZB(j,k)   *(ZZ(j,k)-ZZ(j,k-1))
     *   .                    +ZB(j,k+1) *(ZZ(j,k)-ZZ(j,k+1)))
     *        ZV(j,k)= ZV(j,k)+S*(ZA(j,k)*(ZR(j,k)-ZR(j+1,k))
     *   .                    -ZA(j-1,k) *(ZR(j,k)-ZR(j-1,k))
     *   .                    -ZB(j,k)   *(ZR(j,k)-ZR(j,k-1))
     *   .                    +ZB(j,k+1) *(ZR(j,k)-ZR(j,k+1)))
     * 72    CONTINUE
     *       DO 75  k= 2,KN
     *       DO 75  j= 2,JN
     *        ZR(j,k)= ZR(j,k)+T*ZU(j,k)
     *        ZZ(j,k)= ZZ(j,k)+T*ZV(j,k)
     * 75    CONTINUE
     */
    ntest = 18;

    do {
        t = 0.0037;
        s = 0.0041;
        kn = 6;
        jn = n;
        for ( k=1 ; k<kn ; k++ ) {
/* #pragma nohazard */
          for ( j=1 ; j<jn ; j++ ) {
              za[k][j] = ( zp[k+1][j-1] +zq[k+1][j-1] -zp[k][j-1] -zq[k][j-1] )*
                         ( zr[k][j] +zr[k][j-1] ) / ( zm[k][j-1] +zm[k+1][j-1]);
              zb[k][j] = ( zp[k][j-1] +zq[k][j-1] -zp[k][j] -zq[k][j] ) *
                         ( zr[k][j] +zr[k-1][j] ) / ( zm[k][j] +zm[k][j-1]);
          }
        }
        for ( k=1 ; k<kn ; k++ ) {
/* #pragma nohazard */
            for ( j=1 ; j<jn ; j++ ) {
                zu[k][j] += s*( za[k][j]   *( zz[k][j] - zz[k][j+1] ) -
                                za[k][j-1] *( zz[k][j] - zz[k][j-1] ) -
                                zb[k][j]   *( zz[k][j] - zz[k-1][j] ) +
                                zb[k+1][j] *( zz[k][j] - zz[k+1][j] ) );
                zv[k][j] += s*( za[k][j]   *( zr[k][j] - zr[k][j+1] ) -
                                za[k][j-1] *( zr[k][j] - zr[k][j-1] ) -
                                zb[k][j]   *( zr[k][j] - zr[k-1][j] ) +
                                zb[k+1][j] *( zr[k][j] - zr[k+1][j] ) );
            }
        }
        for ( k=1 ; k<kn ; k++ ) {
/* #pragma nohazard */
            for ( j=1 ; j<jn ; j++ ) {
                zr[k][j] = zr[k][j] + t*zu[k][j];
                zz[k][j] = zz[k][j] + t*zv[k][j];
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 19 -- general linear recurrence equations
     *******************************************************************
     *               KB5I= 0
     *           DO 194 L= 1,Loop
     *           DO 191 k= 1,n
     *         B5(k+KB5I)= SA(k) +STB5*SB(k)
     *               STB5= B5(k+KB5I) -STB5
     *191        CONTINUE
     *192        DO 193 i= 1,n
     *                  k= n-i+1
     *         B5(k+KB5I)= SA(k) +STB5*SB(k)
     *               STB5= B5(k+KB5I) -STB5
     *193        CONTINUE
     *194 CONTINUE
     */
    ntest = 19;

    kb5i = 0;
    do {
       for ( k=0 ; k<n ; k++ ) {
            b5[k+kb5i] = sa[k] + stb5*sb[k];
            stb5 = b5[k+kb5i] - stb5;
        }
        for ( i=1 ; i<=n ; i++ ) {
            k = n - i ;
            b5[k+kb5i] = sa[k] + stb5*sb[k];
            stb5 = b5[k+kb5i] - stb5;
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 20 -- Discrete ordinates transport, conditional recurrence on xx
     *******************************************************************
     *    DO 20 L= 1,Loop
     *    DO 20 k= 1,n
     *         DI= Y(k)-G(k)/( XX(k)+DK)
     *         DN= 0.2
     *         IF( DI.NE.0.0) DN= MAX( S,MIN( Z(k)/DI, T))
     *       X(k)= ((W(k)+V(k)*DN)* XX(k)+U(k))/(VX(k)+V(k)*DN)
     *    XX(k+1)= (X(k)- XX(k))*DN+ XX(k)
     * 20 CONTINUE
     */
    ntest = 20;

    do {
       for ( k=0 ; k<n ; k++ ) {
            di = y[k] - g[k] / ( xx[k] + dk );
            dn = 0.2;
            if ( di ) {
                dn = z[k]/di ;
                if ( t < dn ) dn = t;
                if ( s > dn ) dn = s;
            }
            x[k] = ( ( w[k] + v[k]*dn )* xx[k] + u[k] ) / ( vx[k] + v[k]*dn );
            xx[k+1] = ( x[k] - xx[k] )* dn + xx[k];
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 21 -- matrix*matrix product
     *******************************************************************
     *    DO 21 L= 1,Loop
     *    DO 21 k= 1,25
     *    DO 21 i= 1,25
     *    DO 21 j= 1,n
     *    PX(i,j)= PX(i,j) +VY(i,k) * CX(k,j)
     * 21 CONTINUE
     */
    ntest = 21;

    do {
       for ( k=0 ; k<25 ; k++ ) {
            for ( i=0 ; i<25 ; i++ ) {
/* #pragma nohazard */
                for ( j=0 ; j<n ; j++ ) {
                    px[j][i] += vy[k][i] * cx[j][k];
                }
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 22 -- Planckian distribution
     *******************************************************************
     *     EXPMAX= 20.0
     *       U(n)= 0.99*EXPMAX*V(n)
     *    DO 22 L= 1,Loop
     *    DO 22 k= 1,n
     *                                          Y(k)= U(k)/V(k)
     *       W(k)= X(k)/( EXP( Y(k)) -1.0)
     * 22 CONTINUE
     */
    ntest = 22;

/* #pragma intrinsic exp */
    expmax = 20.0;
    u[n-1] = 0.99*expmax*v[n-1];
    do {
       for ( k=0 ; k<n ; k++ ) {
            y[k] = u[k] / v[k];
            w[k] = x[k] / ( exp( y[k] ) -1.0 );
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 23 -- 2-D implicit hydrodynamics fragment
     *******************************************************************
     *    DO 23  L= 1,Loop
     *    DO 23  j= 2,6
     *    DO 23  k= 2,n
     *          QA= ZA(k,j+1)*ZR(k,j) +ZA(k,j-1)*ZB(k,j) +
     *   .          ZA(k+1,j)*ZU(k,j) +ZA(k-1,j)*ZV(k,j) +ZZ(k,j)
     * 23  ZA(k,j)= ZA(k,j) +.175*(QA -ZA(k,j))
     */
    ntest = 23;

    do {
       for ( j=1 ; j<6 ; j++ ) {
            for ( k=1 ; k<n ; k++ ) {
                qa = za[j+1][k]*zr[j][k] + za[j-1][k]*zb[j][k] +
                     za[j][k+1]*zu[j][k] + za[j][k-1]*zv[j][k] + zz[j][k];
                za[j][k] += 0.175*( qa - za[j][k] );
            }
        }
    } while( test( ntest ) > 0 );

    /*
     *******************************************************************
     *   Kernel 24 -- find location of first minimum in array
     *******************************************************************
     *     X( n/2)= -1.0E+10
     *    DO 24  L= 1,Loop
     *           m= 1
     *    DO 24  k= 2,n
     *          IF( X(k).LT.X(m))  m= k
     * 24 CONTINUE
     */
    ntest = 24;

    x[n/2] = -1.0e+10;
    do {
       m = 0;
        for ( k=1 ; k<n ; k++ ) {
            if ( x[k] < x[m] ) m = k;
        }
    } while( test( ntest ) > 0 );

    /*
     *   Epilogue
     */

    for ( k=0 ; k<mk ; k++ ) {
        times[k][il-1][jr-1] = time[k];
        fopn[k][il-1][jr-1]  = flopn[k];
        terrs[k][il-1][jr-1] = terr1[k];
        npfs[k][il-1][jr-1]  = npfs1[k];
        csums[k][il-1][jr-1] = csum[k];
        dos[k][il-1][jr-1]   = total[k];
    }

    TK[0] = 0.0;
    TK[1] = 0.0;
    for ( k=0 ; k<mk ; k++ ) {
                        TK[0] += time[k];
                        TK[1] += flopn[k]*total[k];
    }

    track( "kernel  " );

    return;
}

#undef SET_VARS
#include "c_cernel.h"

/* This function executes kernel 12. It is placed in this file
   so that it will be optimized the same way as the kernels used 
   in the benchmark timings. In the original version of the loops,
   this loop was inline in the timing verification routine and 
   was run at reduced optimization when there were difficulties with
   the I/O routines. The result was a serious underestimate of
   the number of passes needed to get reasonable elapsed times.
*/
double kernel_12(long lo, long n, REAL *x)
{
  long k, m;
  double sum= 0.0;

  /* the sum accumulated below is intended to prevent
     over-clever optimization. */
  for (m=1 ; m<=lo ; m++) {
    /* the following loop has the minus ones to mimic the
       macro used elsewhere in the program to get Fortran-like
       indexing */
    for (k=1 ; k<=n ; k++) {
      x[k-1]= x[k+1-1] - x[k-1];
    }
    sum += x[n-1];
  }
  return sum;
}

/* End of File */

