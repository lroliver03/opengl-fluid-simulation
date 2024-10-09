#include <iostream>
#include <omp.h>

int main() {

  int nthreads, tid, procs, maxt, inpar, dynamic, nested;

  #pragma omp parallel private (nthreads, tid) 
  {
    tid = omp_get_thread_num();

    if (tid == 0) {
      std::cout << "Getting thread info...\n";
      
      procs = omp_get_num_procs();
      nthreads = omp_get_num_threads();
      maxt = omp_get_max_threads();
      inpar = omp_in_parallel();
      dynamic = omp_get_dynamic();
      nested = omp_get_nested();

      std::cout << "Number of processors = " << procs << std::endl;
      std::cout << "Number of threads = " << nthreads << std::endl;
      std::cout << "Max threads = " << maxt << std::endl;
      std::cout << "In parallel? = " << inpar << std::endl;
      std::cout << "Is dynamic? = " << dynamic << std::endl;
      std::cout << "Is nested? = " << nested << std::endl;
    }
  }

  return 0;
}