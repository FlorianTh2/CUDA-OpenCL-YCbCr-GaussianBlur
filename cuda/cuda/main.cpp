#include "main.h"
// zum Einbinden: 1. zip herunterladen 2. entpacken 3. c/c++ compiler als additional library hinzufügen (root pfad des ordners) 4. in Ordner die .bar aufführen+ warten + .b2 ausführen+waren 5. dem linker als additional library hinzufügen (root/stage/lib-pfad)
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <math.h>



// unsigned char array to mat
// mat displayen
// adjustierte mat displayen (damit man vergleichen kann mit opencv-ver�nderte matrix)


namespace po = boost::program_options;


// deviceQuery(): gtx 750 ti
//Max dimension size of a thread block(x, y, z) : (1024, 1024, 64) -> (1024,1024,61) means that max threads are 1024 and not 1024*1024*64, that are max dimension limits i guess
//Max dimension size of a grid size(x, y, z) : (2147483647, 65535, 65535)
//Total amount of constant memory : 65536 bytes
//Total amount of shared memory per block : 49152 bytes
//( 5) Multiprocessors, (128) CUDA Cores/MP:     640 CUDA Cores

int main(int argc, char** argv)
{


	//allYCBCR("images/", 3.0, 5); // präsentation: 	doBenchmark("images/", 3.0, 5);
	//allGaussian("images/", 3.0, 5); // präsentation: 	doBenchmark("images/", 3.0, 5);


	programStartArgumentHandling(argc, argv);
	//cv::Mat mat = colorConversionYCBCR("dice.png");
	//cv::Mat mat2 = convertBRGToYcbcr(readImageWithName("dice.png"));
	//cv::Mat mat2 = gaussianFilter1("universum_8192x8192.jpg", 5.0, 1); // Periodic_table_large_6000x3300.png // lufthansa-tk-big_8192x4608.png // dice_large_1754x1554


	//displayImage(mat2);
	return 0;
}



void programStartArgumentHandling(int argc, char** argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		// required
		("task", po::value<int>(), "set task: 0=rgb2YCbCr; 1=gaussian_blur; 2=all_ycbcr_all_images; 3=gaussian_all_images")
		("image_name", po::value<string>(), "set name / path of image to read")

		// only for gaussian blur
		("sigma", po::value<double>(), "set sigma to use for filter")
		("filter_height", po::value<int>(), "set height/width of filter to use (has to be odd)")
		
		//help
		("help", "produce help message");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("task") && vm.count("image_name")) {
		//if (readImageWithName(vm["image_name"].as<string>()).empty())
		//{
		//	cout << "Image could not be found, stopping." << endl;
		//	return;
		//}
		if (vm["task"].as<int>() != 0 && vm["task"].as<int>() != 1 && vm["task"].as<int>() != 2 && vm["task"].as<int>() != 3)
		{
			cout << "Task was not 0 or 1 or 2" << endl;
			return;
		}

		if (vm["task"].as<int>() == 0)
		{
			colorConversionYCBCR(vm["image_name"].as<string>());
		}
		else if(vm.count("sigma") && vm["sigma"].as<double>() > 0 && vm.count("filter_height") && (vm["filter_height"].as<int>() % 2) !=0)
		{
			if(vm["task"].as<int>() == 1)
			{
				cout << "Task 1 started!" << endl;
			displayImage(gaussianFilter1(vm["image_name"].as<string>(), vm["sigma"].as<double>(), vm["filter_height"].as<int>()));
			}
			else if (vm["task"].as<int>() == 3)
			{
				cout << "Task 3 started!" << endl;
				allGaussian(vm["image_name"].as<string>(), vm["sigma"].as<double>(), vm["filter_height"].as<int>());
			}
			else
			{
				cout << "Task 2 started!" << endl;
				allYCBCR(vm["image_name"].as<string>(), vm["sigma"].as<double>(), vm["filter_height"].as<int>());
			}
		}
		else
		{
			cout << "Parameter sigma or filter_height were somehow wrong or missing, stopping" << endl;
		}
	}
	else
	{
		cout << "Error at argument parsing" << endl;
	}

	if (vm.count("help")) {
		cout << desc << "\n";
		return;
	}

}


//return mat rgb
cv::Mat gaussianFilter1(string image_name, double sigmaPara, int filter_height)
{
	// alpha channel will be ignored in this project since it is about gpu-computing
	const int channels = 3;
	cv::Mat matBGR;
	if (readImageWithName(image_name).channels() == 4)
		matBGR = bgra2bgr(readImageWithName(image_name));
	else
		matBGR = readImageWithName(image_name);

	tuple<int, int> imageSize = getMatSize(matBGR);
	// so it DOES includes the channels
	int dataSize = get<0>(imageSize) * get<1>(imageSize) * channels;
	const int sizeOfOneColorChannel = dataSize / channels;
	int filterHeight = filter_height;
	double sigma = sigmaPara;
	int newImageWidth = get<0>(imageSize) - filterHeight + 1;
	int newImageHeight = get<1>(imageSize) - filterHeight + 1;
	tuple<int, int> imageSizeResultImage = make_tuple(newImageWidth, newImageHeight);
	int maxThreadsInBlockX = sqrt(1024);
	int maxThreadsInBlockY = sqrt(1024);

	// example: 1800x1600
	// blockDims = (1024,1024,1)
	// gridDims = (~1700/1024, ~1500/1024,1) = (2,2,1)
	// all together (remember we are processing each channel with new kernel call -> so not * 3 (3 == channels)):
	// we need: 1800x1600
	// we have indexes: 1024*1024*2*2
	dim3 blockDims = dim3(maxThreadsInBlockX, maxThreadsInBlockY,1);

	dim3 gridDims = dim3(ceil((newImageHeight*channels) / (float) maxThreadsInBlockY), ceil((newImageWidth*channels) / (float) maxThreadsInBlockX),1);

	uchar* tmp1 = returnMatDataWithCharArray(matBGR);

	uchar* resultdata = applyGaussianFilter(tmp1, dataSize, gridDims, blockDims, channels, get<1>(imageSize), get<0>(imageSize), filterHeight, sigma);

	cv::Mat resultMat = returnMatFromCharArray(resultdata, imageSizeResultImage);

	return resultMat;
}


