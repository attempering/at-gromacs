#include "types/commrec.h"
#include <unistd.h>

void gmx_bcast(int nbytes, void *b, const t_commrec *cr)
{
#ifdef GMX_MPI
    MPI_Bcast(b, nbytes, MPI_BYTE, MASTERRANK(cr), cr->mpi_comm_mygroup);
#endif
}

void gmx_sumd(int nr, double r[], const t_commrec *cr)
{
#if defined(GMX_MPI)
    MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_DOUBLE, MPI_SUM,
                  cr->mpi_comm_mygroup);
#endif
}
