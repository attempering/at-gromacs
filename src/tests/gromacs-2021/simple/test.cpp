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

// running a simple Gaussian energy model
double sigma = 100.0;

at_llong_t nsteps = 1000000;

void init_gromacs_vars(t_commrec *cr, t_inputrec *ir, gmx_enerdata_t *enerd)
{
  int world_size = 1, world_rank = 0;

#if ATGMX_MPI
  cr->mpi_comm_mygroup = MPI_COMM_WORLD;
  cr->mpi_comm_mysim = MPI_COMM_WORLD;

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  fprintf(stderr, "node %d/%d\n", world_rank, world_size);
#endif

  cr->nodeid = world_rank;
  cr->nnodes = world_size;

  //cr->ms = NULL; // TODO

  ir->delta_t = 0.002;

  ir->opts.ngtc = 1;
  ir->opts.ref_t = (real *) calloc(1, sizeof(real));
  ir->opts.ref_t[0] = 300;

  enerd->term[F_EPOT] = 0.0;
}


int work(int argc, char **argv)
{
  at_llong_t step = 0;
  auto cr = std::make_shared<t_commrec>();
  auto ir = std::make_unique<t_inputrec>();
  auto enerd = std::make_shared<gmx_enerdata_t>(1, 0);
  at_bool_t multi_dirs = AT__TRUE;
  at_bool_t from_cpt = AT__FALSE;
  zcom_mtrng_t rng[1];

  // initialize GROMACS variables
  init_gromacs_vars(cr.get(), ir.get(), enerd.get());

  auto atgmx = atgmx::AtGmx("at.cfg", ir.get(), cr.get(), 
      nullptr, multi_dirs, from_cpt, AT__INIT_VERBOSE);

  zcom_mtrng__init_from_seed(rng, 12345);

  if (ATGMX__IS_MAIN_RANK(cr)) {
    remove("atdata/trace.dat");
  }

  for (step = 1; step <= nsteps; step++) {
    at_bool_t is_first_step = (step == 1);
    at_bool_t is_last_step = (step == nsteps);
    at_bool_t has_global_stats = AT__TRUE;
    at_bool_t is_xtc_step = AT__TRUE;
    at_bool_t is_ns_step = AT__TRUE;
    double epot = 0;

    if (ATGMX__IS_MAIN_RANK(cr)) {
      epot = -sigma * sigma * atgmx.getBeta()
          + sigma * zcom_mtrng__rand_gauss(rng);
    } else {
      epot = 0;
    }

    enerd->term[F_EPOT] = epot;

    atgmx.move(enerd.get(), step,
        is_first_step, is_last_step, has_global_stats,
        is_xtc_step, is_ns_step,
        cr.get());

    if (ATGMX__IS_MAIN_RANK(cr) && step % 1000 == 0) {
      printf("step %ld: beta %g epot %g\n", (long) step, atgmx.getBeta(), epot);
    }
  }

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
