#include "main.h"



// unsigned char array to mat
// mat displayen
// adjustierte mat displayen (damit man vergleichen kann mit opencv-veränderte matrix)



int main(int argc, char** argv)
{
	Timer timer;
	timer.start();

	cv::Mat mat = readImage();
	std::vector<uchar> vector = returnMatDataWithVector(mat);
	int dataSize = vector.size();

	//dim3 gridDims((unsigned int)ceil((double)(width * height * 3 / blockDims.x)), 1, 1);
	//dim3 blockDims(512, 1, 1);
	//blur << <gridDims, blockDims >> > (dev_input, dev_output, width, height);
	dim3 blockDims(512, 1, 1);
	// ceil=next-biggest-integer
	dim3 gridDims((unsigned int) ceil((double)(dataSize / blockDims.x)), 1, 1);

	uchar* data = returnMatDataWithCharArray(mat);
	cudaMain(data, dataSize, gridDims, blockDims);
	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;






	return 0;
}