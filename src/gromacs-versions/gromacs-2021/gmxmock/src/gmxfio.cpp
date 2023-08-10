#include "gromacs/fileio/gmxfio.h"
#include <cstdio>
#include <cstdlib>

#include "gromacs/fileio/gmxfio_impl.h"
//typedef struct t_fileio {
//  FILE* fp;
//} t_fileio;

t_fileio *gmx_fio_open(const char *fn, const char *mode)
{
  t_fileio *fio = (t_fileio *) calloc(1, sizeof(*fio));

  fio->fp = fopen(fn, mode);

  return fio;
}

FILE *gmx_fio_getfp(t_fileio *fio) {
  return fio->fp;
}

int gmx_fio_fclose(FILE *fp) {
  fclose(fp);
  return 0;
}
