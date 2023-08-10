/* 
 * Copyright (C) 2010-2023  AT-Engine Developers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

#include "at-gromacs/at-gromacs__src.h"

int work(int argc, char **argv)
{
  int world_size = 1, world_rank = 0;

#if ATGMX_MPI
  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

  printf("node %d/%d\n", world_rank, world_size);

#if ATGMX_MPI
  MPI_Finalize();
#endif

  return 0;
}

int main(int argc, char **argv)
{
#if GMX_LIB_MPI
  MPI_Init(&argc, &argv);
#endif

#if GMX_THREAD_MPI
  tMPI_Init(&argc, &argv, &work);
#endif

  work(argc, argv);

  return 0;
}
