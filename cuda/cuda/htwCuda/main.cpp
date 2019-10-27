#include <iostream>
#include <stdlib.h>
#include "cudaTest.h"
#include "opencvTest.h"
//#include <opencv2/opencv.hpp>



int main(int argc, char** argv)
{
	std::cout << "Hi from main-function" << std::endl;
	doSmth();
	readImage();

	return 0;
}