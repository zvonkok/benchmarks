/* $Id: fibo.c,v 1.2 1996/01/16 14:16:53 chris Exp $ */
/*************************************************************
*  fib
*	The standard benchmark that calculates the Fibonacci series
*/

#include <stdio.h>
#include <bench.h>


TYPE fib(TYPE x)
{
	if(x > 2)
	{
		return(fib(x - 1) + fib(x - 2));
	}
	else
	{
		return(1);
	}
}      

SINGLE_BENCHMARK(USE(fib(WORKLOAD)))
