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

// rgb->Ycbcr
__global__ void dev_convertColorSpace(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize)
{
	int channels = 3;
	//dataSize /= 3;
	// blockid * wieVieleBlöckeGesamt + (threadID*channels)
	int globalThreadId = (blockIdx.x * channels) * blockDim.x + (threadIdx.x*channels);

	// grid-stride loop
	for(int dataElement = globalThreadId; dataElement < (dataSize-(channels)); dataElement= dataElement+(gridDim.x * blockDim.x)*(channels))
	{
		unsigned char r = dev_data[dataElement + 0];
		unsigned char g = dev_data[dataElement +1];
		unsigned char b = dev_data[dataElement +2];
		//*r = 16+ (((*r << 6) + (*r << 1) + (*g << 7) + *g + (*b << 4) + (*b << 3) + *b) >> 8); // Y
		//*g= 128 + ((-((*r<<5)+(*r<<2)+(*r<<1))-((*g<<6)+(*g<<3)+(*g<<1))+(*b<<7)-(*b<<4))>>8); // Cb
		//*b = 128 + (((*r<<7)-(*r<<4)-((*g<<6)+(*g<<5)-(*g<<1))-((*b<<4)+(*b<<1)))>>8); // Cr
		//*(dev_dataResult + dataElement + 0) = *r;
		//*(dev_dataResult + dataElement + 1) = *g;
		//*(dev_dataResult + dataElement + 2) = *b;

		dev_dataResult[dataElement + 0] = 16 + (((r << 6) + (r << 1) + (g << 7) + g + (b << 4) + (b << 3) + b) >> 8); // Y
		dev_dataResult[dataElement + 1] = 128 + ((-((r << 5) + (r << 2) + (r << 1)) - ((g << 6) + (g << 3) + (g << 1)) + (b << 7) - (b << 4)) >> 8); // Cb
		dev_dataResult[dataElement + 2] = 128 + (((r << 7) - (r << 4) - ((g << 6) + (g << 5) - (g << 1)) - ((b << 4) + (b << 1))) >> 8); // Cr
	}
}

__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize)
{
	int channels = 3;
	for (size_t i = 0; i < dataSize; i++)
	{

	}

	*dev_dataResult = 1;



    //double stdv = 1.0;
    //double r, s = 2.0 * stdv * stdv;  // Assigning standard deviation to 1.0
    //double sum = 0.0;   // Initialization of sun for normalization
    //for (int x = -2; x <= 2; x++) // Loop to generate 5x5 kernel
    //{
    //    for(int y = -2; y <= 2; y++)
    //    {
    //        r = sqrt(x*x + y*y);
    //        gk[x + 2][y + 2] = (exp(-(r*r)/s))/(M_PI * s);
    //        sum += gk[x + 2][y + 2];
    //    }
    //}

    //for(int i = 0; i < 5; ++i) // Loop to normalize the kernel
    //    for(int j = 0; j < 5; ++j)
    //        gk[i][j] /= sum;

}


void doSmth()
{
	std::cout << "Hi from doSmth" << std::endl;
	test << < 1, 1 >> > ();
	return;
}

unsigned char * convertRGBToYCBCR(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	cout << dataSize << endl;
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



unsigned char* gaussianOneChannel(unsigned char * data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSize);
	unsigned char* dev_data;
	unsigned char* dev_dataResult;
	cudaMalloc(&dev_data, sizeof(unsigned char) * dataSize);
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSize);
	cudaMemcpy(dev_data, data, sizeof(unsigned char) * dataSize, cudaMemcpyHostToDevice);
	dev_applyGaussian << < gridDims, blockDims >> > (dev_data, dev_dataResult, dataSize);
	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSize, cudaMemcpyDeviceToHost);
	cudaFree(&dev_data);
	cudaFree(&dev_dataResult);
	return dataResult;
}



unsigned char** applyGaussianFilter(unsigned char** data, const int dataSize, dim3 gridDims, dim3 blockDims, const int channelsPara)
{
	cout << dataSize << endl;
	const int channels = channelsPara;

	const int sizeOfOneColorChannel = dataSize / 3;

	unsigned char** resultChannels1 = (unsigned char**)malloc(channels * sizeof(unsigned char*));

	for (int i = 0; i < channels; i++) {
		resultChannels1[i] = (unsigned char*) malloc(sizeof(unsigned char) * sizeOfOneColorChannel);
		resultChannels1[i] = gaussianOneChannel(data[i], dataSize, gridDims, blockDims);

	}
	
	
	
	cudaDeviceReset();



	return resultChannels1;
}




void cudaMain(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
{
	doSmth();
	convertRGBToYCBCR(data, dataSize, gridDims, blockDims);
	//applyGaussianFilter(data, dataSize, gridDims, blockDims);
}

