#include "opencvTest.h"

void readImage()
{
	cv::Mat img = cv::imread("dice.png");

	if (img.empty())
		std::cout << "failed to open img.jpg" << std::endl;
	else
		std::cout << "img.jpg loaded OK" << std::endl;

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::imshow("image", img);
	cv::waitKey(0);


}