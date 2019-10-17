#include <iostream>
#include <omp.h>

int main(int argc, char* argv[]){

    #pragma omp parallel
    {
        printf("Thread %d of %d\n", omp_get_thread_num()+1,omp_get_num_threads());
    }
    
    return 0;
}