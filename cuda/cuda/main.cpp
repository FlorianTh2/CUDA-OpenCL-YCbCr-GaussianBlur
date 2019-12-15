#include "main.h"


int main(int argc, char** argv)
{
	std::cout << "start main-function" << std::endl;


	uchar* data = readImageAndReturnCharArray();


	Timer timer;
	timer.start();
	cudaMain(data);
	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;






	return 0;
}