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
/*
__inline const char *atgmx__opt2fn(const char *opt, int nfile, const t_filenm fnm[])
{
  const t_filenm* fileOption = nullptr;

  for (int i = 0; i < nfile; i++)
  {
    if ((fnm[i].opt != nullptr && strcmp(opt, fnm[i].opt) == 0))
    {
      fileOption = &fnm[i];
      break;
    }
  }

  if (fileOption)
  {
    return fileOption->filenames[0].c_str();
  }

  return nullptr;
}
*/



double AtGmx::get_beta() {
  return at__get_beta(at);
}



/* update the current scale for all nodes */
void AtGmx::update_force_scale(t_commrec *cr)
{
  if (!enabled) {
    return;
  }

  // call the single processor version on the main rank
  if (ATGMX_IS_MAIN_RANK(cr)) {
    at__update_force_scale(at);
  }

#ifdef GMX_MPI
  /* inform all other PP nodes the new scale(s) */
  if (PAR(cr)) {
#if GMX_VERSION >= 20210000
    gmx_bcast(sizeof(at->force_scale), &at->force_scale, cr->mpi_comm_mygroup);
#else
    gmx_bcast(sizeof(at->force_scale), &at->force_scale, cr);
#endif
  }
#endif

}



bool AtGmx::do_tempering_on_step(at_llong_t step,
    bool is_ns_step)
{
  if (!enabled) {
    return false;
  }

  int nsttemp = at->driver->nsttemp;
  bool do_tempering;

  if (nsttemp > 0) {
    do_tempering = (step % nsttemp) == 0;
  } else {
    do_tempering = is_ns_step;
  }

  return do_tempering;
}



void AtGmx::sum_energy(
#if GMX_VERSION >= 20220000
    const std::array<real, F_NRE>& eterm,
#else
    const real *eterm,
#endif
    t_commrec *cr,
    at_llong_t step,
    bool dirty)
{
  double epot = eterm[F_EPOT];

  if (!enabled) {
    fprintf(stderr, "\rError@atgmx: trying to call sum_energy() without enabling atgmx\n");
    return;
  }

  if (dirty) { /* summarize local potential energy */
    zcom_util__exit_if (!PAR(cr),
      "node: %d, step: " at_llong_pfmt ": no energy available\n",
        cr->nodeid, step);

#ifdef GMX_MPI
    gmx_sumd(1, &epot, cr);
#endif

  }

  if (ATGMX_IS_MAIN_RANK(cr)) {
    at->energy = epot;
  }

}



/* only a PP-processor calls this function
 * also assume global_stat() has been called */
int AtGmx::move(
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

  if (!enabled) {
    //fprintf(stderr, "\rError@atgmx: trying to call move() without enabling atgmx\n");
    return -1;
  }

  step_params->step = step;
  step_params->is_first_step = is_first_step;
  step_params->is_last_step = is_last_step;
  step_params->do_trace = is_xtc_step;
  step_params->flush_output = AT__FALSE;

  do_tempering = do_tempering_on_step(step,
      is_ns_step);

  if (do_tempering) {

    at_bool_t dirty = PAR(cr) && !has_global_stats;

    /* summarize the relevant energy */
    sum_energy(enerd->term, cr, step, dirty);

    /* change temperature, and regularly write output files */
    if (ATGMX_IS_MAIN_RANK(cr)) {

      // calling at__move()
      zcom_util__exit_if(0 != at__move(at, step_params),
          "#%d, step = " at_llong_pfmt ", error during moving master\n",
          cr->nodeid, step);
    }

    // update atgmx->at->force_scale
    update_force_scale(cr);

  } else {

    // not doing tempering, we may need to output
    // if tempering is done, output tasks are already
    // considered in at__move()
    if (ATGMX_IS_MAIN_RANK(cr)) {
      at__output(at, step_params);
    }

  }

  return 0;
}


#if GMX_VERSION >= 20210000
void AtGmx::scale_force(gmx::ForceBuffersView& forceView, const t_mdatoms *mdatoms)
{
  if (!enabled) {
    return;
  }

  real fs = (real) at->force_scale;

  auto f = forceView.forceWithPadding();
  int homenr = mdatoms->homenr;

#if GMX_VERSION >= 20220000
  int nth = gmx_omp_nthreads_get(ModuleMultiThread::Update);
#else
  int nth = gmx_omp_nthreads_get(emntUpdate);
#endif

#pragma omp parallel for num_threads(nth) schedule(static)
  for (int th = 0; th < nth; th++) {
    try
    {
      int start_th, end_th;
      getThreadAtomRange(nth, th, homenr, &start_th, &end_th);

      rvec* f_rvec  = as_rvec_array(f.unpaddedArrayRef().data());
      for (int i = start_th; i < end_th; i++) {
        for (int d = 0; d < DIM; d++) {
          f_rvec[i][d] *= fs;
        }
      }
    }
    GMX_CATCH_ALL_AND_EXIT_WITH_FATAL_ERROR
  }

}

#else
// for versions up to 2020

void AtGmx::scale_force(rvec f[], const t_mdatoms *mdatoms)
{
  if (!enabled) {
    return;
  }

  /* scale the force */
  real fs = (real) at->force_scale;

  int homenr = mdatoms->homenr;
  int nth = gmx_omp_nthreads_get(emntUpdate);

#pragma omp parallel for num_threads(nth) schedule(static)
  for (int th = 0; th < nth; th++) {
    int start_th = ((th+0)*homenr)/nth;
    int end_th   = ((th+1)*homenr)/nth;

    for (int i = start_th; i < end_th; i++) {
      for (int d = 0; d < DIM; d++) {
        f[i][d] *= fs;
      }
    }
  }

}

#endif



#endif
