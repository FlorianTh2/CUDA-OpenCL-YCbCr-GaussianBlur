#pragma once


#include <iostream>
#include <stdlib.h>
#include "cudaTest.cuh"
#include "opencvTest.h"
#include "timer.h"

void colorConversionYCBCR();
void gaussianFilter1();
int main(int argc, char** argv);