/*
 * TEST slalom
 * DESC Scalable Language-independent Ames Laboratory One-minute Measurement
 * CATEGORY misc
 * END
 *
 * WARNING: when you move this file edit the next line
 */

#define GEOM "slalom.geom"

/******************************************************************************
			       S L A L O M

    Scalable Language-independent Ames Laboratory One-minute Measurement

     The following program is the first benchmark based on fixed time rather
  than fixed problem comparison.  Not only is fixed time more representative
  of the way people use computers, it also greatly increases the scope and
  longevity of the benchmark.  SLALOM is very scalable, and can be used to
  compare computers as slow as 126 floating-point operations per second
  (FLOPS) to computers running a trillion times faster.  The scalability can
  be used to compare single processors to massively parallel collections
  of processors, and to study the space of problem size vs. ensemble size
  in fine detail.  It resembles the LINPACK benchmark since it involves
  factoring and backsolving a (nearly) dense matrix, but incorporates a
  number of improvements to that benchmark that we hope will make SLALOM
  a better reflection of general system performance.

     The SLALOM benchmark solves a complete, real problem (optical radiosity
  on the interior of a box), not a contrived kernel or a synthetic mixture of
  sample operations.  SLALOM is unusual since it times input, problem setup,
  solution, and output, not just the solution.  For slower computers, the
  problem setup will take the majority of the time; it grows as the square of
  the problem size.  The solver grows as the cube of the problem size, and
  dominates the time for large values of n.

     While the following is C, you are free to translate it into any
  language you like, including assembly language specific to one computer.
  You may use compiler directives, hand-tuned library calls, loop unrolling,
  and even change the algorithm, if you can provide a convincing argument
  that the program still works for the full range of possible inputs.  For
  example, if you replace the direct solver with an iterative one, you must
  make sure your method is correct even when the geometry is quite eccentric
  and the box faces are highly reflective. (rho = .999)

     The main() driver should be used with the value of 60 seconds for the
  SLALOM benchmark.  The work done for a particular problem size is figured
  after timing has ceased, so there is no overhead for work assessment.  The
  residual check ||Ax - b|| is also done after timing has ceased.  Two
  computers may be compared either by their problem size n, or by their MFLOPS
  rate, never by the ratio of execution times.  Times will always be near one
  minute in SLALOM.  We have used the following weights for floating-point
  operation counting, based on the weights used by Lawrence Livermore National
  Laboratory:

			OPERATION                       WEIGHT
		    a=b, a=(constant)                      0
	    a<0, a<=0, a==0, a!=0, a>0, a>=0               0
		 -a, fabs(a), fsgn(a, b)                   0
		   a+b, a-b, a*b, a^2                      1
	    a<b, a<=b, a==b, a!=b, a>b, a>=b               1
		   (int) a, (double)b                      1
			1/a, -1/a                          3
			   a/b                             4
			  sqrt(a)                          4
	       Format to or from ASCII string              6
       sin(a), cos(a), tan(a), log(a), atan(a), exp(a)     8

     We invite you to share with us the results of any measurements that you
  make with SLALOM.  We do NOT accept anonymous data; machine timings will be
  referenced and dated.

     The least you need to do to adapt SLALOM to your computer is:

	1.  In the "Measure" routine, set NMAX to a value large enough to keep
	    the computer working for a minute.  Vary it slightly if it helps
	    (for reasons of cache size, interleaving, etc.)

	2.  Replace the timer call in "When" with the most accurate wall-clock
	    timer at your disposal.  If only CPU time is available, try to run
	    the job standalone or at high priority, since we are ultimately
	    interested in the top of the statistical range of performance.

	3.  Edit in the information specific to your test in the "What"
	    routine, so that final output will be automatically annotated.

	4.  Compile, link, and run the program, interacting to select values
	    of n that bracket a time of one minute.  Once everything is
	    running, run it as a batch job so as to record the session.

     Examples of ways you may optimize performance:

	1.  Unroll the loops in SetUp1 and SetUp2; it is possible to
	    vectorize both SetUp1 and SetUp2 at the cost of some extra
	    operations, program complexity, and storage.

	2.  Replace the innermost loops of Solver with calls to well-tuned
	    libraries of linear algebra routines, such as DDOT from the
	    Basic Linear Algebra Subroutines (level 1 BLAS).  Better still,
	    use a tuned library routine for all of Solver; the sparsity
	    exploited in Solver is only a few percent, so you will usually
	    gain more than you lose by applying a dense symmetric solver.

	3.  Parallelize the SetUp and Solver routines; all are highly
	    parallel.  Each element of the matrix can be constructed
	    independently, once each processor knows the geometry and part of
	    the partitioning into regions.  A substantial body of literature
	    now exists for performing the types of operations in Solver in
	    parallel.

	4.  Overlap computation with output.  Once the Region routine is done,
	    the first part of the output file (patch geometry) can be written
	    while the radiosities are being calculated.

     Examples of what you may NOT do:

	1.  The tuning must not be made specific to the particular input
	    provided.  For example, you may not eliminate IF tests simply
	    because they always come out the same way for this input; you
	    may not use precomputed answers or table look-up unless those
	    answers and tables cover the full range of possible inputs; and
	    you may not exploit symmetry for even values of the problem size.

	2.  You may not disable the self-consistency tests in SetUp3 and
	    Verify, nor alter their tolerance constants.

	3.  You may not change the input or output files to unformatted
	    binary or other format that would render them difficult to create
	    or read for humans.

	4.  You may not eliminate the reading of the "geom" file by putting
	    its data directly into the compiled program.

	5.  You may not change any of the work assessments in Meter.  If you
	    use more floating-point operations than indicated, you must still
	    use the assessments provided.  If you find a way to use fewer
	    operations and still get the job done for arbitrary input
	    parameters, please tell us!

			  -John Gustafson, Diane Rover, Michael Carter,
			   and Stephen Elbert
			   Ames Laboratory, Ames, Iowa 50011
******************************************************************************/

