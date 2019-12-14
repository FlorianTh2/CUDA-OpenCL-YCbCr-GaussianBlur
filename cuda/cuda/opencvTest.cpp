#include "opencvTest.h"

// using namespace std;

void readImage()
{
	// CV_LOAD_IMAGE_COLOR = loads the image in the BGR format
	cv::Mat img = imread("dice.png",CV_LOAD_IMAGE_COLOR);

	img.empty() ? printf("failed to open img.jpg\n") : printf("img.jpg loaded OK"\n);

//	if (img.empty())
//		cout << "failed to open img.jpg" << endl;
//	else
//		cout << "img.jpg loaded OK" << endl;

	// WINDOW_NORMAL = image will resize itself according to the current window size
	cv::namedWindow("image", cv::WINDOW_NORMAL);
	// update the content of the OpenCV window with a new image
	cv::imshow("image", img);
	// window to be displayed until the user presses a key
	cv::waitKey(0);


}