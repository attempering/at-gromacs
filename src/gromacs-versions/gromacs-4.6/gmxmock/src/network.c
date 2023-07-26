#include "network.h"

void gmx_bcast(int nbytes, void *b, const t_commrec *cr)
{
#ifdef GMX_MPI
  MPI_Bcast(b, nbytes, MPI_BYTE, MASTERRANK(cr), cr->mpi_comm_mygroup);
#endif
}

void gmx_sumd(int nr, double r[], const t_commrec *cr)
{
#ifdef GMX_MPI
  MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_DOUBLE, MPI_SUM,
                cr->mpi_comm_mygroup);
#endif
}

void gmx_sumi_sim(int nr, int r[], const gmx_multisim_t *ms)
{
#ifdef GMX_MPI
  MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_INT, MPI_SUM, ms->mpi_comm_masters);
#endif
}

#ifdef GMX_MPI
void gmx_sumf_comm(int nr, float r[], MPI_Comm mpi_comm)
{
  MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_FLOAT, MPI_SUM, mpi_comm);
}

void gmx_sumd_comm(int nr, double r[], MPI_Comm mpi_comm)
{
  MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_DOUBLE, MPI_SUM, mpi_comm);
}
#endif

void gmx_sumf_sim(int nr, float r[], const gmx_multisim_t *ms)
{
#ifdef GMX_MPI
  gmx_sumf_comm(nr, r, ms->mpi_comm_masters);
#endif
}

void gmx_sumd_sim(int nr, double r[], const gmx_multisim_t *ms)
{
#ifdef GMX_MPI
  gmx_sumd_comm(nr, r, ms->mpi_comm_masters);
#endif
}