/*****************************************************************************/
/*  The following program finds a value n such that a problem of size n      */
/*  takes just under "goal" seconds to execute.                              */
/*                                                                           */
/*  John Gustafson, Diane Rover, Michael Carter, and Stephen Elbert          */
/*  Ames Laboratory, 3/18/90                                                 */
/*                                                                           */
/*  Calls:  Meter   Measures execution time for some application.            */
/*          What    Prints work-timing statistics and system information.    */
/*****************************************************************************/

#include				<stdio.h>
#include				<math.h>
#include <bench.h>

/* NMAX = Largest npatch for your computer; adjust as needed. */
#define		NMAX		1024
#define		EPS			(0.5e-8)
#define		FALSE		(1==0)
#define		TRUE		(!FALSE)
#define		MAX(a,b)	(((a) > (b)) ? (a) : (b))

/* Global variables and function return types: */
double 	goal,	/* User input, fixed-time benchmark goal, in seconds. */
	timing,		/* Elapsed time returned by Meter routine, in seconds.*/
	work,		/* In this case, number of FLOPs performed.           */
	When(),		/* Wall clock in seconds.                             */
	Ddot();		/* Double dot product.                                */
int		mean,	/* Avg between upper and lower bounds for bisection   */
                /* method.                                            */
	n,			/* The problem size.                                  */
	nupper,		/* Upper bound on problem size, used in iterating     */
                /* toward goal.                                       */
	Meter(),	/* Driver for following benchmark functions.          */
	Reader (),	/* Reads problem description from 'geom' file.        */
	Region (),	/* Subdivides box faces into patches.                 */
	SetUp3 (),	/* Set up matrix to solve.                            */
	Storer (),	/* Write result to 'answer' file.                     */
	Verify ();	/* Verify the radiosity solution from solver.         */
void	SetUp1 (),	/* Set up matrix to solve.                            */
	SetUp2 (),	/* Set up matrix to solve.                            */
	Solver ();	/* Solve the radiosity matrix.                        */



/*****************************************************************************/
/*  This routine measures time required on a revised LINPACK-type benchmark, */
/*  including input, matrix generation, solution, and output.                */
/*                                                                           */
/*  John Gustafson, Diane Rover, Michael Carter, and Stephen Elbert          */
/*  Ames Laboratory, 3/18/90                                                 */
/*                                                                           */
/*  Calls: Reader  Reads the problem description from secondary storage.     */
/*         Region  Partitions box surface into rectangular regions (patches).*/
/*         SetUp1  Sets up equations from patch geometries-parallel faces.   */
/*         SetUp2  Sets up equations from patch geometries-orthogonal faces. */
/*         SetUp3  Sets up equations-row normalization and radiant props.    */
/*         Solver  Solves the equations by LDL factorization.                */
/*         Storer  Stores solution (patch radiosities) on secondary storage. */
/*         When    Returns wall-clock time, in seconds.                      */
/*****************************************************************************/

