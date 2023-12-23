#include <iostream>
#include <random>

using namespace std;

#define LOOPS 1000000

#define ELEMENTS 1000

float elements[ELEMENTS] = {0.f};

int main() {

    for (int iter = 0; iter < LOOPS; iter++) {
        #pragma omp parallel for
        for (int i = 0; i < ELEMENTS; i++) {
            float value = elements[i];
            value = 1e-12 * value * value;
            elements[i] = value;
        }
    }

    return 0;
}