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

#ifndef ATGMX_H__
#define ATGMX_H__

#include "logger/atgmx_logger.h"

namespace gmx
{

class AtGmx
{

public:

  AtGmx(const char *fnCfg,
        const t_inputrec *ir,
        t_commrec *cr,
        bool isContinuation,
        bool multiSims,
        at_flags_t flags);

  ~AtGmx();

  void initLogger(bool isContinuation);

  int move(
    gmx_enerdata_t *enerd,
    at_llong_t step,
    bool isFirstStep,
    bool isLastStep,
    bool hasGlobalStats,
    bool isXtcStep,
    bool isNsStep,
    t_commrec *cr);

#if GMX_VERSION >= 20210000
  void scaleForce(gmx::ForceBuffersView& forceView, const t_mdatoms *mdatoms);
#else
  void scaleForce(rvec f[], const t_mdatoms *mdatoms);
#endif

  void updateThermostatTemperatures(const t_inputrec *ir) const;

  int initMpi(MPI_Comm comm);

  double getBeta();

  bool doTemperingOnStep(at_llong_t step,
      gmx_bool isNsStep);

  void updateForceScale(t_commrec *cr);

  void sumEnergy(
#if GMX_VERSION >= 20220000
      const std::array<real, F_NRE>& eterm,
#else
      const real *eterm,
#endif
      t_commrec *cr,
      at_llong_t step,
      bool dirty);

private:

  bool enabled_;
  bool isMainNode_;

  at_t at_[1];

  std::shared_ptr<AtGmxLogger> logger_;

#ifdef GMX_MPI
  MPI_Comm mpiComm_;
#endif

  int mpiSize_;
  int mpiRank_;

};

} // namespace gmx

#endif