double wor[100];
Meter (void)
{
	int npatch=16;
	double *work=wor;
	static
		double	area[NMAX],	/* Areas of patches * 8 * pi.                */
		box[7],				/* Dimensions of box in x, y, z directions.  */
		coeff[NMAX][NMAX],	/* The coefficients of the eqns to solve.    */
		diag[3][NMAX],		/* Diag terms of the eqns to solve. (RGB)    */
		emiss[6][3],		/* (RGB) emissivities of patches.            */
		place[3][NMAX],		/* Width-height-depth position of patches.   */
		result[3][NMAX],	/* Answer radiosities (RGB).                 */
		rho[6][3],			/* (RGB) Reflectivities of patches.          */
		rhs[3][NMAX],		/* Right-hand sides of eqns to solve (RGB).  */
		size[2][NMAX];		/* Width-height sizes of patches.            */
	double	ops[8],			/* Floating-point operation counts.          */
		p[6],				/* Number of patches in faces.               */
		sec[8],				/* Times for routines, in seconds.           */
		tmp1, tmp2;			/* Double temporary variables.               */
	int		i,				/* Loop counter.                             */
		itmp1,				/* Integer temporary variable.               */
		non0;				/* Index of first nonzero off-diagonal elem. */
	static
		int		loop[6][2];			/* Patch number ranges for faces.            */

	/* First check that npatch lies between 6 and NMAX: */
	if (npatch < 6) {
		printf ("Must be at least 6, the number of faces.\n");
		return (FALSE);
	}
	else if (npatch > NMAX) {
		printf ("Exceeds %d = maximum for this system.\n", NMAX);
		return (FALSE);
	}

	/* Ensure that previous 'answer' file is deleted: */
	/*unlink ("answer");*/

	/* Time the tasks, individually and collectively.  */
	if (!Reader (box, rho, emiss))
		return (FALSE);
	if (!Region (npatch, loop, box, place, size, area))
		return (FALSE);
	SetUp1 (npatch, loop, coeff, place, size);
	SetUp2 (npatch, loop, coeff, place, size);
	if (!SetUp3 (npatch, loop, area, rho, emiss, coeff, diag, rhs))
		return (FALSE);
	non0 = loop[1][0];
	Solver (npatch, non0, coeff, diag, rhs, result);

	/* Assess floating-point work done by each routine called, and total: */
	/* Note the ops counts are talleyed into a double array, and there    */
	/* some strange casts to double in some equations.  This is to        */
	/* prevent integer overflow.                                          */
	itmp1 = 0;
	tmp1 = 0.0;
	for (i = 0 ; i < 6 ; i++) {
		p[i] = loop[i][1] - loop[i][0] + 1;
		tmp1 += p[i] * p[i];
		itmp1 += sqrt(p[i] * box[i] / box[i + 1]) + 0.5;
	}
	tmp2 = p[0] * p[3] + p[1] * p[4] + p[2] * p[5];
	ops[0] = 258;
	ops[1] = 154 + (double) 8 * itmp1 + npatch;
	ops[2] = 6 + 532 * tmp2;
	ops[3] = 8*npatch + 370 * ((double) npatch * npatch - tmp1 - 2*tmp2) / 2.0;
	ops[4] = 72 + (double) 9 * npatch + (double) npatch * npatch - tmp1;
	ops[5] = npatch * (npatch * ((double) npatch + 7.5) - 2.5) - 21
		+ (non0+1) * ((non0+1) * (2 * ((double) non0+1) - 16.5) + 35.5)
		+ (non0+1) * npatch * (9 - 3 * ((double) non0+1));
	ops[6] = 48 * npatch;
	*work = ops[0] + ops[1] + ops[2] + ops[3] + ops[4] + ops[5] + ops[6];

	return (TRUE);
}

/*****************************************************************************/
/* The following routine reads in the problem description from secondary     */
/* storage, and checks that numbers are in reasonable ranges.                */
/*****************************************************************************/
Reader (box, rho, emiss)
double	box[],			/* Out: Dimensions of box in x, y, z directions.  */
	rho[][3],		/* Out: (RGB) Reflectivities of patches.          */
	emiss[][3];		/* Out: (RGB) emissivities of patches.            */
{
	/*
	 *  Local variables:
	 *    infile  Device number for input file.
	 *    i, j    Loop counters.
	 *    tmp1    Maximum emissivity, to check that emissivities are not all 0.
	 */
	int		i, j,		/* Loop counters.                            */
		n;			/* Number of args fscanf()'ed from file.     */
	double	tmp1;	/* Maximum emissivity.                       */

#if defined(OLD_CODE)
	
	FILE	*infile;	/* Input file pointer.                       */
	char	buff[81];	/* Buffer used to eat a line of input.       */


	/* Open the input file and read in the data. */
	if ((infile = fopen (GEOM, "r")) == NULL) {
		printf ("slalom: '" GEOM "' geometry file not found.\n");
		exit (1);
	}

	/* Read the box coordinates and error check. */
	n = 0;
	for (i = 0 ; i < 3 ; i++) {
		n += fscanf (infile, "%lg", &box[i]);
		printf("box[%d] = %lg;\n", i, box[i]);
	}
	fgets (buff, 80, infile);		/* Eat the rest of the line. */
	if (n != 3) {
		printf ("Must specify exactly 3 box coordinates.\n");
		exit(1);
	}

	/* Read the reflectivities and error check. */
	n = 0;
	for (j = 0 ; j < 3 ; j++) {
		for (i = 0 ; i < 6 ; i++) {
			n += fscanf (infile, "%lg", &rho[i][j]);
			printf("rho[%d][%d] = %lg;\n", i, j, rho[i][j]);
		}
	}
	fgets (buff, 80, infile);		/* Eat the rest of the line. */
	if (n != 18) {
		printf ("Must specify exactly 18 box coordinates.\n");
		exit(1);
	}

	/* Read the emissivities and error check. */
	n = 0;
	for (j = 0 ; j < 3 ; j++) {
		for (i = 0 ; i < 6 ; i++) {
			n += fscanf (infile, "%lg", &emiss[i][j]);
			printf("emiss[%d][%d] = %lg;\n", i, j, emiss[i][j]);
		}
	}
	fgets (buff, 80, infile);		/* Eat the rest of the line. */
	if (n != 18) {
		printf ("Must specify exactly 18 box coordinates.\n");
		exit(1);
	}
	fclose (infile);
#else
	box[0] = 13.5;
	box[1] = 9;
	box[2] = 8;
	rho[0][0] = 0.8;
	rho[1][0] = 0.99;
	rho[2][0] = 0.54;
	rho[3][0] = 0.84;
	rho[4][0] = 0.01;
	rho[5][0] = 0.84;
	rho[0][1] = 0.8;
	rho[1][1] = 0.01;
	rho[2][1] = 0.54;
	rho[3][1] = 0.84;
	rho[4][1] = 0.01;
	rho[5][1] = 0.84;
	rho[0][2] = 0.8;
	rho[1][2] = 0.01;
	rho[2][2] = 0.54;
	rho[3][2] = 0.84;
	rho[4][2] = 0.99;
	rho[5][2] = 0.84;
	emiss[0][0] = 1.27;
	emiss[1][0] = 0;
	emiss[2][0] = 0;
	emiss[3][0] = 0;
	emiss[4][0] = 0;
	emiss[5][0] = 0;
	emiss[0][1] = 1.27;
	emiss[1][1] = 0;
	emiss[2][1] = 0;
	emiss[3][1] = 0;
	emiss[4][1] = 0;
	emiss[5][1] = 0;
	emiss[0][2] = 1.27;
	emiss[1][2] = 0;
	emiss[2][2] = 0;
	emiss[3][2] = 0;
	emiss[4][2] = 0;
	emiss[5][2] = 0;
#endif



	/* Now sanity-check the values that were just read. */
	for (j = 0 ; j < 3 ; j++) {
		if (box[j] < 1.0 || box[j] >= 100.0) {
			printf ("Box dimensions must be between 1 and 100.\n");
			return (FALSE);
		}
		box[j+3] = box[j];

		tmp1 = 0.0;
		for (i = 0 ; i < 6 ; i++) {
			if (rho[i][j] < 0.000 || rho[i][j] > 0.999) {
				printf ("Reflectivities must be between .000 and .999.\n");
				return (FALSE);
			}
			if (emiss[i][j] < 0.0) {
				printf ("Emissivity cannot be negative.\n");
				return (FALSE);
			}
			if (tmp1 < emiss[i][j])
				tmp1 = emiss[i][j];
		}
		if (tmp1 == 0.0) {
			printf ("Emissivities are zero.  Problem is trivial.\n");
			return (FALSE);
		}
	}
	box[6] = box[3];
	return (TRUE);
}

