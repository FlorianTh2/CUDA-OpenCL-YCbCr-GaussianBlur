#include "main.h"



// unsigned char array to mat
// mat displayen
// adjustierte mat displayen (damit man vergleichen kann mit opencv-ver�nderte matrix)




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

void programStartArgumentHandling()
{
	// name of programm; hier ./main
	// not used anymore at the moment
	string programName= argv[0];

	// parameter to determine if user wants to get colorConversion-functionality or gaussian-blur
	// colorConversion=0 -> user wants colorConversion; colorConversion=1 -> user wants gaussian-blur (and not colorConversion)
	int colorConversion = argv[1]==vorhanden ? argv[1] : -1; --------------------------------------------------------------------- //.toInteger // get von *char einfach so zu string?


	// name of image
	string imageName = argv[2]==vorhanden ? argv[2] : "error";

	// value of sigma for 
	double sigma = argv[3]==vorhanden ? argv[1] : -1.0;--------------------------------------------------------------------- //.toInteger 

	// since filter is assumed to be symmetric only height (or width) is required
	// has to be odd, e.g. 3,5,7,9,11,...
	int filterHeight = argv[4]==vorhanden ? argv[1] : -1; --------------------------------------------------------------------- //.toInteger 

	bool imageExistence = (imageName != "error" && opencv.doesExist) ? true : false;

	if(colorConversion != 0 && colorConversion != 1 && imageName != "error")
	{
		if(imageExistence)
		{
		if(colorConversion == 0)
		{
			colorConversionYCBCR();
		}
		else
		{
			if(filterHeight != -1 && sigma != -1.0 && filterHeight > 1 && filterHeight%2!=0 && sigma > 0)
			{
				gaussianFilter1();
			}
			else
			{
				cout << "The arguments filterHeight and/or filterHeight are not correct or missng. Checked by condition: if(filterHeight != -1 && sigma != -1.0 && filterHeight > 1 && filterHeight%2!=0 && sigma > 0)" << endl;
				return;
			}
		}
		}
		else
		{
			cout << "Imagename does not exist" << endl;
			return
		}
	}
	else
	{
		cout << "The arguments colorConversion (1) and/or imageName (2) [at least] are incorrect or missing. Please enter correct arguments" << endl;
		return;
	}
	

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
	int filterHeight = 7;
	double sigma = 10.0;
	int newImageWidth = get<0>(imageSize) - filterHeight + 1;
	int newImageHeight = get<1>(imageSize) - filterHeight + 1;
	int dataSizeResultImage = newImageWidth * newImageHeight;

	dim3 blockDims = dim3(1,1,1); //(1, 0, 0);
	dim3 gridDims = dim3(dataSizeResultImage,1,1);


	unsigned char** resultChannels = (unsigned char**)malloc(channels * sizeof(unsigned char*));


	for (int i = 0; i < channels; i++) {
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
		tuple<int, int> imageSizeResultImage = make_tuple(newImageWidth, newImageHeight);
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