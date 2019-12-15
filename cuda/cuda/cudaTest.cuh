#pragma once
#include <iostream>
#include <stdlib.h>
#include "cudaTest.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

__global__ void test();


void doSmth();
void convertBGRToYCBCR(unsigned char* data);
void cudaMain(unsigned char* data);