/*****************************************************************************/
/* The following routine decomposes the surface of a variable-sized box      */
/* into patches that are as nearly equal in size and square as possible.     */
/*****************************************************************************/
Region (npatch, loop, box, place, size, area)
int		npatch,			/* In: Problem size.                             */
	loop[][2];		/* Out: Patch number ranges for faces.           */
double	area[],			/* Out: 8pi * areas of the patches.              */
	box[],			/* In: Dimensions of box in x, y, z directions.  */
	place[][NMAX],	/* Out: Width-height-depth positions of patches. */
	size[][NMAX];	/* Out: Width-height sizes of patches.           */
{


	int		icol,	/* Loop counter over the number of columns. */
		ipatch,	/* Loop counter over the number of patches. */
		iface,	/* Loop counter over the number of faces.   */
		itmp1,	/* Integer temporary variables.             */
		itmp2,	/* Integer temporary variables.             */
		last,	/* Inner loop ending value.                 */
		lead,	/* Inner loop starting value.               */
		numcol,	/* Number of columns on faces.              */
		numpat,	/* Number of patches on a face.             */
		numrow;	/* Number of rows of patches in a column.   */
	double	height,	/* Height of a patch within a column.       */
		tmp1,	/* double temporary variables.              */
		tmp2,	/* double temporary variables.              */
		tmp3,	/* double temporary variables.              */
		tmp4,	/* double temporary variables.              */
		width;	/* Width of a column of patches.            */

	/* Allocate patches to each face, proportionate to area of each face. */
	tmp1 = 2.0 * (box[0] * box[1] + box[1] * box[2] + box[2] * box[0]);
	tmp2 = 0.0;
	tmp3 = npatch;
	loop[0][0] = 0;
	for (iface = 0 ; iface < 5 ; iface++) {
		tmp2 = tmp2 + box[iface] * box[iface + 1];
		loop[iface][1] = (int) (tmp3 * tmp2 / tmp1 + 0.5) - 1;
		loop[iface + 1][0] = loop[iface][1] + 1;
	}
	loop[5][1] = npatch - 1;

	/* Subdivide each face into numpat patches. */
	for (iface = 0 ; iface < 6 ; iface++) {
		numpat = loop[iface][1] - loop[iface][0] + 1;
		tmp3 = 0.0;
		if (iface >= 3)
			tmp3 = box[iface-1];
		numcol = (int) (sqrt(numpat * box[iface] / box[iface + 1]) + 0.5);
		if (numcol > numpat)
			numcol = numpat;
		if (numcol == 0)
			numcol = 1;
		width = box[iface] / numcol;
		itmp1 = numcol - 1;
		tmp1 = 0.0;
		for (icol = 0 ; icol < numcol ; icol++) {
			itmp2 = itmp1 / numcol;
			numrow = (itmp1 + numpat) / numcol - itmp2;
			if (numrow == 0) {
				printf ("Eccentric box requires more patches.\n");
				return (FALSE);
			}
			height = box[iface + 1] / numrow;
			tmp2 = 0.0;
			tmp4 = width * height * (8.0 * M_PI);
			lead = loop[iface][0] + itmp2;
			last = lead + numrow;
			for (ipatch = lead ; ipatch < last ; ipatch++) {
				size[0][ipatch] = width;
				size[1][ipatch] = height;
				place[0][ipatch] = tmp1;
				place[1][ipatch] = tmp2;
				place[2][ipatch] = tmp3;
				area[ipatch] = tmp4;
				tmp2 = tmp2 + height;
			}
			tmp1 = tmp1 + width;
			itmp1 = itmp1 + numpat;
		}
	}

	return (TRUE);
}

