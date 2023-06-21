#include "network.cpp"

#include <exception>
#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

int gmx_omp_nthreads_get(int mod)
{
#ifdef _OPENMP
  return omp_get_num_threads();
#else
  return 1;
#endif
}

void getThreadAtomRange(int numThreads, int threadIndex, int numAtoms, int* startAtom, int* endAtom)
{
    constexpr int blockSize = 1;

    int numBlocks = (numAtoms + blockSize - 1) / blockSize;

    *startAtom = ((numBlocks * threadIndex) / numThreads) * blockSize;
    *endAtom   = ((numBlocks * (threadIndex + 1)) / numThreads) * blockSize;
    if (threadIndex == numThreads - 1)
    {
        *endAtom = numAtoms;
    }
}

namespace gmx {

void processExceptionAsFatalError(const std::exception& ex)
{
  std::cerr << ex.what() << '\n';
  throw;
}

}