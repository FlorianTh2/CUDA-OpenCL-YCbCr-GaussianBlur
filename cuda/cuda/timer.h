#pragma once

#include <iostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

using namespace std;

// implementation sometimes oriented at https://gist.github.com/mcleary/b0bf4fa88830ff7c882d
class Timer
{
	private:
		chrono::time_point<chrono::system_clock> m_StartTime;
		chrono::time_point<chrono::system_clock> m_EndTime;
		bool m_bRunning = false;

	public:
		void start();
		void stop();
		double elapsedMilliseconds();
		double elapsedSeconds();
};