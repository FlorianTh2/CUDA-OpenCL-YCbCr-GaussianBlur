//#include <iostream>
//#include <stdlib.h>
//#include <omp.h>
//
//
//void testParallel()
//{
//	int threadCount = 4;
//	#pragma omp parallel num_threads(threadCount)
//	{
//		int myRank = omp_get_thread_num();
//		int threadCount1 = omp_get_num_threads();
//		printf("Hi, i am Thread %d out of %d\n", myRank + 1, threadCount1);
//	}
//}
