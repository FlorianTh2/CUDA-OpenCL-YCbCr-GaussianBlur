#pragma once
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;


cv::Mat readImage();
void displayImage(cv::Mat mat);
void mainInOpencv();
cv::Mat convertBRGToYcbcr(cv::Mat mat);
cv::Mat convertYcbcrToBRG(cv::Mat mat);
cv::Mat changeYcbcrStyle(cv::Mat mat);
std::string GetMatDepth(const cv::Mat& mat);
std::string GetMatType(const cv::Mat& mat);
void analyseMatInput(cv::Mat mat);
void printCharArray(vector<uchar> mat);
vector<uchar> returnMatDataWithVector(cv::Mat mat);
uchar* returnMatDataWithCharArray(cv::Mat mat);
uchar* readImageAndReturnCharArray();
cv::Mat convertMatBGRToRGB(cv::Mat);
cv::Mat returnMatFromCharArray(uchar* data, std::tuple<int, int> size);
cv::Mat convertRBG2BGR(cv::Mat mat);
tuple<int, int> getMatSize(cv::Mat mat);
int differenceBetweenOpenCVAndGPURendered(cv::Mat openMat, cv::Mat gpuMat);
cv::Mat convertYCRCB2BGR(cv::Mat mat);
void displayImages(cv::Mat mat1, cv::Mat mat2);
cv::Mat convertMatRGB2BGR(cv::Mat mat);
cv::Mat returnMatFromCharArrayOneChannel(uchar* data, std::tuple<int, int> size);
cv::Mat readImageWithName(string im_name);
cv::Mat bgra2bgr(cv::Mat image);