# Parallelization with OpenMP

To compile a C++ script which uses OpenMP, add the `-fopenmp` flag to the command line when compiling. So, run
```bash
$ g++ [script.cpp] -o [script.o] -fopenmp
```

# Theoretical application

By making grid-based operations parallel, we could reduce the time taken in density and pressure calculations.

We could also make parallel vector-based operations with the correct changes to the `physics.h` library, thus reducing the time taken in acceleration, velocity and position calculations.

# Initial method

By using one lock/mutex per grid cell, we can avoid multiple threads reading the same cell. In order for multithreading to become more efficient, it would also be necessary to divide the grid cells into groups, where each group of cells has a designated thread.