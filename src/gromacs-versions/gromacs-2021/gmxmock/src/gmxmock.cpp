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

typedef struct t_fileio {
  FILE* fp;
} t_fileio;

t_fileio *gmx_fio_open(const char *fn, const char *mode)
{
  t_fileio *fio = (t_fileio *) calloc(1, sizeof(*fio));

  fio->fp = fopen(fn, mode);

  return fio;
}

FILE *gmx_fio_getfp(t_fileio *fio) {
  return fio->fp;
}

void gmx_fio_fclose(FILE *fp) {
  fclose(fp);
}

namespace gmx {

void processExceptionAsFatalError(const std::exception& ex)
{
  std::cerr << ex.what() << '\n';
  throw;
}

}