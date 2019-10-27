#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "opencvTest.h"


void readImage()
{
	cv::Mat img = cv::imread("dice.png");
	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::imshow("image", img);
	cv::waitKey(0);
}