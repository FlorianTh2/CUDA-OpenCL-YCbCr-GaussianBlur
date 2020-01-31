#pragma once


#include <iostream>
#include <stdlib.h>
#include "cuda.cuh"
#include "opencv.h"
#include "timer.h"
#include <boost/program_options.hpp>
// zum Einbinden: 1. zip herunterladen 2. entpacken 3. c/c++ compiler als additional library hinzufügen (root pfad des ordners) 4. in Ordner die .bar aufführen+ warten + .b2 ausführen+waren 5. dem linker als additional library hinzufügen (root/stage/lib-pfad)
#include <boost/filesystem.hpp>
#include <iterator>
#include <algorithm>
#include <math.h>

cv::Mat colorConversionYCBCR(string image_name);
cv::Mat gaussianFilter1(string image_name, double sigmaPara, int filter_height);
int main(int argc, char** argv);
void programStartArgumentHandling(int argc, char** argv);
void createFolderIfNotExistent(string pathPara);
vector<string> returnImageNamesInPath(string path);
void allYCBCR(string image_name);
string getImageNameFromPath(string path);
void allGaussian(string image_name, double sigmaPara, int filter_height);