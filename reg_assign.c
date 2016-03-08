/*
TEST reg_assign
DESC register assignment and advanced cse test
SOURCE based in an idea by Scott A Crosby <crosby@qwes.math.cmu.edu>
CAT integer
DEF TYPE={char,short,int,long,long long}
CAT fp
DEF TYPE={float,double}
END
*/

/* Marc Lehmann <pcg@goof.com> */

#include <bench.h>

TYPE a[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

TYPE reg_assign1_8 (void)
{
  TYPE b[7];
  
  b[0] = 1*a[1] + 2*a[2] + 3*a[3] + 4*a[4] + 5*a[5] + 6*a[6] + 7*a[7];
  b[1] = 0*a[1] + 1*a[2] + 2*a[3] + 3*a[4] + 4*a[5] + 5*a[6] + 6*a[7];
  b[2] = 0*a[1] + 0*a[2] + 1*a[3] + 2*a[4] + 3*a[5] + 4*a[6] + 5*a[7];
  b[3] = 0*a[1] + 0*a[2] + 0*a[3] + 1*a[4] + 2*a[5] + 3*a[6] + 4*a[7];
  b[4] = 0*a[1] + 0*a[2] + 0*a[3] + 0*a[4] + 1*a[5] + 2*a[6] + 3*a[7];
  b[5] = 0*a[1] + 0*a[2] + 0*a[3] + 0*a[4] + 0*a[5] + 1*a[6] + 2*a[7];
  b[6] = 0*a[1] + 0*a[2] + 0*a[3] + 0*a[4] + 0*a[5] + 0*a[6] + 1*a[7];
  
  return b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6];
}



void reg_assign(void)
{
	int i = 0;
	int res = 0;
	int loops = WORKLOAD;
	
	for (i = 0; i < loops; i++)
	{	
		res += reg_assign1_8();
	}
	USE(res);
}


SINGLE_BENCHMARK (reg_assign())
