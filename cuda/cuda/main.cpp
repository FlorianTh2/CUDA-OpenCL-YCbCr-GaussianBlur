#include "main.h"



// unsigned char array to mat
// mat displayen
// adjustierte mat displayen (damit man vergleichen kann mit opencv-veränderte matrix)




// deviceQuery(): gtx 750 ti
//Max dimension size of a thread block(x, y, z) : (1024, 1024, 64)
//Max dimension size of a grid size(x, y, z) : (2147483647, 65535, 65535)
//Total amount of constant memory : 65536 bytes
//Total amount of shared memory per block : 49152 bytes
//( 5) Multiprocessors, (128) CUDA Cores/MP:     640 CUDA Cores

int main(int argc, char** argv)
{
	//colorConversionYCBCR();
	gaussianFilter1();
	return 0;
}



void gaussianFilter1()
{
	Timer timer;
	timer.start();
	const int channels = 3;
	cv::Mat matBGR = readImage();
	cv::Mat matBGRSplitted[channels];
	cv::split(matBGR, matBGRSplitted);





	analyseMatInput(matBGR);
	tuple<int, int> imageSize = getMatSize(matBGR);
	// so it DOES includes the channels
	int dataSize = get<0>(imageSize) * get<1>(imageSize) * channels;
	const int sizeOfOneColorChannel = dataSize / 3;
	// its full height, not from mid or somehing like that
	int filterHeight = 5;
	double sigma = 10.0;
	int newImageWidth = get<0>(imageSize) - filterHeight + 1;
	int newImageHeight = get<1>(imageSize) - filterHeight + 1;
	int dataSizeResultImage = newImageWidth * newImageHeight;

	dim3 blockDims = dim3(1,1,1); //(1, 0, 0);
	dim3 gridDims = dim3(dataSizeResultImage,1,1);


	unsigned char** resultChannels = (unsigned char**)malloc(channels * sizeof(unsigned char*));


	for (int i = 0; i < channels; i++) {
		resultChannels[i] = (unsigned char*) malloc(sizeof(unsigned char) * sizeOfOneColorChannel);
		resultChannels[i] = returnMatDataWithCharArray(matBGRSplitted[i]);
	}


	uchar** resultdata = applyGaussianFilter(resultChannels, dataSize, gridDims, blockDims, channels, get<1>(imageSize), get<0>(imageSize), filterHeight, sigma);





	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;





	std::vector<cv::Mat> arrayChannelMatsResult;


	//// testing to merge spliced channels (which are not processed)
	//for (size_t i = 0; i < channels; i++)
	//{
	//	tuple<int, int> imageSizeResultImage = imageSize;
	//	arrayChannelMatsResult.push_back(returnMatFromCharArrayOneChannel(resultChannels[i], imageSizeResultImage));
	//}

	// merges processed channels
	for (size_t i = 0; i < channels; i++)
	{
		tuple<int, int> imageSizeResultImage = make_tuple(newImageWidth+3, newImageHeight);
		arrayChannelMatsResult.push_back(returnMatFromCharArrayOneChannel(resultdata[i], imageSizeResultImage));
	}

	cv::Mat matBGRResult;
	cv::merge(arrayChannelMatsResult, matBGRResult);
	displayImage(matBGRResult);

	//int differenceColorConversion = differenceBetweenOpenCVAndGPURendered(opencvYCBCR, matResultYCRCB);
	free(resultChannels);
	free(resultdata);
}














void colorConversionYCBCR()
{
	Timer timer;
	timer.start();
	int channels = 3;
	cv::Mat matBGR = readImage();
	cv::Mat mat = convertMatBGRToRGB(matBGR);
	analyseMatInput(mat);
	tuple<int, int> imageSize = getMatSize(mat);
	// so it DOES includes the channels
	int dataSize = get<0>(imageSize) * get<1>(imageSize) * channels;

	//dim3 gridDims((unsigned int)ceil((double)(width * height * 3 / blockDims.x)), 1, 1);
	//dim3 blockDims(512, 1, 1);
	//blur << <gridDims, blockDims >> > (dev_input, dev_output, width, height);
	//dim3 blockDims(512, 1, 1);
	dim3 blockDims(1, 1, 1);
	// ceil=next-biggest-integer
	//dim3 gridDims((unsigned int) ceil((double)(dataSize / blockDims.x)), 1, 1);
	dim3 gridDims(1, 1, 1);

	uchar* data = returnMatDataWithCharArray(mat);
	uchar* resultdata = convertRGBToYCBCR(data, dataSize, gridDims, blockDims);

	cv::Mat matResultYCRCB = returnMatFromCharArray(resultdata, imageSize);

	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;

	cv::Mat opencvYCBCR = convertBRGToYcbcr(mat);
	int differenceColorConversion = differenceBetweenOpenCVAndGPURendered(opencvYCBCR, matResultYCRCB);
	cout << "differenceColorConversion: " << differenceColorConversion << endl;



	cv::Mat toDisplay1 = convertYcbcrToBRG(matResultYCRCB);
	cv::Mat toDisplay2 = convertYcbcrToBRG(opencvYCBCR);



	displayImages(toDisplay1, toDisplay2);

}