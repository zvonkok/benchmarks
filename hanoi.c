/*
  TEST hanoi
  DESC tower of hanoi - highly recursive
  CAT integer
  DEF TYPE={signed char,short,int,long,long long,unsigned char}
  DEF TYPE={unsigned short, unsigned int, unsigned long, unsigned long long}
  END
*/
#include <bench.h>
#define CYLINDERS WORKLOAD
typedef TYPE type;
static type ncylinders[3];
static type cylinders[3][CYLINDERS];
static type
move (type n, TYPE source, TYPE destination)
{
	type sum = 0;
	type free = 0;
	if (free == source)
		free++;
	if (free == destination)
		free++;
	if (free == source)
		free++;
	if (n)
		sum += move (n - 1, source, free);
	/*printf ("%i from %i to %i (stack %i,%i)\n", cylinders[source][ncylinders[source] - 1], source, destination, ncylinders[source], ncylinders[destination]);*/
	cylinders[destination][ncylinders[destination]] = cylinders[source][ncylinders[source] - 1];
	ncylinders[destination]++;
	ncylinders[source]--;
	sum++;
	if (n)
		sum += move (n - 1, free, destination);
	return sum;
}
type solve ()
{
	type i = 0;
	for (i = 0; i < CYLINDERS; i++)
		cylinders[0][i] = CYLINDERS - i;
	ncylinders[0] = CYLINDERS;
	ncylinders[1] = 0;
	ncylinders[2] = 0;
	return (move (CYLINDERS - 1, 0, 1));
}

void hanoi(void)
{
	int i = 0;
	int res = 0;
	int loops = 10000;

	for (i = 0; i < loops; i++)
		res += solve();

	USE(res);
}

SINGLE_BENCHMARK (hanoi())
