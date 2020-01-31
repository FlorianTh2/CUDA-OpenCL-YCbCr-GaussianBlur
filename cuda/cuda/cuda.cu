#include "cuda.cuh"

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
	int globalThreadId = (blockIdx.x * channels) * blockDim.x + (threadIdx.x*channels);

	// grid-stride loop
	for(int dataElement = globalThreadId; dataElement < (dataSize-(channels)); dataElement= dataElement+(gridDim.x * blockDim.x)*(channels))
	{
		unsigned char r = dev_data[dataElement + 0];
		unsigned char g = dev_data[dataElement +1];
		unsigned char b = dev_data[dataElement +2];

		dev_dataResult[dataElement + 0] = 16 + (((r << 6) + (r << 1) + (g << 7) + g + (b << 4) + (b << 3) + b) >> 8); // Y
		dev_dataResult[dataElement + 1] = 128 + (((r << 7) - (r << 4) - ((g << 6) + (g << 5) - (g << 1)) - ((b << 4) + (b << 1))) >> 8); // Cb
		dev_dataResult[dataElement + 2] = 128 + ((-((r << 5) + (r << 2) + (r << 1)) - ((g << 6) + (g << 3) + (g << 1)) + (b << 7) - (b << 4)) >> 8); // Cr
	}
}


//__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight)
//{
//	int blockId = blockIdx.x + blockIdx.y * gridDim.x+ gridDim.x * gridDim.y * blockIdx.z;
//	int threadId = blockId * (blockDim.x * blockDim.y * blockDim.z)+ (threadIdx.z * (blockDim.x * blockDim.y))+ (threadIdx.y * blockDim.x) + threadIdx.x;
//	int currentIndex = threadId;
//
//	int imageYSource = currentIndex / imageWidth;
//	int imageXSource = currentIndex % imageWidth;
//
//	int cuttedAwayTotal = filterHeight / 2;
//
//	if (imageYSource <  cuttedAwayTotal || imageYSource >(imageHeight -1 -cuttedAwayTotal) || imageXSource < cuttedAwayTotal || imageXSource >(imageWidth -1 -cuttedAwayTotal))
//	{
//		return;
//	}
//
//	int newImageHeight = imageHeight - filterHeight+1;
//	int newImageWidth = imageWidth - filterHeight+1;
//
//	//height
//	for (int h = 0; h < filterHeight; h++)
//	{
//		//width
//		for (int w = 0; w < filterHeight; w++)
//		{
//			double tmp = filter[h * filterHeight + w] * dev_data[(imageYSource + h) * imageWidth + (imageXSource + w)];
//
//			dev_dataResult[(imageYSource- cuttedAwayTotal) * newImageWidth + (imageXSource- cuttedAwayTotal)] += tmp;
//		}
//	}
//
//
//	//// max = 7000000 with block- and grid-dim = 1
//	//for (int i = 0; i < 1; i++) {
//	//	dev_dataResult[0] = 1;
//	//}
//
//}

__global__ void dev_applyGaussianALL(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight)
{

	int channels = 3;

	int blockId = blockIdx.x + blockIdx.y * gridDim.x;
	int threadId = blockId * (blockDim.x * blockDim.y) + (threadIdx.y * blockDim.x) + threadIdx.x;
	int currentIndex = threadId;

	int imageYSource = currentIndex / (channels * imageWidth);
	int imageXSource = currentIndex % (channels * imageWidth);

	int cuttedAway = filterHeight / 2;

	int newImageHeight = imageHeight - filterHeight + 1;
	int newImageWidth = imageWidth - filterHeight + 1;

	int currentChannel = currentIndex % channels;


	if (!(imageYSource < cuttedAway || imageYSource >(imageHeight - 1 - cuttedAway) || imageXSource < cuttedAway * channels || imageXSource >(imageWidth * channels - 1 - cuttedAway * channels)))
	{
		////height
		for (int h = 0; h < filterHeight; h++)
		{
			//width
			for (int w = 0; w < filterHeight; w++)
			{
				double tmp = filter[h * filterHeight + w] * dev_data[((imageYSource + h) * (channels * imageWidth) + (imageXSource + (channels * w)))];
				dev_dataResult[((imageYSource - cuttedAway) * (channels * newImageWidth) + (imageXSource - channels * cuttedAway))] += tmp;

			}
		}
	}

	//// max = 7000000 with block- and grid-dim = 1
//for (int i = 0; i < 1; i++) {
//	dev_dataResult[0] = 1;
//}

}

unsigned char * convertRGBToYCBCR(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims)
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
	cudaDeviceReset();

	return dataResult;
}


