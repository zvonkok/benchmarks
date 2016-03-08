#         __   __
#         \/   \/ __ __   __ ________  TM
#         ||_  || \/ \ \  \/ |/~||~\|
#         ||-\-|| || ||\\ ||    ||
#         ||  ~|| || || \\||    ||
#         /\   /\ /\ /\  \ |    /\
#         ~~   ~~ ~~ ~~   \/    ~~
#
#  Creating HINT(tm)
#      1. Change ARCH to reflect your architecture. We currently support
#            unix      serial version only - general unix machine
#            NCUBE2S   requires mimd code
#            NCUBE2    requires mimd code
#            PARAGON   requires mimd code
#            SGI       shared memory parallel version - requires Power C
#            KSR       Kendall Square - requires shared memory version
#            MP1       MasPar 1 - requires SIMD version
#            MP2       MasPar 2 - requires SIMD version
#         note: If your architecture is not present, you will have to add a 
#               define and the architecture specific code within the HINT 
#               source code. See the README.porting and the comments within
#               the source code for help.
#      2. Set DATATYPE to the appropriate C data type to run your test on.
#         We have found that setting DATATYPE equal to DOUBLE is large enough
#         to sufficiently exercise most architectures. For integer testing, you
#         need to use at least a 32 bit integer. Supported data types are:
#            DOUBLE     double
#            FLOAT      float
#            LONGLONG   long long
#            LONG       long
#            INT        int
#      3. Set CC to the appropriate compiler.
#      4. Set the various FLAGS variables appropriately. 
#          The  "-D$(ARCH) -D$(DATATYPE)"  _MUST_ be present on both the 
#          KERN_CFLAGS and the DRIV_CFLAGS.
#       5. Save the Makefile and type 'make' to create the executables.
#          To run the benchmark type 'make run'.
#
# Modified 2003 by Moritz Franosch (mail@Franosch.org)


ARCH       = unix

DRIV_SRC   = hint.c
KERN_SRC   = hkernel.c
INCLUDES_PATH   = hint.h typedefs.h
INCLUDES   = 
TARGET     = DOUBLE INT FLOAT 

CC         = gcc
KERN_CFLAGS= 
DRIV_CFLAGS= 
CFLAGS     = -O3 -DDEBUG
# -O2 -fomit-frame-pointer -finline-functions -funroll-loops

LFLAGS     = -lm 

targets:    $(TARGET)


#run:    $(TARGET) $(DATAFILE)


#rerun:
#		/bin/rm -f  $(DATAFILE)
#		make run

clean:
		/bin/rm -f  $(TARGET) hint.o hkernel.o

clobber:
		/bin/rm -f  $(DATAFILE)
		/bin/rm -f  $(TARGET)



#
# This section of the Makefile is for compiling the binaries
#

FLAGS = $(CFLAGS) $(DRIV_SRC) $(KERN_SRC) -D$(ARCH) $(LFLAGS) -DIINT
# take -DILONG for accessing more than 2 GiB RAM, otherwise -DIINT


DOUBLE: $(DRIV_SRC) $(INCLUDES_PATH) Makefile
	$(CC) $(FLAGS) -DDOUBLE -o DOUBLE


INT: $(DRIV_SRC) $(INCLUDES_PATH) Makefile
	$(CC) $(FLAGS) -DINT -o INT


FLOAT: $(DRIV_SRC) $(INCLUDES_PATH) Makefile
	$(CC) $(FLAGS) -DFLOAT -o FLOAT


SHORT: $(DRIV_SRC) $(INCLUDES_PATH) Makefile
	$(CC) $(FLAGS) -DSHORT -o SHORT


LONGLONG: $(DRIV_SRC) $(INCLUDES_PATH) Makefile
	$(CC) $(FLAGS) -DLONGLONG -o LONGLONG


#
# This section of the Makefile is for displaying the data with xmgrace
#

display%: data%
	xmgrace -block $</DOUBLE -bxy 5:2 -block $</FLOAT -bxy 5:2 -block $</INT -bxy 5:2 -par xmgrace.par -par $</description.par

results/%.jpg: data.%
	xmgrace -block $</DOUBLE -bxy 5:2 -block $</FLOAT -bxy 5:2 -block $</INT -bxy 5:2 -par xmgrace.par -par $</description.par -hdevice JPEG -hardcopy -printfile $@
