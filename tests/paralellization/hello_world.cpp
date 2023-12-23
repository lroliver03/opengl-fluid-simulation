#include <iostream>
#include <omp.h>
#include <unistd.h>

using namespace std;

#define THREAD_NUM 16

int main() {

    omp_set_num_threads(THREAD_NUM);

    cout << "Hello, World!" << endl;
    cout << "Available threads: " << omp_get_num_threads() << endl;
    #pragma omp parallel
    {
        usleep(1000 * omp_get_thread_num());
        cout << "Available threads: " << omp_get_num_threads() << "\tThread ID: " << omp_get_thread_num() << endl; 
    }
    return 0;
}