/*****************************************************************************/
/* This routine sets up the radiosity matrix for parallel patches.           */
/*****************************************************************************/
void
SetUp1 (npatch, loop, coeff, place, size)
	int		npatch,			/* In: Problem size.                             */
	loop[][2];		/* In: Patch number ranges for faces.            */
	double	coeff[][NMAX],	/* Out: The coefficients of the eqns to solve.   */
	place[][NMAX],		/* In: Width-height-depth positions of patches.  */
	size[][NMAX];		/* In: Width-height sizes of patches.            */
{
	int		i, j, k,	/* General loop counters.                            */
		m, n,		/* General loop counters.                            */
		iface,		/* Loop counter over the number of faces.            */
		ipatch,		/* Loop counter over the number of patches.          */
		jface,		/* Face coupled to iface when computing mat. elems.  */
		jpatch;		/* Patch coupled to ipatch when computing mat. elems.*/
	double	d[2][2][2],	/* Point-to-point couplings between patch corners.   */
		d2[2][2][2],/* Squares of d values, to save recomputation.       */
		tmp1, tmp2,	/* Double temporary variables.                       */
		tmp3, tmp4,	/* Double temporary variables.                       */
		tmp5, tmp6,	/* Double temporary variables.                       */
		tmp7, tmp8;	/* Double temporary variables.                       */

	for (iface = 0 ; iface < 3 ; iface++) {
		jface = iface + 3;
		tmp1 = place[2][loop[jface][0]] * place[2][loop[jface][0]];
		tmp6 = tmp1 + tmp1;
		for (ipatch = loop[iface][0] ; ipatch <= loop[iface][1] ; ipatch++) {
			for (jpatch=loop[jface][0] ; jpatch <= loop[jface][1] ; jpatch++) {
				for (j = 0 ; j < 2 ; j++) {
					d [0][0][j] = place[j][jpatch] - place[j][ipatch];
					d [1][0][j] = d[0][0][j] + size[j][jpatch];
					d [0][1][j] = d[0][0][j] - size[j][ipatch];
					d [1][1][j] = d[1][0][j] - size[j][ipatch];
					d2[0][0][j] = d[0][0][j] * d[0][0][j];
					d2[1][0][j] = d[1][0][j] * d[1][0][j];
					d2[0][1][j] = d[0][1][j] * d[0][1][j];
					d2[1][1][j] = d[1][1][j] * d[1][1][j];
				}
				tmp2 = 0.0;
				for (m = 0 ; m < 2 ; m++) {
					for (i = 0 ; i < 2 ; i++) {
						tmp3 = d2[m][i][1] + tmp1;
						tmp4 = sqrt(tmp3);
						tmp5 = 1.0 / tmp4;
						tmp8 = 0.0;
						for (k = 0 ; k < 2 ; k++) {
							for (n = 0 ; n < 2 ; n++) {
								tmp7 = d[k][n][0];
								tmp8 = -tmp7 * atan(tmp7 * tmp5) - tmp8;
							}
							tmp8 = -tmp8;
						}
						tmp2 = -4.0 * tmp4 * tmp8 - tmp2 - tmp6 *
							log(((d2[1][0][0] + tmp3) * (d2[0][1][0] + tmp3)) /
							    ((d2[0][0][0] + tmp3) * (d2[1][1][0] + tmp3)));
					}
					tmp2 = -tmp2;
				}
				for (m = 0 ; m < 2 ; m++) {
					for (i = 0 ; i < 2 ; i++) {
						tmp4 = sqrt(d2[m][i][0] + tmp1);
						tmp5 = 1.0 / tmp4;
						tmp8 = 0.0;
						for (k = 0 ; k < 2 ; k++) {
							for (n = 0 ; n < 2 ; n++) {
								tmp7 = d[k][n][1];
								tmp8 = -tmp7 * atan(tmp7 * tmp5) - tmp8;
							}
							tmp8 = -tmp8;
						}
						tmp2 = -4.0 * tmp4 * tmp8 - tmp2;
					}
					tmp2 = -tmp2;
				}
				coeff[ipatch][jpatch] = tmp2;
				coeff[jpatch][ipatch] = tmp2;
			}
		}
	}
}

