#pragma once


#include <iostream>
#include <stdlib.h>
#include "cudaTest.cuh"
#include "opencvTest.h"
#include "timer.h"

cv::Mat colorConversionYCBCR(string image_name);
cv::Mat gaussianFilter1(string image_name, double sigmaPara, int filter_height);
int main(int argc, char** argv);
void programStartArgumentHandling(int argc, char** argv);
void createFolderIfNotExistent(string pathPara);
vector<string> returnImageNamesInPath(string path);
void doBenchmark(string image_name, double sigmaPara, int filter_height);
string getImageNameFromPath(string path);