#include "gromacs/mdlib/update.h"

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

