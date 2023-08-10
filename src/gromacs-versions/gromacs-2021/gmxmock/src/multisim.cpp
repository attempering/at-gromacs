#include "gromacs/mdrunutility/multisim.h"

gmx_multisim_t::gmx_multisim_t(int numSimulations, int simulationIndex, MPI_Comm mainRankComm, MPI_Comm simulationComm) :
    numSimulations_(numSimulations),
    simulationIndex_(simulationIndex),
    mastersComm_(mainRankComm),
    simulationComm_(simulationComm)
{
}

gmx_multisim_t::~gmx_multisim_t()
{}

#if GMX_MPI
static void gmx_sumd_comm(int nr, double r[], MPI_Comm mpi_comm)
{
    MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_DOUBLE, MPI_SUM, mpi_comm);
}
#endif

void gmx_sumd_sim(int gmx_unused nr, double gmx_unused r[], const gmx_multisim_t gmx_unused* ms)
{
#if GMX_MPI
    gmx_sumd_comm(nr, r, ms->mastersComm_);
#endif
}

void gmx_sumi_sim(int gmx_unused nr, int gmx_unused r[], const gmx_multisim_t gmx_unused* ms)
{
#if GMX_MPI
    MPI_Allreduce(MPI_IN_PLACE, r, nr, MPI_INT, MPI_SUM, ms->mastersComm_);
#endif
}