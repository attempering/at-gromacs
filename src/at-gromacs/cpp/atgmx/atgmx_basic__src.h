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

#ifndef ATGMX_BASIC__SRC_H__
#define ATGMX_BASIC__SRC_H__

#include "atgmx__def.h"
//#include "atgmx_mpi.h"


void AtGmx::update_thermostat_temperatures(const t_inputrec *ir) const
{
  int i;
  const double tol = 0.5;
  const real ref_temp = static_cast<real>(at->utils->thermostat_temp);

  if (!enabled) {
    fprintf(stderr, "\rError@atgmx: trying to call update_thermostat_temperatures without enabling atgmx\n");
    return;
  }

  for (i = 0; i < ir->opts.ngtc; i++) {
    if (std::fabs(ir->opts.ref_t[i] - ref_temp) > tol) {
      fprintf(stderr, "\rWarning@atgmx: trying to modify Gromacs thermostat group temperature from %g to %g\n",
        ir->opts.ref_t[i], ref_temp);
    }
    ir->opts.ref_t[i] = ref_temp;
  }
}



AtGmx::AtGmx(
    const char *fn_cfg,
    const t_inputrec *ir,
    t_commrec *cr,
    at_bool_t is_continuation,
    at_bool_t multi_sims,
    at_flags_t flags)
{
  if (fn_cfg != nullptr) {
    enabled = AT__TRUE;
  } else {
    enabled = AT__FALSE;
  }

  is_master = (ATGMX_IS_MAIN_RANK(cr) ? AT__TRUE : AT__FALSE);

  if (enabled && is_master) {
    at_params_sys_t sys_params[1];

#if GMX_VERSION >= 20220000
    sys_params->boltz = gmx::c_boltz;
#else
    sys_params->boltz = BOLTZ;
#endif

    sys_params->sim_id = 0;
    sys_params->md_time_step = ir->delta_t;
    sys_params->multi_sims = multi_sims;
    sys_params->is_continuation = is_continuation;

    // This call may fail is the configuration doesn't exist
    // or its content contains an error
    //
    // Our policy is to throw an exception if the filename
    // is given, but it doesn't exist or it contains an error.
    //
    // However, if the filename is not given,
    // i.e., the command-line options `-at at.cfg` is not set,
    // we will simply set `enabled` to false
    //
    if (at__init(at, fn_cfg, sys_params, flags) != 0) {
      throw;
    }
  }
  //fprintf(stderr, "enabled %d, is_master %d\n", enabled, is_master); getchar();

#ifdef GMX_MPI
  // tell every node the settings on the master
  // valid only for PP only node, maybe we need to
  // consider using mpi_comm_mysim for more advanced versions
  // we pass MPI_COMM_NULL to avoid the case of one-node-mpi
  //
  // This function should be called even if AtGmx is disabled
  // so that members mpi_rank, mpi_size, mpi_comm, is_master are properly set
  //
  if (PAR(cr)) {
    init_mpi(PAR(cr) ? cr->mpi_comm_mygroup : MPI_COMM_NULL);
  }

#endif

  if (enabled) {
    // every node in the MPI case should updates its thermostat temperature(s)
    update_thermostat_temperatures(ir);

    // every node in the MPI case should update its force scale
    update_force_scale(cr);

    if (is_master) {
      at__manifest(at);
    }
  }

}



AtGmx::~AtGmx()
{
  if (enabled && is_master) {
    at__finish(at);
  }
}


#endif