/*****************************************************************************/
/* This routine sets up the radiosity matrix for orthogonal patches.         */
/*****************************************************************************/
void
SetUp2 (npatch, loop, coeff, place, size)
	int		npatch,			/* In: Problem size.                             */
	loop[][2];		/* In: Patch number ranges for faces.            */
	double	coeff[][NMAX],	/* Out: The coefficients of the eqns to solve.   */
	place[][NMAX],	/* In: Width-height-depth positions of patches.  */
	size[][NMAX];	/* In: Width-height sizes of patches.            */
{
	int		m,			/* General loop counters.                            */
		iface,		/* Loop counter over the number of faces.            */
		ipatch,		/* Loop counter over the number of patches.          */
		jface,		/* Face coupled to iface when computing mat. elems.  */
		jpatch;		/* Patch coupled to ipatch when computing mat. elems.*/

	double	tmpb, tmpa,
		c11d, c12d, c21d, c22d, c11s, c12s, c21s, c22s,
		d11d, d12d, d21d, d22d, d11s, d12s, d21s, d22s,
		d11i, d12i, d21i, d22i, a10s, a20s, b01s, b02s,
		e1111, e1211, e2111, e2211, e1112, e1212, e2112, e2212,
		e1121, e1221, e2121, e2221, e1122, e1222, e2122, e2222;

	for (iface = 0 ; iface < 6 ; iface++) {
		for (m = 0 ; m < 2 ; m++) {
			jface = (iface + m + 1) % 6;
			for (ipatch=loop[iface][0] ; ipatch <= loop[iface][1] ; ipatch++) {
				a10s = place[m][ipatch] - place[2][loop[jface][0]];
				a20s = a10s + size[m][ipatch];
				a10s = a10s * a10s;
				a20s = a20s * a20s;
				for (jpatch=loop[jface][0] ; jpatch<=loop[jface][1];jpatch++) {
					c11d = place[m][jpatch] - place[1-m][ipatch];
					c12d = c11d + size[m][jpatch];
					c21d = c11d - size[1-m][ipatch];
					c22d = c12d - size[1-m][ipatch];
					c11s = c11d * c11d;
					c12s = c12d * c12d;
					c21s = c21d * c21d;
					c22s = c22d * c22d;
					b01s = place[1 - m][jpatch] - place[2][ipatch];
					b02s = b01s + size[1 - m][jpatch];

					/**/
					/* Bump the term by a small real to avoid
					   /* singularities in coupling function:
					   /**/
					b01s = b01s * b01s + 1e-35;
					b02s = b02s * b02s + 1e-35;
					d11s = a10s + b01s;
					d12s = a10s + b02s;
					d21s = a20s + b01s;
					d22s = a20s + b02s;
					d11d = sqrt(d11s);
					d12d = sqrt(d12s);
					d21d = sqrt(d21s);
					d22d = sqrt(d22s);
					d11i = 1.0 / d11d;
					d12i = 1.0 / d12d;
					d21i = 1.0 / d21d;
					d22i = 1.0 / d22d;

					tmpa =	  d11d * ( c11d * atan (c11d * d11i)
						           - c12d * atan (c12d * d11i)
						           - c21d * atan (c21d * d11i)
						           + c22d * atan (c22d * d11i))
						+ d12d * (-c11d * atan (c11d * d12i)
						          + c12d * atan (c12d * d12i)
						          + c21d * atan (c21d * d12i)
						          - c22d * atan (c22d * d12i))
						+ d21d * (-c11d * atan (c11d * d21i)
						          + c12d * atan (c12d * d21i)
						          + c21d * atan (c21d * d21i)
						          - c22d * atan (c22d * d21i))
						+ d22d * ( c11d * atan (c11d * d22i)
						           - c12d * atan (c12d * d22i)
						           - c21d * atan (c21d * d22i)
						           + c22d * atan (c22d * d22i));

					e1111 = c11s + d11s;
					e1211 = c12s + d11s;
					e2111 = c21s + d11s;
					e2211 = c22s + d11s;
					e1112 = c11s + d12s;
					e1212 = c12s + d12s;
					e2112 = c21s + d12s;
					e2212 = c22s + d12s;
					e1121 = c11s + d21s;
					e1221 = c12s + d21s;
					e2121 = c21s + d21s;
					e2221 = c22s + d21s;
					e1122 = c11s + d22s;
					e1222 = c12s + d22s;
					e2122 = c21s + d22s;
					e2222 = c22s + d22s;

					tmpb =    c11s * log( e1111 * e1122 / (e1112 * e1121))
						- c12s * log( e1211 * e1222 / (e1212 * e1221))
						- c21s * log( e2111 * e2122 / (e2112 * e2121))
						+ c22s * log( e2211 * e2222 / (e2212 * e2221))
						- d11s * log( e1111 * e2211 / (e1211 * e2111))
						+ d12s * log( e1112 * e2212 / (e1212 * e2112))
						+ d21s * log( e1121 * e2221 / (e1221 * e2121))
						- d22s * log( e1122 * e2222 / (e1222 * e2122));

					coeff[ipatch][jpatch] = fabs(4.0 * tmpa + tmpb);
					coeff[jpatch][ipatch] = coeff[ipatch][jpatch];
				}
			}
		}
	}
}

/*****************************************************************************/
/* This routine sets up the radiosity matrix... normalizes row sums to 1,    */
/* and includes terms derived from reflectivites and emissivities of faces.  */
/*****************************************************************************/
SetUp3 (npatch, loop, area, rho, emiss, coeff, diag, rhs)
int		npatch,			/* In: Problem size.                                 */
	loop[][2];		/* In: Patch number ranges for faces.                */
