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

#ifndef ATGMX__DEF_H__
#define ATGMX__DEF_H__

#define AT__GROMACS 1
#include "at-engine/at-engine__def.h"

#include "context/atgmx_context.h"

class AtGmx
{
public:

  at_bool_t enabled;
  at_bool_t is_master;

  at_t at[1];

#ifdef GMX_MPI
  MPI_Comm mpi_comm;
#endif

  int mpi_size;
  int mpi_rank;

public:

  AtGmx(
    const char *fn_cfg,
    const t_inputrec *ir,
    t_commrec *cr,
    at_bool_t is_continuation,
    at_bool_t multi_sims,
    at_flags_t flags);

  ~AtGmx();

  int move(
    gmx_enerdata_t *enerd,
    at_llong_t step,
    at_bool_t is_first_step,
    at_bool_t is_last_step,
    at_bool_t has_global_stats,
    at_bool_t is_xtc_step,
    at_bool_t is_ns_step,
    t_commrec *cr);

#if GMX_VERSION >= 20210000
  void scale_force(gmx::ForceBuffersView& forceView, const t_mdatoms *mdatoms);
#else
  void scale_force(rvec f[], const t_mdatoms *mdatoms);
#endif

  void update_thermostat_temperatures(const t_inputrec *ir) const;

  int init_mpi(MPI_Comm comm);

  double get_beta();

  bool do_tempering_on_step(at_llong_t step,
      bool is_ns_step);

  void update_force_scale(t_commrec *cr);

  void sum_energy(
#if GMX_VERSION >= 20220000
      const std::array<real, F_NRE>& eterm,
#else
      const real *eterm,
#endif
      t_commrec *cr,
      at_llong_t step,
      bool dirty);

};


#endif
