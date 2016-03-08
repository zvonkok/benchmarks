# @
# @         @
# @             @       @  @@@@@@@  @ @@  @ @@ @@@    @@@@@   @ @@   @@@@@@@
# @         @   @       @ @       @ @@  @ @@  @   @  @     @  @@  @ @       @
# @         @    @     @  @@@@@@@@@ @     @   @   @ @       @ @     @@@@@@@@@
# @         @  @  @   @   @         @     @   @   @  @     @  @     @
# @@@@@@@@@  @@    @@@     @@@@@@@  @     @   @   @   @@@@@   @      @@@@@@@
#
# @       @                                    @@
# @      @                                      @
# @     @    @@@@@@@  @ @@  @ @@@@@    @@@@@@@  @  @@@@@@@
# @@@@@@    @       @ @@  @ @@     @  @       @ @ @
# @     @   @@@@@@@@@ @     @       @ @@@@@@@@@ @  @@@@@@@
# @      @  @         @     @       @ @         @         @
# @       @  @@@@@@@  @     @       @  @@@@@@@ @@@ @@@@@@@
#
# This makes the Livermore Kernels Benchmark ANSI C
# The executables are as follows:
#       clldouble - C (kernel) DOUBLE precision benchmark
#       cllfloat  - C (kernel) SINGLE precision benchmark
#
#________________________________________________________________________
#
# Modify the following section to fit your machine and compiler.
#
# FLOAT_TYPE is the desired floating point type (double or float)
#
# TIMER is the timer option appropriate to your system.
# Read the comments at the end of cflopio.c for detailed instructions.
# Use a more accurate timer than the BSD getrusage routine, the
# SYSV/POSIX times routine, or the ANSI C clock routine, if
# one is available.
#TIMER = -DSYSV_TIMER
TIMER = -DANSI_TIMER
#
#________________________________________________________________________
#
# The following section probably won't need any changes.
#
# What is your ANSI C compiler called....
CC= gcc
#
# Use the highest optimization flags your compiler can stand...
# NOTE: high levels of optimization may cause the compiler
# or linker to take nearly forever if applied to the driver 
# routines. The only place where optimization matters is
# for the kernels, so two compiler flags variables are provided.
#
COPTIMIZE= -O2 -malign-double -ffast-math -funroll-loops
CFLAGS= -DREAL=double 
#
CODE_NAME = clldouble
#
# If you need any special load flags put them here...
LDFLAGS=-lm
#
# Default make rules for C
.c.o :
	$(CC) $(CFLAGS) -c $<

HEADERS = c_base1.h c_cernel.h c_fake1.h c_order.h c_space3.h c_tau.h \
	  c_base2.h c_cksums.h c_fake2.h c_proof.h c_sysid.h \
	  c_baser.h c_debug.h c_iqrand.h c_rats.h c_tags.h

 
# This is the rule that make runs by default when you type make.
# It will make the benchmark and run it.
# The results are collected in a file whose name starts cllf or
# clld (for the float and double versions) followed by the 
# file name specified in the input file.
run : all
	./$(CODE_NAME)

all: $(CODE_NAME)

# The following will only be needed to check on what the
# pre-processor is doing. 
cpre: cflops.c cflopio.c ccernel.c block.c c_cernel.h
	$(CC) $(CFLAGS) -E cflops.c >cflops.cpre 
	$(CC) $(CFLAGS) -E cflopio.c >cflopio.cpre 
	$(CC) $(CFLAGS) -E ccernel.c >ccernel.cpre 
	$(CC) $(CFLAGS) -E block.c >block.cpre 

#
#       Pure C version of Livermore Loops benchmark.
$(CODE_NAME): cflops.o cflopio.o block.o ccernel.o
	$(CC) $(CFLAGS) $(COPTIMIZE) -o $(CODE_NAME) cflops.o cflopio.o block.o ccernel.o $(LDFLAGS)
#
block.o : block.c block.h cflopio.h c_cernel.h $(HEADERS)
	$(CC) ${CFLAGS} -c block.c

cflops.o : cflops.c cflops.h block.h cflopio.h c_cernel.h $(HEADERS)
	$(CC) ${CFLAGS} -c cflops.c

cflopio.o : cflopio.c cflopio.h cflops.h c_cernel.h $(HEADERS)
	$(CC) ${CFLAGS} $(TIMER) -c cflopio.c

ccernel.o : ccernel.c c_cernel.h Makefile
	$(CC) ${CFLAGS} $(COPTIMIZE) -c ccernel.c
#
# Clean up the junk, but leave the results...
clean:
	rm -f *.o cllfloat clldouble core
#
# Clean up everything
pristine:
	rm -f *.o cllfloat clldouble core clld.* cllf.* input Makefile
