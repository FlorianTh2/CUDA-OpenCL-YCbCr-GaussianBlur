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


//// d_paddedImage: speicherallokierung mit ->  paddedIWidth * paddedIHeight * sizeof(float)
//const T* d_f,
//
//// paddedIWidth = iWidth + 2 * hFilterSize // hFilterSize = filterSize / 2 = eig radius des kernels
//const unsigned int paddedW,
//
//
//// paddedIHeight = iHeight + 2 * hFilterSize // hFilterSize = filterSize / 2 = eig radius des kernels
//const unsigned int paddedH,
//
//// radius of filter, später zusammengetragen mit filterSize=(2S+1)×(2S+1)(2S+1)×(2S+1).
//const int S,
//
//// d_filteringResult speicherallokierung mit // iWidth * iHeight * sizeof(float)
//T* d_h,
//
//// image widthe
//const unsigned int W,
//
////image height
//const unsigned int H )


__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, double** filter, int dataSize, int imageHeight, int imageWidth, int filterHeight)
{

	int filterWidth = filterHeight;
	int newImageHeight = imageHeight - filterHeight + 1;
	int newImageWidth = imageWidth - filterHeight + 1;


		for (int i = 0; i < newImageWidth; i++) {
			for (int j = 0; j < newImageHeight; j++) {
				for (int h = i; h < i + filterWidth; h++) {
					for (int w = j; w < j + filterHeight; w++) {
						dev_dataResult[i* imageWidth + j] = dev_dataResult[i * imageWidth + j] + filter[h - i][w - j] * dev_data[h * imageWidth + w];
					}
				}
			}
		}

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



unsigned char* gaussianOneChannel(unsigned char * data, int dataSize, dim3 gridDims, dim3 blockDims, double** filter, int imageHeight, int imageWidth, int filterHeight)
{
	int newImageWidth = imageWidth - filterHeight + 1;
	int newImageHeight = imageHeight - filterHeight + 1;
	int dataSizeResultImage = newImageWidth * newImageHeight;
	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSizeResultImage);

	unsigned char* dev_data;
	unsigned char* dev_dataResult;


	cudaMalloc(&dev_data, sizeof(unsigned char) * dataSize/3);
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSizeResultImage);
	cudaMemcpy(dev_data, data, sizeof(unsigned char) * dataSize / 3, cudaMemcpyHostToDevice);
	dev_applyGaussian << < gridDims, blockDims >> > (dev_data, dev_dataResult, filter, dataSize, imageHeight, imageWidth, filterHeight);

	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		// print the CUDA error message and exit
		printf("CUDA error: %s\n", cudaGetErrorString(error));
		exit(-1);
	}


	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSizeResultImage, cudaMemcpyDeviceToHost);
	cudaFree(&dev_data);
	cudaFree(&dev_dataResult);
	return dataResult;
}

double** createGaussianFilter(int width, int height, double sigma)
{
	double PI = 3.1415;

	double** kernel = (double**) malloc(height * sizeof(double *));

	for (int i = 0; i < height; i++) {
		kernel[i] = (double*) malloc(sizeof(double) * width);
	}


	double sum = 0.0;
	int a, b;

	for (a = 0; a < height; a++) {

		for (b = 0; b < width; b++) {
			cout << "hier ";
			double result = exp(-(a * a + b * b) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
			cout << "result: " << result << endl;
			kernel[a][b] = result;
			sum += kernel[a][b];
		}
	}

	for (a = 0; a < height; a++) {

		for (b = 0; b < width; b++) {
			kernel[a][b] /= sum;
		}
	}

	cout << "sum: " << sum << endl;

	//for (a = 0; a < width; a++)
	//{
	//	for (b = 0; b < height; b++)
	//	{
	//		cout << (double) kernel[a][b] << " ";
	//	}
	//	cout << endl;
	//}

	return kernel;
}

// data: BGR-Sequence of the input channels of data
unsigned char** applyGaussianFilter(unsigned char** data, const int dataSize, dim3 gridDims, dim3 blockDims, const int channelsPara, int imageHeight, int imageWidth, int filterHeight, double sigma)
{
	cout << dataSize << endl;
	const int channels = channelsPara;

	const int sizeOfOneColorChannel = dataSize / 3;

	unsigned char** resultChannels1 = (unsigned char**)malloc(channels * sizeof(unsigned char*));

	double** filter = createGaussianFilter(filterHeight, filterHeight, sigma);

	for (int i = 0; i < channels; i++) {
		resultChannels1[i] = (unsigned char*) malloc(sizeof(unsigned char) * sizeOfOneColorChannel);
		resultChannels1[i] = gaussianOneChannel(data[i], dataSize, gridDims, blockDims, filter, imageHeight, imageWidth, filterHeight);
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

