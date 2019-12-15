#include "main.h"



int main(int argc, char** argv)
{
	Timer timer;
	timer.start();

	std::cout << "Hi from main-function" << std::endl;
	doSmth();

	timer.stop();
	cout << "Since start " << timer.elapsedMilliseconds() << "ms passed" << endl;



	mainInOpencv();



	return 0;
}