#include <iostream>
#include <stdlib.h>
#include "cudaTest.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

__global__ void test()
{

	return;
}


void doSmth()
{
	std::cout << "Hi from doSmth" << std::endl;;
	test << <1, 1 >> >();
	return;
}