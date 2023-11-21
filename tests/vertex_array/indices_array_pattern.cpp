#include <iostream>

const int steps = 4;
const int particles = 10;

unsigned int indices[3 * (steps - 2) * particles] = {0U};

// Function to initialize vertex indices based on number of particles and number of vertices per particle circle (steps).
void initIndices(const unsigned int &index) {
    std::cout << "INDICES OF INDEX " << index << std::endl;
    for (int i = 0; i < steps - 2; i++) {
        std::cout << "\tContents:\t";

        indices[index*(steps - 2) + 3*i] = steps*index;
        indices[index*(steps - 2) + 3*i + 1] = steps*index + i + 1;
        indices[index*(steps - 2) + 3*i + 2] = steps*index + i + 2;

        std::cout << "i = " << i << "\t\t" << indices[index*(steps - 2) + 3*i] << "\t" << indices[index*(steps - 2) + 3*i + 1] << "\t" << indices[index*(steps - 2) + 3*i + 2] << std::endl;
    }
}

int main() {

    for (int i = 0; i < particles; i++) {
        initIndices(i);
    }

    return 0;
}