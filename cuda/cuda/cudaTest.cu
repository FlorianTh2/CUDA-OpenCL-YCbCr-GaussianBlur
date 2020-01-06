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
	// blockid * wieVieleBl�ckeGesamt + (threadID*channels)
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


__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight)
{

	int imageYSource = blockIdx.x / imageWidth;
	int imageXSource = blockIdx.x % imageWidth;

	int cuttedAway = (filterHeight/2)

	if (imageYSource <  cuttedAway && imageYSource > (imageHeight - cuttedAway -1) && imageXSource <  cuttedAway && imageXSource > (imageWidth - cuttedAway -1))
		return;


	int newImageHeight = imageHeight - filterHeight + 1;
	int newImageWidth = imageWidth - filterHeight + 1;

	int imageYResult = newImageWidth / newImageWidth;
	int imageXResult = newImageWidth % newImageWidth;





		//for (int h = 0; h < filterHeight; h++)
		//{
		//	for (int w = 0; w < filterHeight; w++)
		//	{
		//		double tmp = filter[h * filterHeight + w] * dev_data[(imageYResult + h - filterHeight/2) * newImageWidth + (imageXResult + w - filterHeight / 2)];
		//
		//		dev_dataResult[imageYResult * newImageWidth + imageXResult] += filter[4 * filterHeight + 4] * tmp* 100; // (dev_dataResult[imageYResult * newImageWidth + imageXResult] + tmp * 1000000);
		//	}
		//}

		for (int h = 0; h < filterHeight; h++)
		{
			for (int w = 0; w < filterHeight; w++)
			{
				double tmp = filter[h * filterHeight + w] * dev_data[(imageYSource + h) * imageWidth + (imageXSource + w)];

				dev_dataResult[(imageYSource-cuttedAway) * newImageWidth + (imageXSource- cuttedAway)] += tmp * 100;
			}
		}


		//dev_dataResult[0] =  100 * filter[0];
		//dev_dataResult[1] =  dev_data[17051];
		//dev_dataResult[2] = filter[4 * filterHeight + 4] * 100000;


		

	//// max = 7000000 with block- and grid-dim = 1
	//for (int i = 0; i < 1; i++) {
	//	dev_dataResult[0] = 1;
	//}


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



unsigned char* gaussianOneChannel(unsigned char * data, int dataSize, dim3 gridDims, dim3 blockDims, double* filter, int imageHeight, int imageWidth, int filterHeight)
{


	int newImageWidth = imageWidth - filterHeight + 1;
	int newImageHeight = imageHeight - filterHeight + 1;
	int dataSizeResultImage = newImageWidth * newImageHeight;
	unsigned char* dataResult = (unsigned char*)malloc(sizeof(unsigned char) * dataSizeResultImage);

	unsigned char* dev_data;
	unsigned char* dev_dataResult;
	double* dev_filter;

	int tmp = dataSize / 3;


	cudaMalloc(&dev_filter, sizeof(double) * filterHeight * filterHeight);
	cudaMemcpy(dev_filter, filter, sizeof(double) * filterHeight * filterHeight, cudaMemcpyHostToDevice);



	cudaMalloc(&dev_data, sizeof(unsigned char) * tmp);
	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error0: %s\n", cudaGetErrorString(error));
		exit(-1);
	}
	cudaMalloc(&dev_dataResult, sizeof(unsigned char) * dataSizeResultImage);

	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error1: %s\n", cudaGetErrorString(error));
		exit(-1);
	}
	cudaMemcpy(dev_data, data, sizeof(unsigned char) * (dataSize / 3), cudaMemcpyHostToDevice);
	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error2: %s\n", cudaGetErrorString(error));
		exit(-1);
	}

	dev_applyGaussian << < gridDims, blockDims >> > (dev_data, dev_dataResult, dev_filter, dataSize, imageHeight, imageWidth, filterHeight);

	error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		printf("CUDA error3: %s\n", cudaGetErrorString(error));
		exit(-1);
	}


	cudaMemcpy(dataResult, dev_dataResult, sizeof(unsigned char) * dataSizeResultImage, cudaMemcpyDeviceToHost);




	for (size_t i = 0; i < dataSizeResultImage; i++)
	{
		//cout << (int) dataResult[i] << " ";



		//if (data[i] != 0 && data[i] != 255) {
		//	cout << "This index of element not equals 0 and not 255: " << i << endl;
		//}
		//cout << (int) data[i] << " ";
	}

	cudaFree(&dev_data);
	cudaFree(&dev_dataResult);
	cudaFree(&dev_filter);
	cudaDeviceReset();

	return dataResult;
}

