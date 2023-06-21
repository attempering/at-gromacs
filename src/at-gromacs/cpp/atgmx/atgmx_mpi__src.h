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

#include "atgmx__def.h"

// This function should be called even if AtGmx is disabled
// so that members of the class
// mpi_rank, mpi_size, mpi_comm, is_master are properly set
//
int AtGmx::init_mpi(MPI_Comm comm)
{
  int mpi_size_l = 1;
  int mpi_rank_l = 0;

  if (comm != MPI_COMM_NULL) {
    if (MPI_SUCCESS != MPI_Comm_size(comm, &mpi_size_l)) {
      fprintf(stderr, "cannot even get MPI size\n");
      fprintf(stderr, "    src: %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }
  }
  if (mpi_size_l > 1) {
    if (MPI_SUCCESS != MPI_Comm_rank(comm, &mpi_rank_l)) {
      fprintf(stderr, "cannot get MPI rank\n");
      fprintf(stderr, "    src: %s:%d\n", __FILE__, __LINE__);
      exit(1);
    }
    if (MPI_SUCCESS != MPI_Bcast(this, sizeof(*this), MPI_BYTE, 0, comm)) {
      fprintf(stderr, "%3d/%3d: failed to bcast atgmx (%p), type = *atgmx, size = 1 (%d), comm = 0x%lX\n",
          mpi_rank_l, mpi_size_l, (void *) this, 1, (unsigned long) comm);
      fprintf(stderr, "FILE: %s, LINE: %d\n", __FILE__, __LINE__);
      exit(1);
    }
  }

  mpi_comm = comm;
  mpi_size = mpi_size_l;
  mpi_rank = mpi_rank_l;

  is_master = (mpi_rank == 0);

  return 0;
}


#endif /* defined(GMX_MPI) */
#endif
