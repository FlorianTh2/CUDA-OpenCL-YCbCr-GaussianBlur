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

void convertBGRToYCBCR(unsigned char* data)
{






	return;
}

void cudaMain(unsigned char* data)
{
	doSmth();
	convertBGRToYCBCR(data);
}

