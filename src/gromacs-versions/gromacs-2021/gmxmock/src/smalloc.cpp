#include "gromacs/utility/smalloc.h"
#include <cstdlib>

void* save_calloc(const char* name, const char* file, int line, size_t nelem, size_t elsize)
{
  return calloc(nelem, elsize);
}


void save_free(const char gmx_unused* name, const char gmx_unused* file, int gmx_unused line, void* ptr)
{
  if (ptr != nullptr)
  {
    free(ptr);
  }
}
