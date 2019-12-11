#include "cudaTest.cuh"

__global__ void test()
{

	return;
}


void doSmth()
{
	std::cout << "Hi from doSmth" << std::endl;;
	test << < 1, 1 >> > ();
	return;
}