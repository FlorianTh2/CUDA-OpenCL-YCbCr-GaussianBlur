#include "opencvTest.h"


// using namespace std;

cv::Mat readImage()
{
	// CV_LOAD_IMAGE_COLOR = loads the image in the BGR format
	cv::Mat img = imread("dice.png",cv::IMREAD_COLOR);
	return img;
}

void displayImage(cv::Mat mat) {
	// WINDOW_NORMAL = image will resize itself according to the current window size
	cv::namedWindow("OpenCV_Test_Window", cv::WINDOW_NORMAL);
	// update the content of the OpenCV window with a new image
	cv::imshow("OpenCV_Test_Window", mat);
	// window to be displayed until the user presses a key
	cv::waitKey(0);

}

void displayImages(cv::Mat mat1, cv::Mat mat2)
{
	// WINDOW_NORMAL = image will resize itself according to the current window size
	cv::namedWindow("OpenCV_Test_Window", cv::WINDOW_NORMAL);
	// update the content of the OpenCV window with a new image
	cv::imshow("1", mat1);
	cv::imshow("2", mat2);
	// window to be displayed until the user presses a key
	cv::waitKey(0);
}

cv::Mat convertBRGToYcbcr(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_BGR2YCrCb);
	return matConverted;
}

cv::Mat convertYcbcrToBRG(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_YCrCb2BGR);
	return matConverted;
}




cv::Mat changeYcbcrStyle(cv::Mat mat)
{
	cv::Mat yCrCbChannels[3];
	cv::split(mat, yCrCbChannels);
	cv::Mat half(yCrCbChannels[0].size(), yCrCbChannels[0].type(), 127);

	//// make grey-scale
	//vector<cv::Mat> yChannels = { yCrCbChannels[0], half, half };
	//cv::Mat resultMat;
	//merge(yChannels, resultMat);

	vector<cv::Mat> CrChannels = { half, yCrCbChannels[1], half };
	cv::Mat resultMat;
	cv::merge(CrChannels, resultMat);

	return resultMat;
}




void analyseMatInput(cv::Mat mat)
{
	cout << mat.size << endl;
	cout << mat.dims << endl;
	// CV_8UC3
	cout << "type\t" << GetMatType(mat) << endl;
	cout << "depth\t" << GetMatDepth(mat) << endl;
}

cv::Mat applyGaussian(cv::Mat mat)
{
	cv::Mat resultMat;
	// kernel size has to be odd
	GaussianBlur(mat, resultMat, cv::Size(101, 101), 0, 0);
	return resultMat;
}

cv::Mat convertMatBGRToRGB(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_BGR2RGB);
	return matConverted;
}

cv::Mat convertMatRGB2BGR(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_RGB2BGR);
	return matConverted;
}



vector<uchar> returnMatDataWithVector(cv::Mat mat)
{
	vector<uchar> returnArray;
	returnArray.assign(mat.data, mat.data + mat.total());
	return returnArray;

}



uchar * returnMatDataWithCharArray(cv::Mat mat)
{
	return mat.data;
}

cv::Mat returnMatFromCharArray(uchar* data, std::tuple<int, int> size)
{
	const cv::Mat img(cv::Size(get<0>(size), get<1>(size)), CV_8UC3, data);
	bool check;
	return img;
}

tuple<int, int> getMatSize(cv::Mat mat)
{
	return make_tuple(mat.size().width, mat.size().height);
}

cv::Mat convertRBG2BGR(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_RGB2BGR);
	return matConverted;
}

cv::Mat convertYCRCB2BGR(cv::Mat mat)
{
	cv::Mat matConverted;
	cv::cvtColor(mat, matConverted, cv::COLOR_YCrCb2BGR);
	return matConverted;
}

int differenceBetweenOpenCVAndGPURendered(cv::Mat openMat, cv::Mat gpuMat)
{
	cv::Mat result;

	auto difference = openMat - gpuMat;

	cv::subtract(openMat, gpuMat, result);
	return cv::sum(result)[0] + cv::sum(result)[1] + cv::sum(result)[2];

}

void printCharArray(vector<uchar> mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		printf("%d ", mat[i]);
	}
}

uchar* readImageAndReturnCharArray()
{
	return returnMatDataWithCharArray(readImage());
}

void mainInOpencv() {
	cv::Mat mat = readImage();
	if (mat.empty())
	{
		cout << "failed to open img.jpg" << endl;
		return;
	}
	analyseMatInput(mat);
	//displayImage(mat);
	cv::Mat matYcbcR = convertBRGToYcbcr(mat);
	cv::Mat matAdjusted = changeYcbcrStyle(matYcbcR);
	cv::Mat convertedBack = convertYcbcrToBRG(matAdjusted);
	//displayImage(convertedBack);
	//displayImage(applyGaussian(mat));





	cv::Mat testMatrix(5, 5, CV_8UC3, cv::Scalar(0, 0, 255));
	vector<uchar> testMatrixInVector = returnMatDataWithVector(testMatrix);
	printCharArray(testMatrixInVector);
	displayImage(testMatrix);


}

























std::string GetMatDepth(const cv::Mat& mat)
{
	const int depth = mat.depth();

	switch (depth)
	{
	case CV_8U:  return "CV_8U";
	case CV_8S:  return "CV_8S";
	case CV_16U: return "CV_16U";
	case CV_16S: return "CV_16S";
	case CV_32S: return "CV_32S";
	case CV_32F: return "CV_32F";
	case CV_64F: return "CV_64F";
	default:
		return "Invalid depth type of matrix!";
	}
}

std::string GetMatType(const cv::Mat& mat)
{
	const int mtype = mat.type();

	switch (mtype)
	{
	case CV_8UC1:  return "CV_8UC1";
	case CV_8UC2:  return "CV_8UC2";
	case CV_8UC3:  return "CV_8UC3";
	case CV_8UC4:  return "CV_8UC4";

	case CV_8SC1:  return "CV_8SC1";
	case CV_8SC2:  return "CV_8SC2";
	case CV_8SC3:  return "CV_8SC3";
	case CV_8SC4:  return "CV_8SC4";

	case CV_16UC1: return "CV_16UC1";
	case CV_16UC2: return "CV_16UC2";
	case CV_16UC3: return "CV_16UC3";
	case CV_16UC4: return "CV_16UC4";

	case CV_16SC1: return "CV_16SC1";
	case CV_16SC2: return "CV_16SC2";
	case CV_16SC3: return "CV_16SC3";
	case CV_16SC4: return "CV_16SC4";

	case CV_32SC1: return "CV_32SC1";
	case CV_32SC2: return "CV_32SC2";
	case CV_32SC3: return "CV_32SC3";
	case CV_32SC4: return "CV_32SC4";

	case CV_32FC1: return "CV_32FC1";
	case CV_32FC2: return "CV_32FC2";
	case CV_32FC3: return "CV_32FC3";
	case CV_32FC4: return "CV_32FC4";

	case CV_64FC1: return "CV_64FC1";
	case CV_64FC2: return "CV_64FC2";
	case CV_64FC3: return "CV_64FC3";
	case CV_64FC4: return "CV_64FC4";

	default:
		return "Invalid type of matrix!";
	}
}