double* createGaussianFilter(int width, int height, double sigma)
{
        double r, s = 2.0 * sigma * sigma; 

    

	double PI = 3.1415;

	double** GKernel = (double**) malloc(height * sizeof(double *));

	for (int i = 0; i < height; i++) {
		GKernel[i] = (double*) malloc(sizeof(double) * width);
	}

	double sum = 0.0;


for (int x = -height/2; x <= height/2; x++) { 
        for (int y = -height/2; y <= height/2; y++) { 
            r = sqrt(x * x + y * y); 
            GKernel[x + height/2][y + height/2] = (exp(-(r * r) / s)) / (M_PI * s); 
            sum += GKernel[x + height/2][y + height/2]; 
        } 
    } 
  
    // normalising the Kernel 
    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < height; ++j)
        {
            GKernel[i][j] /= sum; 
        }
	} 


    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < height; ++j)
        {
            cout << GKernel[i][j] << " ";
        }
        cout << endl;
	} 






	cout << "sum: " << sum << endl;


	double* kernelFlat = (double*)malloc(height * height * sizeof(double));

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < height; w++)
		{
			// y*width+width_pos
			kernelFlat[h * height + w] = GKernel[h][w];
		}
	}


	return kernelFlat;
}

// data: BGR-Sequence of the input channels of data
unsigned char** applyGaussianFilter(unsigned char** data, int dataSize, dim3 gridDims, dim3 blockDims, const int channelsPara, int imageHeight, int imageWidth, int filterHeight, double sigma)
{


	cout << dataSize << endl;
	const int channels = channelsPara;

	const int sizeOfOneColorChannel = dataSize / 3;

	unsigned char** resultChannels1 = (unsigned char**)malloc(channels * sizeof(unsigned char*));

	double* filter = createGaussianFilter(filterHeight, filterHeight, sigma);

	int newImageWidth = imageWidth - filterHeight + 1;
	int newImageHeight = imageHeight - filterHeight + 1;
	int dataSizeResultImage = newImageWidth * newImageHeight;

	for (int i = 0; i < channels; i++) {
		resultChannels1[i] = (unsigned char*) malloc(sizeof(unsigned char) * dataSizeResultImage);
		resultChannels1[i] = gaussianOneChannel(data[i], dataSize, gridDims, blockDims, filter, imageHeight, imageWidth, filterHeight);
	}
	//resultChannels1[0] = gaussianOneChannel(data[0], dataSize, gridDims, blockDims, filter, imageHeight, imageWidth, filterHeight);


	for (size_t i = 0; i < dataSize/3; i++)
	{
		//cout << (int) data[0] << " ";
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




	//width
	//for (int i = 0; i < newImageWidth; i++) {
	//	//height
	//	for (int j = 0; j < newImageHeight; j++) {
	//		//filterWidth
	//		//used "filterHeight" for height and width because its assumed that filter is symmetric
	//		for (int h = i; h < (i + filterHeight); h++) {
	//			//filterHeight
	//			for (int w = j; w < (j + filterHeight); w++) {
	//				//dev_dataResult[i* imageWidth + j] = (unsigned char) (dev_dataResult[i * imageWidth + j] + filter[h - i][w - j] * dev_data[h * imageWidth + w]);
	//				//dev_dataResult[0] = 1;
	//			}
	//		}
	//	}
	//}

