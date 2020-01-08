#pragma once


#include <iostream>
#include <stdlib.h>
#include "cudaTest.cuh"
#include "opencvTest.h"
#include "timer.h"

void colorConversionYCBCR(string image_name);
void gaussianFilter1(string image_name, double sigma, int filter_height);
int main(int argc, char** argv);
void programStartArgumentHandling(int argc, char** argv);