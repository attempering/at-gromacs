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

namespace gmx
{

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



double AtGmx::getBeta() {
  return at__get_beta(at_);
}



/* update the current scale for all nodes */
void AtGmx::updateForceScale(t_commrec *cr)
{
  if (!enabled_) {
    return;
  }

  // call the single processor version on the main rank
  if (ATGMX_IS_MAIN_RANK(cr)) {
    at__update_force_scale(at_);
  }

#ifdef GMX_MPI
  /* inform all other PP nodes the new scale(s) */
  if (PAR(cr)) {
#if GMX_VERSION >= 20210000
    gmx_bcast(sizeof(at_->force_scale), &at_->force_scale, cr->mpi_comm_mygroup);
#else
    gmx_bcast(sizeof(at_->force_scale), &at_->force_scale, cr);
#endif
  }
#endif

}



bool AtGmx::doTemperingOnStep(at_llong_t step,
    gmx_bool isNsStep)
{
  if (!enabled_) {
    return false;
  }

  int nst_tempering = at_->driver->nst_tempering;
  bool doTempering;

  if (nst_tempering > 0) {
    doTempering = (step % nst_tempering) == 0;
  } else {
    doTempering = static_cast<bool>(isNsStep);
  }

  return doTempering;
}



void AtGmx::sumEnergy(
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

  if (!enabled_) {
    fprintf(stderr, "\rError@atgmx: trying to call sum_energy() without enabling atgmx\n");
    return;
  }

  if (dirty) { /* summarize local potential energy */
    zcom_utils__exit_if (!PAR(cr),
      "node: %d, step: " at_llong_pfmt ": no energy available\n",
        cr->nodeid, step);

#ifdef GMX_MPI
    gmx_sumd(1, &epot, cr);
#endif

  }

  if (ATGMX_IS_MAIN_RANK(cr)) {
    at_->energy = epot;
  }

}


void AtGmx::initLogger(
    bool isContinuation)
{
  if (!isMainNode_) {
    return;
  }

  std::string fnLog;
  if (enabled_) {
    fnLog = std::string(at_->utils->data_dir) + "/atgmx.log";
  } else {
    fnLog = "atgmx.log";
  }

  // initialize the logger that is active only on the main node;
  // On non-master nodes, calls return 
  logger_ = std::make_shared<AtGmxLogger>(
      fnLog, isMainNode_, isContinuation);
}


/* only a PP-processor calls this function
 * also assume global_stat() has been called */
int AtGmx::move(
    gmx_enerdata_t *enerd,
    at_llong_t step,
    bool isFirstStep,
    bool isLastStep,
    bool hasGlobalStats,
    bool isXtcStep,
    bool isNsStep,
    t_commrec *cr)
{
  if (!enabled_) {
    //fprintf(stderr, "\rError@atgmx: trying to call move() without enabling atgmx\n");
    return -1;
  }

  at_params_step_t step_params[1];

  step_params->step = step;
  step_params->is_first_step = static_cast<at_bool_t>(isFirstStep);
  step_params->is_last_step = static_cast<at_bool_t>(isLastStep);
  step_params->do_trace = static_cast<at_bool_t>(isXtcStep);
  step_params->flush_output = AT__FALSE;

  bool doTempering = doTemperingOnStep(step,
      isNsStep);

  if (doTempering) {

    bool dirty = PAR(cr) && !hasGlobalStats;

    /* summarize the relevant energy */
    sumEnergy(enerd->term, cr, step, dirty);

    /* change temperature, and regularly write output files */
    if (ATGMX_IS_MAIN_RANK(cr)) {

      // calling at__move()
      zcom_utils__exit_if(0 != at__move(at_, step_params),
          "#%d, step = " at_llong_pfmt ", error during moving master\n",
          cr->nodeid, step);
    }

    // update at_->force_scale
    updateForceScale(cr);

  } else {

    // not doing tempering, we may need to output
    // if tempering is done, output tasks are already
    // considered in at__move()
    if (ATGMX_IS_MAIN_RANK(cr)) {
      at__output(at_, step_params);
    }

  }

  return 0;
}


#if GMX_VERSION >= 20210000
void AtGmx::scaleForce(gmx::ForceBuffersView& forceView, const t_mdatoms *mdatoms)
{
  if (!enabled_) {
    return;
  }

  real fs = static_cast<real>(at_->force_scale);

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

void AtGmx::scaleForce(rvec f[], const t_mdatoms *mdatoms)
{
  if (!enabled_) {
    return;
  }

  /* scale the force */
  real fs = (real) at_->force_scale;

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

} // namespace gmx

#endif
