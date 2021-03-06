#pragma once
#include <iostream>
#include <stdlib.h>
#include "cuda.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

__global__ void test();
__global__ void dev_convertColorSpace(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize);
__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight);
__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight);
__global__ void dev_applyGaussianALL(unsigned char* dev_data, unsigned char* dev_dataResult, double* filter, int dataSize, int imageHeight, int imageWidth, int filterHeight);


void doSmth();
unsigned char* convertRGBToYCBCR(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims);
unsigned char* applyGaussianFilter(unsigned char* data, const int dataSize, dim3 gridDims, dim3 blockDims, const int channelsPara, int imageHeight, int imageWidth, int filterHeight, double sigma);
void cudaMain(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims);
unsigned char* gaussianOneChannel(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims, double* filter, int imageHeight, int imageWidth, int filterHeight);
double* createGaussianFilter(int width, int height, double sigma);
unsigned char* gaussianAllChannel(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims, double* filter, int imageHeight, int imageWidth, int filterHeight);