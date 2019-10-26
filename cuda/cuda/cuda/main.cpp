#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>


__global__ void test()
{
	// Empty Kernel
}


int main(int argc, char** argv)
{
	int threadCount = 4;

#pragma omp parallel num_threads(threadCount)
	{
		int myRank = omp_get_thread_num();
		int threadCount1 = omp_get_num_threads();
		printf("Hi, i am Thread %d out of %d\n", myRank + 1, threadCount1);
	}

	test << <1, 1 >> >();



	return 0;
}