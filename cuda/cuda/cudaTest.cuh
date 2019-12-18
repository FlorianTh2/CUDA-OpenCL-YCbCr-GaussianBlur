#pragma once
#include <iostream>
#include <stdlib.h>
#include "cudaTest.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

__global__ void test();
__global__ void dev_convertColorSpace(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize);
__global__ void dev_applyGaussian(unsigned char* dev_data, unsigned char* dev_dataResult, int dataSize);




void doSmth();
unsigned char* convertRGBToYCBCR(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims);
unsigned char** applyGaussianFilter(unsigned char** data, const int dataSize, dim3 gridDims, dim3 blockDims, const int channelsPara);
void cudaMain(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims);
unsigned char* gaussianOneChannel(unsigned char* data, int dataSize, dim3 gridDims, dim3 blockDims);