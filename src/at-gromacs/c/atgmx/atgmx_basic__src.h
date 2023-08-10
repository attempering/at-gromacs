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

#include "atgmx_basic.h"
#include "atgmx_mpi.h"
#include "atgmx_utils.h"



static void atgmx__update_thermostat_temperatures(atgmx_t *atgmx, t_inputrec *ir)
{
  int i;

  for (i = 0; i < ir->opts.ngtc; i++) {
    ir->opts.ref_t[i] = (real) atgmx->at->utils->thermostat_temp;
  }
}



int atgmx__init(
    atgmx_t *atgmx,
    const char *fn_cfg,
    t_inputrec *ir,
    t_commrec *cr,
    const gmx_multisim_t *ms,
    at_bool_t multi_dirs,
    at_bool_t is_continuation,
    at_flags_t flags)
{
  atgmx->enabled = ((fn_cfg != NULL) ? AT__TRUE : AT__FALSE);

  atgmx->is_main_node = (ATGMX__IS_MAIN_RANK(cr) ? AT__TRUE : AT__FALSE);

  if (atgmx->enabled && atgmx->is_main_node) {
    at_params_sys_t sys_params[1];

    sys_params->boltz = BOLTZ;
    sys_params->md_time_step = ir->delta_t;
    sys_params->sim_id = (ms != NULL ? ms->sim : 0);
    sys_params->add_suffix = (ms != NULL && !multi_dirs) ? AT__TRUE : AT__FALSE;
    sys_params->is_continuation = is_continuation;

    zcom_utils__exit_if (at__init(atgmx->at, fn_cfg, sys_params, flags) != 0,
        "failed to initialize at from \"%s\"\n", fn_cfg);
  }

  //init_logger();

#ifdef GMX_MPI
  /* tell every node the settings on the main node
   * valid only for PP only node, maybe we need to
   * consider using mpi_comm_mysim for more advanced versions
   * we pass MPI_COMM_NULL to avoid the case of one-node-mpi */
  if (PAR(cr)) {
    atgmx__init_mpi(atgmx, PAR(cr) ? cr->mpi_comm_mygroup : MPI_COMM_NULL);
  }
#endif

  if (atgmx->enabled) {
    atgmx__update_thermostat_temperatures(atgmx, ir);

    atgmx__update_force_scale(atgmx, cr);

    if (atgmx->is_main_node) {
      at__manifest(atgmx->at);
    }
  }

  return 0;
}



void atgmx__finish(atgmx_t *atgmx)
{
  if (atgmx->enabled && atgmx->is_main_node) {
    at__finish(atgmx->at);
  }
}


#endif