//unsigned char* gaussianOneChannel(unsigned char * data, int dataSize, dim3 gridDims, dim3 blockDims, double* filter, int imageHeight, int imageWidth, int filterHeight)
//{
//
//	int channels = 3;
//	int newImageWidth = imageWidth - filterHeight + 1;
//	int newImageHeight = imageHeight - filterHeight + 1;
//	int dataSizeResultImage = newImageWidth * newImageHeight;
//	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSizeResultImage);
//
//	unsigned char* dev_data;
//	unsigned char* dev_dataResult;
//	double* dev_filter;
//
//	int tmp = dataSize / channels;
//
//
//	cudaMalloc(&dev_filter, sizeof(double) * filterHeight * filterHeight);
//	cudaMemcpy(dev_filter, filter, sizeof(double) * filterHeight * filterHeight, cudaMemcpyHostToDevice);
//
//
//
//	cudaMalloc(&dev_data, sizeof(unsigned char) * tmp);
//	cudaError_t error = cudaGetLastError();
//	if (error != cudaSuccess)
//	{
//		printf("CUDA error0: %s\n", cudaGetErrorString(error));
//		exit(-1);
//	}
//	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSizeResultImage);
//
//	error = cudaGetLastError();
//	if (error != cudaSuccess)
//	{
//		printf("CUDA error1: %s\n", cudaGetErrorString(error));
//		exit(-1);
//	}
//	cudaMemcpy(dev_data, data, sizeof(unsigned char) * (dataSize / channels), cudaMemcpyHostToDevice);
//	error = cudaGetLastError();
//	if (error != cudaSuccess)
//	{
//		printf("CUDA error2: %s\n", cudaGetErrorString(error));
//		exit(-1);
//	}
//
//	dev_applyGaussian << < gridDims, blockDims >> > (dev_data, dev_dataResult, dev_filter, dataSize, imageHeight, imageWidth, filterHeight);
//
//	error = cudaGetLastError();
//	if (error != cudaSuccess)
//	{
//		printf("CUDA error3: %s\n", cudaGetErrorString(error));
//		exit(-1);
//	}
//
//
//	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSizeResultImage, cudaMemcpyDeviceToHost);
//
//
//	//cudaFree(&dev_data);
//	//cudaFree(&dev_dataResult);
//	//cudaFree(&dev_filter);
//	//cudaDeviceReset();
//
//	return dataResult;
//}

unsigned char* gaussianAllChannel(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims, double* filter, int imageHeight, int imageWidth, int filterHeight)
{

	int channels = 3;
	int newImageWidth = channels*(imageWidth - filterHeight + 1);
	int newImageHeight = channels*(imageHeight - filterHeight + 1);
	int dataSizeResultImage = newImageWidth * newImageHeight;
	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSizeResultImage);

	unsigned char* dev_data;
	unsigned char* dev_dataResult;
	double* dev_filter;



	cudaMalloc(&dev_filter, sizeof(double) * filterHeight * filterHeight);
	cudaMemcpy(dev_filter, filter, sizeof(double) * filterHeight * filterHeight, cudaMemcpyHostToDevice);



	cudaMalloc(&dev_data, sizeof(unsigned char) * dataSize);
	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error0: %s\n", cudaGetErrorString(error));
		exit(-1);
	}
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSizeResultImage); //dataSizeResultImage

	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error1: %s\n", cudaGetErrorString(error));
		exit(-1);
	}
	cudaMemcpy(dev_data, data, sizeof(unsigned char) * dataSize, cudaMemcpyHostToDevice);
	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error2: %s\n", cudaGetErrorString(error));
		exit(-1);
	}

	dev_applyGaussianALL << < gridDims, blockDims >> > (dev_data, dev_dataResult, dev_filter, dataSize, imageHeight, imageWidth, filterHeight);

	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error3: %s\n", cudaGetErrorString(error));
		exit(-1);
	}


	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSizeResultImage, cudaMemcpyDeviceToHost); //

	//for (int i = 0; i < newImageHeight * newImageWidth; i++)
	//{
	//	cout << (int)dataResult[i] << " ";
	//}

	//cudaDeviceSynchronize();

	//cudaFree(&dev_data);
	//cudaFree(&dev_dataResult);
	//cudaFree(&dev_filter);
	//cudaDeviceReset();

	return dataResult;
}




double* createGaussianFilter(int width, int height, double sigma)
{
    double r, s = 2.0 * sigma * sigma; 

    

	double PI = 3.1415;

	double** kernel = (double**) malloc(height * sizeof(double *));

	for (int i = 0; i < height; i++) {
		kernel[i] = (double*) malloc(sizeof(double) * width);
	}

	double sum = 0.0;


	for (int x = -height/2; x <= height/2; x++) { 
        for (int y = -height/2; y <= height/2; y++) { 
            r = sqrt(x * x + y * y); 
			kernel[x + height/2][y + height/2] = (exp(-(r * r) / s)) / (PI * s);
            sum += kernel[x + height/2][y + height/2];
        } 
    } 

  
    // normalising the Kernel 
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < height; ++j) {
			kernel[i][j] /= sum;
        }
	} 

	double* kernelFlat = (double*)malloc(height * height * sizeof(double));

	for (int h = 0; h < height; h++){
		for (int w = 0; w < height; w++){
			kernelFlat[h * height + w] = kernel[h][w]; // y*width+width_pos
		}
	}

	return kernelFlat;
}

unsigned char* applyGaussianFilter(unsigned char* data, int dataSize, dim3 gridDims,
									dim3 blockDims, const int channelsPara, int imageHeight,
									int imageWidth, int filterHeight, double sigma)
{
	double* filter = createGaussianFilter(filterHeight, filterHeight, sigma);
	unsigned char* resultData = gaussianAllChannel(data, dataSize, gridDims, blockDims,
													filter, imageHeight, imageWidth, filterHeight);
	return resultData;
}