double	area[],			/* In: 8 * pi * areas of the patches.                */
	rho[][3],		/* In: (RGB) Reflectivities of the face interiors.   */
	emiss[][3],		/* In: (RGB) Emissivities of the face interiors.     */
	coeff[][NMAX],	/* Out: The coefficients of the eqns to solve.       */
	diag[][NMAX],	/* Out: (RGB) Diagonal terms of the system.          */
	rhs[][NMAX];	/* Out: (RGB) Right-hand sides of system to solve.   */
{

	/*
	 *  Local variables:
	 *    iface     Loop counter over the number of faces.
	 *    ipatch    Outer loop counter over the number of patches.
	 *    j         Loop counter over each color (R-G-B).
	 *    jpatch    Inner loop counter over the number of patches.
	 *    tmp1      double temporary variable.
	 *    vtmp1-2   double vector temporary variables.
	 */
	int		j,			/* (RGB) Loop counter over each color.               */
		iface,		/* Loop counter over the number of faces.            */
		ipatch,		/* Outer loop counter over the number of patches.    */
		jpatch;		/* Inner loop counter over the number of patches.    */
	double	tmp1,		/* Double temporary variable.                        */
		vtmp1[3],	/* Double vector temporary variables.                */
		vtmp2[3];	/* Double vector temporary variables.                */

	/* Ensure that row sums to 1, and put in reflectivities (rho) and        */
	/* emissivities.                                                         */
	for (iface = 0 ; iface < 6 ; iface++) {
		for (j = 0 ; j < 3 ; j++) {
			vtmp1[j] = 1.0 / rho[iface][j];
			vtmp2[j] = emiss[iface][j] * vtmp1[j];
		}
		for (ipatch = loop[iface][0] ; ipatch <= loop[iface][1] ; ipatch++) {
			tmp1 = 0.0;
			for (jpatch = 0 ; jpatch < loop[iface][0] ; jpatch++) {
				tmp1 += coeff[ipatch][jpatch];
			}
			for (jpatch = loop[iface][1]+1 ; jpatch < npatch ; jpatch++) {
				tmp1 += coeff[ipatch][jpatch];
			}
			/* Make sure row sum (total form factor) is close to 1: */
			if (fabs(tmp1 - area[ipatch]) > (0.5e-9 * tmp1)) {
				printf ("Total form factor is too far from unity.\n");
				return (FALSE);
			}
			tmp1 = -tmp1;
			/* Set coplanar patch interactions to zero. */
			for (jpatch=loop[iface][0] ; jpatch <= loop[iface][1] ; jpatch++) {
				coeff[ipatch][jpatch] = 0.0;
			}
			/* Assign diagonal entries and right-hand sides. */
			for (j = 0 ; j < 3 ; j++) {
				diag[j][ipatch] = vtmp1[j] * tmp1;
				rhs[j][ipatch] = vtmp2[j] * tmp1;
			}
		}
	}
	return (TRUE);
}

/*****************************************************************************/
/* This routine factors and backsolves a real, symmetric, near-dense matrix  */
/* by LDL factorization.  No pivoting; the matrix is diagonally dominant.    */
/*****************************************************************************/
void
Solver (npatch, non0, coeff, diag, rhs, result)
	int		npatch,			/* In: Problem size.                                 */
	non0;			/* In: Index of first nonzero off-diagonal mat. elem.*/
	double	coeff[][NMAX],	/* In/Out: The coefficients of the eqns to solve.    */
	diag[][NMAX],	/* Out: (RGB) Diagonal terms of the system.          */
	rhs[][NMAX],	/* In: (RGB) Right-hand sides of system to solve.    */
	result[][NMAX];	/* Out: (RGB) solution radiosities.                  */
{
	int		i, j,		/* General loop counters.     */
		    k, m;		/* General loop counters.     */
	double	tmp1;		/* Double temporary variable. */

	/* Load lower triangle of coefficients, diagonal, and solution vector. */
	for (m = 0 ; m < 3 ; m++) {
		for (i = non0 ; i < npatch ; i++) {
			coeff[i][i] = diag[m][i];
			result[m][i] = rhs[m][i];
			for (j = 0 ; j < i ; j++) {
				coeff[i][j] = coeff[j][i];
			}
		}

		/* Factor matrix, writing factors on top of original matrix. */
		for (j = 0 ; j < non0 ; j++) {
			coeff[j][j] = 1.0 / diag[m][j];
			result[m][j] = rhs[m][j];
		}

		for (j = non0 ; j < npatch ; j++) {
			for (k = non0 ; k < j ; k++) {
				coeff[j][k] -= Ddot (k, &coeff[k][0], 1, &coeff[j][0], 1);
			}
			for (k = 0 ; k < j ; k++) {
				tmp1 = coeff[j][k];
				coeff[j][k] = tmp1 * coeff[k][k];
				coeff[j][j] -= tmp1 * coeff[j][k];
			}
			coeff[j][j] = 1.0 / coeff[j][j];
		}

		/* Backsolve, in three stages (for L, D, and L transpose). */
		for (k = non0 ; k < npatch ; k++) {
			result[m][k] -= Ddot (k, &result[m][0], 1, &coeff[k][0], 1);
		}

		for (k = 0 ; k < npatch ; k++) {
			result[m][k] *= coeff[k][k];
		}

		for (k = npatch - 2 ; k >= non0 ; k--) {
			result[m][k] -= Ddot (npatch-(k+1), &result[m][k+1], 1,
			                      &coeff[k+1][k], NMAX);
		}

		for (k = non0 - 1 ; k >= 0 ; k--) {
			result[m][k] -= Ddot (npatch-non0, &result[m][non0], 1,
			                      &coeff[non0][k], NMAX);
		}
	}
}

