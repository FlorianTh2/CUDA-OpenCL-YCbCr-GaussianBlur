#include "cudaTest.cuh"

// cuda procedure
//
// gpu memory allocation
// copy cpu->gpu
// calculation- / kernel phase
// copy gpu->cpu
// gpu memory free

__global__ void test()
{

	return;
}

__global__ void dev_convertColorSpace(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize)
{


	*dev_dataResult = 1;

}

__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize)
{


	*dev_dataResult = 1;

}


void doSmth()
{
	std::cout << "Hi from doSmth" << std::endl;;
	test << < 1, 1 >> > ();
	return;
}

unsigned char * convertBGRToYCBCR(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	unsigned char* dataResult = (unsigned char*) malloc(sizeof(unsigned char) * dataSize);

	unsigned char* dev_data;
	unsigned char* dev_dataResult;
	cudaMalloc(&dev_data, sizeof(unsigned char) * dataSize);
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSize);

	cudaMemcpy(dev_data, data, sizeof(unsigned char) * dataSize, cudaMemcpyHostToDevice);

	dev_convertColorSpace <<< gridDims, blockDims >>> (dev_data, dev_dataResult, dataSize);

	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSize, cudaMemcpyDeviceToHost);

	cudaFree(&dev_data);
	cudaFree(&dev_dataResult);



	return dataResult;
}


unsigned char* applyGaussianFilter(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSize); // verändert sich die output-size?

	unsigned char* dev_data;
	unsigned char* dev_dataResult;

	cudaMalloc(&dev_data, sizeof(unsigned char) * dataSize);
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSize); // verändert sich die output-size?

	cudaMemcpy(dev_data, data, sizeof(unsigned char) * dataSize, cudaMemcpyHostToDevice);

	dev_applyGaussian << < gridDims, blockDims >> > (dev_data, dev_dataResult, dataSize);

	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSize, cudaMemcpyDeviceToHost);

	cudaFree(&dev_data);
	cudaFree(&dev_dataResult);

	return data;
}





void cudaMain(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	doSmth();
	convertBGRToYCBCR(data, dataSize, gridDims, blockDims);
	//applyGaussianFilter(data, dataSize, gridDims, blockDims);
}

