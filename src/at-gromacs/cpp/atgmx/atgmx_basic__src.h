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

#include "atgmx.h"
//#include "atgmx_mpi.h"

namespace atgmx
{

void AtGmx::updateThermostatTemperatures(const t_inputrec *ir) const
{
  int i;
  const double tol = 0.5;
  const real refTemp = static_cast<real>(at_->utils->thermostat_temp);

  if (!enabled_) {
    logger_->error("trying to call update_thermostat_temperatures without enabling atgmx\n");
    return;
  }

  for (i = 0; i < ir->opts.ngtc; i++) {
    if (std::fabs(ir->opts.ref_t[i] - refTemp) > tol) {
      logger_->warning("trying to modify Gromacs thermostat group temperature from %g to %g\n",
          ir->opts.ref_t[i], refTemp);
    }
    ir->opts.ref_t[i] = refTemp;
  }
}



static bool is_multi_sim(const gmx_multisim_t *ms)
{
  return ms != nullptr;
}

static int get_multisim_sim_id(const gmx_multisim_t *ms)
{
  if (is_multi_sim(ms)) {

#if GMX_VERSION >= 20210000
    return ms->simulationIndex_;
#else
    return ms->sim;
#endif

  } else {
    return 0;
  }
}



AtGmx::AtGmx(
    const char *fn_cfg,
    const t_inputrec *ir,
    t_commrec *cr,
    const gmx_multisim_t *ms,
    bool multiDirs,
    bool isContinuation,
    at_flags_t flags)
{
  enabled_ = (fn_cfg != nullptr);

  isMainNode_ = ATGMX__IS_MAIN_RANK(cr);

  if (enabled_ && isMainNode_) {
    at_params_sys_t sys_params[1];

#if GMX_VERSION >= 20220000
    sys_params->boltz = gmx::c_boltz;
#else
    sys_params->boltz = BOLTZ;
#endif

    sys_params->md_time_step = ir->delta_t;
    sys_params->sim_id = get_multisim_sim_id(ms);
    sys_params->add_suffix = ((is_multi_sim(ms) && !multiDirs) ? AT__TRUE : AT__FALSE);
    sys_params->is_continuation = static_cast<at_bool_t>(isContinuation);

    // This call may fail if the configuration file doesn't exist
    // or its content contains an error
    //
    // Our policy is to throw an exception if the filename
    // is given, but it doesn't exist or it contains an error.
    //
    // However, if the filename is not given,
    // i.e., the command-line options `-at at.cfg` is not set,
    // we will simply set `enabled_` to false
    //
    if (at__init(at_, fn_cfg, sys_params, flags) != 0) {
      throw;
    }
  }
  //fprintf(stderr, "enabled %d, isMainNode_ %d\n", enabled, isMainNode_); getchar();

  initLogger(isContinuation);

#if ATGMX_MPI
  // tell every node the settings on the main node
  // valid only for PP only node, maybe we need to
  // consider using mpi_comm_mysim for more advanced versions
  // we pass MPI_COMM_NULL to avoid the case of one-node-mpi
  //
  // This function should be called even if AtGmx is disabled
  // so that members mpiRank_, mpiSize_, mpiComm_, isMainNode_
  // are properly set
  //
  if (PAR(cr)) {
    initMpi(PAR(cr) ? cr->mpi_comm_mygroup : MPI_COMM_NULL);
  }
#endif

  if (enabled_) {
    // in the MPI case, every node updates its
    // thermostat temperature and force scale.
    // So the following calls are not limited to the master.
    updateThermostatTemperatures(ir);
    updateForceScale(cr);

    if (isMainNode_) {
      at__manifest(at_);
    }
  }

}



AtGmx::~AtGmx()
{
  if (enabled_ && isMainNode_) {
    at__finish(at_);
  }
}


} // namespace atgmx

#endif
