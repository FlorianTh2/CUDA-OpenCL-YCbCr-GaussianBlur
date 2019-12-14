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
}

cv::Mat applyGaussian(cv::Mat mat)
{
	cv::Mat resultMat;
	GaussianBlur(mat, resultMat, cv::Size(101, 101), 0, 0);
	return resultMat;
}

void mainInOpencv() {
	cv::Mat mat = readImage();
	if (mat.empty())
	{
		cout << "failed to open img.jpg" << endl;
		return;
	}
	analyseMatInput(mat);
	displayImage(mat);
	cv::Mat matYcbcR = convertBRGToYcbcr(mat);
	cv::Mat matAdjusted = changeYcbcrStyle(matYcbcR);
	cv::Mat convertedBack = convertYcbcrToBRG(matAdjusted);
	displayImage(convertedBack);
	displayImage(applyGaussian(mat));

}