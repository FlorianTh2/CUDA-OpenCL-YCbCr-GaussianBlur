#include "main.h"



// unsigned char array to mat
// mat displayen
// adjustierte mat displayen (damit man vergleichen kann mit opencv-veränderte matrix)



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

	dim3 blockDims(1, 1, 1);
	dim3 gridDims(1, 1, 1);

	const int sizeOfOneColorChannel = dataSize / 3;

	unsigned char** resultChannels = (unsigned char**)malloc(channels * sizeof(unsigned char*));


	for (int i = 0; i < channels; i++) {
		resultChannels[i] = (unsigned char*) malloc(sizeof(unsigned char) * sizeOfOneColorChannel);
		resultChannels[i] = returnMatDataWithCharArray(matBGRSplitted[i]);
	}

	// erstmal auskommentiert, da versucht wird, dass orig-image wieder zu mergen
	uchar** resultdata = applyGaussianFilter(resultChannels, dataSize, gridDims, blockDims, channels);


	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;



	std::vector<cv::Mat> arrayChannelMatsResult;
	for (size_t i = 0; i < channels; i++)
		arrayChannelMatsResult.push_back(returnMatFromCharArrayOneChannel(resultdata[i], imageSize));
	cv::Mat matBGRResult;
	cv::merge(arrayChannelMatsResult, matBGRResult);
	displayImage(matBGRResult);

	//int differenceColorConversion = differenceBetweenOpenCVAndGPURendered(opencvYCBCR, matResultYCRCB);
	free(resultChannels);
}

// merge color array
// darstellen des Ausgangsbildes
// gauss



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