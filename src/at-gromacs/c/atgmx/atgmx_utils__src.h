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

#ifndef ATGMX_UTILS__SRC_H__
#define ATGMX_UTILS__SRC_H__

#include "atgmx.h"

#include "at-engine/at-engine.h"



/* to be used as a replacement of opt2fn(),
 * it will replace the file extension from .mdp to .cfg */
char *atgmx__opt2fn(const char *opt, int nfile, const t_filenm fnm[])
{
  int i;

  for (i = 0; i < nfile; i++) {

    if (strcmp(opt, fnm[i].opt) == 0) { // a match is found
      char *fname = fnm[i].fns[0];

      /*
      if (fnm[i].ftp == efCFG) {
        char *p;

        // modify the extension from .mdp to .cfg
        if (strcmp(fname, "grompp.mdp") == 0) { // replace the default name by NULL
          return NULL; // we do not have default name for .cfg files
        } else if ((p = strstr(fname, ".cfg.mdp")) != NULL) {
          p[4] = '\0';
        } else if ((p = strstr(fname, ".mdp")) != NULL) {
          strcpy(p, ".cfg");
        }
      }
      */

      return fname;
    }

  }

  return NULL;
}



/* update the current scale for all nodes */
void atgmx__update_force_scale(atgmx_t *atgmx, t_commrec *cr)
{
  if (!atgmx->enabled) {
    return;
  }

  /* call the single processor version */
  if (ATGMX__IS_MAIN_RANK(cr)) {
    at__update_force_scale(atgmx->at);
  }

#ifdef GMX_MPI
  /* inform all other PP nodes the new scale(s) */
  if (PAR(cr)) {
    gmx_bcast(sizeof(atgmx->at->force_scale), &atgmx->at->force_scale, cr);
  }
#endif

}



at_bool_t atgmx__do_tempering_on_step(atgmx_t *atgmx, at_llong_t step,
    at_bool_t is_ns_step)
{
  if (!atgmx->enabled) {
    return AT__FALSE;
  }

  int nst_tempering = atgmx->at->driver->nst_tempering;
  at_bool_t do_tempering;

  if (nst_tempering > 0) {
    do_tempering = (step % nst_tempering) == 0;
  } else {
    do_tempering = is_ns_step;
  }

  return do_tempering;
}



static void atgmx__sum_energy(
    atgmx_t *atgmx,
    real *eterm,
    t_commrec *cr,
    at_llong_t step,
    at_bool_t dirty)
{
  double epot = eterm[F_EPOT];

  if (dirty) { /* summarize local potential energy */
    zcom_utils__exit_if (!PAR(cr),
        "node: %d, step: " at_llong_pfmt ": no energy available\n",
        cr->nodeid, step);

#ifdef GMX_MPI
    gmx_sumd(1, &epot, cr);
#endif

  }

  if (ATGMX__IS_MAIN_RANK(cr)) {
    atgmx->at->energy = epot;
  }

}



/* only a PP-processor calls this function
 * also assume global_stat() has been called */
int atgmx__move(atgmx_t *atgmx,
    gmx_enerdata_t *enerd,
    at_llong_t step,
    at_bool_t is_first_step,
    at_bool_t is_last_step,
    at_bool_t has_global_stats,
    at_bool_t is_xtc_step,
    at_bool_t is_ns_step,
    t_commrec *cr)
{
  at_bool_t do_tempering;
  at_params_step_t step_params[1];

  step_params->step = (at_llong_t) step;
  step_params->is_first_step = is_first_step;
  step_params->is_last_step = is_last_step;
  step_params->do_trace = is_xtc_step;
  step_params->flush_output = AT__FALSE;

  do_tempering = atgmx__do_tempering_on_step(atgmx, step,
      is_ns_step);

  if (do_tempering) {

    at_bool_t dirty = PAR(cr) && !has_global_stats;

    /* summarize the relevant energy */
    atgmx__sum_energy(atgmx, enerd->term, cr, step, dirty);

    /* change temperature, and regularly write output files */
    if (ATGMX__IS_MAIN_RANK(cr)) {

      // calling at__move()
      zcom_utils__exit_if(0 != at__move(atgmx->at, step_params),
          "#%d, step = " at_llong_pfmt ", error during moving master\n",
          cr->nodeid, step);
    }

    // update atgmx->at->force_scale
    atgmx__update_force_scale(atgmx, cr);

  } else {

    // not doing tempering, we may need to output
    // if tempering is done, output tasks are already
    // considered in at__move()
    if (ATGMX__IS_MAIN_RANK(cr)) {
      at__output(atgmx->at, step_params);
    }

  }

  return 0;
}



void atgmx__scale_force(atgmx_t *atgmx,
    rvec f[], t_mdatoms *mdatoms)
{
  int k, start, end;
  real scale;

  if (!atgmx->enabled) {
    return;
  }

  /* scale the force */
  scale = (real) atgmx->at->force_scale;

#if GMX_VERSION < 50000
  start = mdatoms->start;
  end = mdatoms->start + mdatoms->homenr;
#else
  start = 0;
  end = mdatoms->homenr;
#endif

  for (k = start; k < end; k++) {
    f[k][0] *= scale;
    f[k][1] *= scale;
    f[k][2] *= scale;
  }
}



double atgmx__get_beta(const atgmx_t *atgmx)
{
  return at__get_beta(atgmx->at);
}


#endif
