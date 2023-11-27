#include <iostream>
#include <random>
#include <omp.h>

using namespace std;

#define LOOPS 1000000

#define ELEMENTS 1000
#define NUM_THREADS 8 // Got best results with 8 threads.

float elements[ELEMENTS] = {0.f};

int main() {

    omp_set_num_threads(NUM_THREADS);

    for (int iter = 0; iter < LOOPS; iter++) {
        #pragma omp parallel for
        for (int i = omp_get_thread_num(); i < ELEMENTS; i += NUM_THREADS) {
            float value = elements[i];
            value = 1e-12 * value * value;
            elements[i] = value;
        }
    }

    return 0;
}