// return mat ycbcr
cv::Mat colorConversionYCBCR(string image_name)
{

	int channels = 3;
	cv::Mat matBGR = readImageWithName(image_name);
	if (matBGR.channels() == 4)
	{
		matBGR = bgra2bgr(matBGR);
	}
	cv::Mat mat = convertMatBGRToRGB(matBGR);
	tuple<int, int> imageSize = getMatSize(mat);

	// so it DOES includes the channels
	int dataSize = get<0>(imageSize) * get<1>(imageSize) * channels;


	int maxThreadsInBlockX = 1024;
	dim3 blockDims(maxThreadsInBlockX, 1, 1);
	dim3 gridDims(ceil((dataSize / 3) / maxThreadsInBlockX), 1, 1);

	uchar* data = returnMatDataWithCharArray(mat);
	uchar* resultdata = convertRGBToYCBCR(data, dataSize, gridDims, blockDims);

	cv::Mat matResultYCRCB = returnMatFromCharArray(resultdata, imageSize);

	return matResultYCRCB;

}


void allYCBCR(string image_name, double sigmaPara, int filter_height)
{
	string outputFolder = "outputImages/";
	createFolderIfNotExistent(outputFolder);

	vector<string> imageNames = returnImageNamesInPath(image_name);
	for (string imageInputPath : imageNames)
	{


		string imageName = getImageNameFromPath(imageInputPath);

		Timer timer;
		timer.start();
		cv::Mat cudaProcessed = colorConversionYCBCR(imageInputPath);
		timer.stop();
		cout << "Needed seconds for cuda ColorConversion with image: " << imageName << " = " << timer.elapsedMilliseconds() << endl;

		timer.start();
		cv::Mat opencvYCBCR = convertBRGToYcbcr(readImageWithName(imageInputPath));
		timer.stop();
		cout << "Needed seconds for opencv ColorConversion with image: " << imageName << " = " << timer.elapsedMilliseconds() << endl;



		int differenceColorConversion = differenceBetweenOpenCVAndGPURendered(opencvYCBCR, cudaProcessed);
		cout << "differenceColorConversion: " << differenceColorConversion << endl;
		saveImage(outputFolder+"YCBCR_"+ imageName, cudaProcessed);
		saveImage(outputFolder + "_YCBCR_DIFFERENCE" + imageName, cudaProcessed - opencvYCBCR);


	}

}


void allGaussian(string image_name, double sigmaPara, int filter_height)
{
	string outputFolder = "outputImages/";
	createFolderIfNotExistent(outputFolder);

	vector<string> imageNames = returnImageNamesInPath(image_name);
	cout << "Inputpath: " << image_name << endl;

	cout << "Outputpath: " << outputFolder << endl;
	for (string imageInputPath : imageNames)
	{

		string imageName = getImageNameFromPath(imageInputPath);
		cout << "im name:" << imageName << endl;

		Timer timer;
		timer.start();
		cv::Mat cudaProcessed2 = gaussianFilter1(imageInputPath, sigmaPara, filter_height);
		timer.stop();
		cout << "Needed seconds for cuda gaussian with image: " << imageName << " = " << timer.elapsedMilliseconds() << endl;


		timer.start();
		cv::Mat opencvGaussian = convertMatBGRToRGB(applyGaussian(readImageWithName(imageInputPath), filter_height, sigmaPara));
		timer.stop();
		cout << "Needed seconds for opencv ColorConversion with image: " << imageName << " = " << timer.elapsedMilliseconds() << endl;

		saveImage(outputFolder + "Gaussian_" + imageName, cudaProcessed2);
		saveImage(outputFolder + "Gaussian_OPENCV_" + imageName, opencvGaussian);

	}
}


void createFolderIfNotExistent(string pathPara)
{
	const char* path = pathPara.c_str();
	boost::filesystem::path dir(path);
	if (boost::filesystem::create_directory(dir))
	{
		std::cerr << "Directory Created: " << pathPara << std::endl;
	}
}


vector<string> returnImageNamesInPath(string path)
{
	vector<string> imageNames;
	if (boost::filesystem::is_directory(path)) {
		std::cout << path << " is a directory containing:\n";
		for (boost::filesystem::directory_entry& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {}))
		{
			imageNames.push_back(entry.path().string());
		}

	}
	return imageNames;
}


string getImageNameFromPath(string path)
{
	std::string s = path;
	std::string delimiter = "\\";
	std::string token = s.substr(s.find(delimiter));
	return token.substr(1, token.size() - 1);
}