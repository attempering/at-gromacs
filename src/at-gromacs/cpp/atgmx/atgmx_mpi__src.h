/* 
 * Copyright (C) 2010-2023  AT-Engine Developers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (atgmx your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ATGMX_MPI__SRC_H__
#define ATGMX_MPI__SRC_H__
#ifdef GMX_MPI

#include "atgmx.h"

namespace atgmx
{

// This function should be called even if AtGmx is disabled
// so that members of the class
// mpi_rank, mpi_size, mpi_comm, isMainNode_ are properly set
//
int AtGmx::initMpi(MPI_Comm comm)
{
  int mpiSize = 1;
  int mpiRank = 0;

  if (comm != MPI_COMM_NULL) {
    if (MPI_SUCCESS != MPI_Comm_size(comm, &mpiSize)) {
      fprintf(stderr, "Error@atgmx.mpi: cannot get MPI size\n");
      fprintf(stderr, "    src: %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  if (mpiSize > 1) {

    if (MPI_SUCCESS != MPI_Comm_rank(comm, &mpiRank)) {
      fprintf(stderr, "Error@atgmx.mpi: cannot get MPI rank\n");
      fprintf(stderr, "    src: %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }

    if (MPI_SUCCESS != MPI_Bcast(&enabled_, sizeof(enabled_), MPI_BYTE, 0, comm)) {
      fprintf(stderr, "Error@atgmx.mpi: %3d/%3d: failed to broadcast `enabled_`, type = bool, comm = 0x%lX\n",
          mpiRank, mpiSize, (unsigned long) comm);
      fprintf(stderr, "FILE: %s, LINE: %d\n", __FILE__, __LINE__);
      exit(1);
    }

    if (MPI_SUCCESS != MPI_Bcast(at_, sizeof(at_t), MPI_BYTE, 0, comm)) {
      fprintf(stderr, "Error@atgmx.mpi: %3d/%3d: failed to broadcast at (%p), type = at_t, comm = 0x%lX\n",
          mpiRank, mpiSize, static_cast<void *>(at_), (unsigned long) comm);
      fprintf(stderr, "FILE: %s, LINE: %d\n", __FILE__, __LINE__);
      exit(1);
    }

  }

  mpiComm_ = comm;
  mpiSize_ = mpiSize;
  mpiRank_ = mpiRank;

  isMainNode_ = (mpiRank_ == 0);

  return 0;
}

} // namespace atgmx

#endif /* defined(GMX_MPI) */
#endif