/*****************************************************************************/
/* This routine verifies that the computed radiosities satisfy the equations.*/
/*                                                                           */
/*  John Gustafson, Diane Rover, Michael Carter, and Stephen Elbert          */
/*  Ames Laboratory, 3/18/90                                                 */
/*****************************************************************************/
Verify (npatch, coeff, diag, rhs, result)
int		npatch;			/* In: Problem size.                                 */
double	coeff[][NMAX],	/* In: The coefficients of the eqns to solve.        */
	diag[][NMAX],	/* In: (RGB) Diagonal terms of the system.           */
	rhs[][NMAX],	/* In: (RGB) Right-hand sides of system to solve.    */
	result[][NMAX];	/* In: (RGB) Radiosity solutions.                    */
{
	double	tmp1, tmp2;	/* Double temporary variables. */
	double	anorm,		/* Norm accumulation variable. */
		xnorm;		/* Norm accumulation variable. */
	int		i, j, m;	/* General loop counters.      */

	tmp1 = 0.0;
	for (m = 0 ; m < 3 ; m++) {
		/* Copy lower triangle of coefficients to upper triangle, */
		/* and load diagonal.                                     */
		for (i = 0 ; i < npatch ; i++) {
			coeff[i][i] = diag[m][i];
			for (j = 0 ; j < i ; j++) {
				coeff[i][j] = coeff[j][i];
			}
		}
		/* Multiply matrix by solution vector, and accum. norm of residual. */
		anorm = xnorm = 0.0;
		for (j = 0 ; j < npatch ; j++) {
			tmp2 = rhs[m][j];
			for (i = 0 ; i < npatch ; i++) {
				tmp2 -= (coeff[j][i] * result[m][i]);
				anorm = MAX(anorm, fabs(coeff[j][i]));
			}
			xnorm = MAX(xnorm, fabs(result[m][j]));
			tmp1 += fabs(tmp2);
		}
	}
	/* printf ("anorm = %g  xnorm = %g\n", anorm, xnorm); */
	tmp1 /= (anorm * xnorm);
	if (tmp1 > 3 * EPS) {
		printf ("Residual is too large: %lg\n", tmp1);
		return (FALSE);
	}
	return (TRUE);
}

#ifdef		SUN4

/*****************************************************************************/
/* Double precision dot product specifically written for Sun 4/370.          */
/* By Michael Carter and John Gustafson, May 30, 1990                        */
/* This code unrolls the dot product four ways since that's how many         */
/* registers are available on the SPARC.  Other RISC system will require     */
/* something very similar.  Also, unit stride is take advantage of in the    */
/* form of special cases.                                                    */
/*****************************************************************************/
double
Ddot (n, a, ia, b, ib)
	register
	int		n,		/* Number of elements in vectors.  */
	ia,		/* Stride of a vector in ELEMENTS. */
	ib;		/* Stride of b vector in ELEMENTS. */
	register
	double	*a,		/* Pointer to first vector.        */
	*b;		/* Pointer to second vector.       */
{
	register double	sum0 = 0.0,
		sum1 = 0.0,
		sum2 = 0.0,
		sum3 = 0.0;
	register int	m = n & 3;
	int				t;

	/* The ragged cleanup part. */
	while (m--) {
		sum0 += *a * *b;
		a += ia;
		b += ib;
	}

	/* The fast pipelined part */
	n >>= 2;
	if (ib == 1 && ia != 1) {
		t = ia;
		ia = ib;
		ib = t;
		t = (int) a;
		b = a;
		a = (double *) t;
	}

	/* We can optimize if one or more strides are equal to 1. */
	if (ia == 1) {
		/* This runs if both strides are 1. */
		if (ib == 1) {
			ia <<= 2;
			ib <<= 2;
			while (n--) {
				sum0 += a[0] * b[0];
				sum1 += a[1] * b[1];
				sum2 += a[2] * b[2];
				sum3 += a[3] * b[3];
				a += ia;
				b += ib;
			}
		}
		/* This runs if stride of a only is equal to 1. */
		else {
			ia <<= 2;
			while (n--) {
				sum0 += a[0] * *b;
				b += ib;
				sum1 += a[1] * *b;
				b += ib;
				sum2 += a[2] * *b;
				b += ib;
				sum3 += a[3] * *b;
				a += ia;
				b += ib;
			}
		}
	}
	/* This runs for the more general case.        */
	/* This is about .5 MFLOPS slower on Sun 4/370 */
	else {
		while (n--) {
			sum0 += *a * *b;
			a += ia;
			b += ib;
			sum1 += *a * *b;
			a += ia;
			b += ib;
			sum2 += *a * *b;
			a += ia;
			b += ib;
			sum3 += *a * *b;
			a += ia;
			b += ib;
		}
	}

	return (sum0 + sum1 + sum2 + sum3);
}

#else

/*****************************************************************************/
/* Generic double-precision dot product.  Unrolling will help pipelined      */
/* computers.  Modify accordingly.                                           */
/*****************************************************************************/
double
Ddot (n, a, ia, b, ib)
	register
	int		n,		/* Number of elements in vectors.  */
	ia,		/* Stride of a vector in ELEMENTS. */
	ib;		/* Stride of b vector in ELEMENTS. */
	register
	double	*a,		/* Pointer to first vector.        */
	*b;		/* Pointer to second vector.       */
{
	register double sum = 0.0;

	while (n--) {
		sum += *a * *b;
		a += ia;
		b += ib;
	}
	return (sum);
}

#endif


void slalom(void)
{
	int i = 0;
	for (i = 0; i < WORKLOAD; i++)
	{
		Meter();
	}
}

SINGLE_BENCHMARK(slalom())
