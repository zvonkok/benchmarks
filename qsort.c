/* Simple implementation of classical quicksort algorithm */

/* TEST quicksort
* DESCRIPTION simple implementation of the classical quicksort algorithm
*
* # this test is in more categories
* CATEGORY integer
* DEFINE TYPE={signed char,short,int,long,long long,unsigned char}
* DEFINE TYPE={unsigned short, unsigned int, unsigned long, unsigned long long}
*
* CATEGORY fp
* DEFINE TYPE={float,double,long double}
* END
*/

#include <bench.h>

typedef TYPE number_t;
#define SIZE WORKLOAD
number_t data[SIZE];

void
myqsort (int start, int end)
{
	number_t median = data[start];
	int pos = start + 1;
	number_t tmp;
	int i;
	if (end - start <= 1)
		return;
	for (i = pos; i <= end; i++)
		if (data[i] < median)
		{
			tmp = data[i];
			data[i] = data[pos];
			data[pos] = tmp;
			pos++;
		}
		pos--;
		tmp = data[start];
		data[start] = data[pos];
		data[pos] = tmp;
		myqsort (start, pos - 1);
		myqsort (pos + 1, end);
}


void qsort(void)
{
	int i;

	for (i = 0; i < SIZE; i++)
		data[i] = i;

	myqsort (0, SIZE - 1);
}

SINGLE_BENCHMARK(qsort())
