#include <iostream>
#include <fstream>
#include <random>

#define FILENAME "positions.txt"
#define PARTICLES 400

#define MINPOS -100.f
#define MAXPOS 100.f

int main() {

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution dist(MINPOS, MAXPOS);

    std::ofstream file;
    file.open(FILENAME, std::ios_base::out);

    for (int i = 0; i < PARTICLES; i++) {
        file << dist(rng) << " " << dist(rng) << " " << dist(rng) << "\n";
    }

    return 0;
}