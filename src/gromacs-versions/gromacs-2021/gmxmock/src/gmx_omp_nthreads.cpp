#include "gromacs/mdlib/gmx_omp_nthreads.h"

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
