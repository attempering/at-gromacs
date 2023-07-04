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

#ifndef ATGMX_CONTEXT_GROMACS_H__
#define ATGMX_CONTEXT_GROMACS_H__


#ifndef GMX_VERSION
#include "gromacs/version.h"
#endif


// /src/gromacs/utility (5.1, 2021)
// /include (4.6)
#include "gromacs/utility/smalloc.h"

#if GMX_VERSION >= 20160000
// /src/gromacs/gmxlib (2021)
#include "gromacs/gmxlib/network.h"
#else
#include "gromacs/legacyheaders/network.h"
#endif

#if GMX_VERSION >= 20160000
// (2021)
#include "gromacs/mdlib/vsite.h"
#else
// (5.0)
#include "gromacs/legacyheaders/network.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/mdlib/enerdata_utils.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/mdtypes/enerdata.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/mdtypes/pull_params.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/mdtypes/multipletimestepping.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/utility/keyvaluetree.h"
#endif

// t_inputrec
#if GMX_VERSION >= 20160000
// (2021)
#include "gromacs/mdtypes/inputrec.h"
#else
// (5.0)
#include "gromacs/legacyheaders/inputrec.h"
#endif

// t_commrec, MASTER(), PAR()
#if GMX_VERSION >= 20160000
// (2021)
#include "gromacs/mdtypes/commrec.h"
#else
// (5.0)
#include "gromacs/legacyheaders/types/commrec.h"
#endif

#ifdef MASTER
#define ATGMX_IS_MAIN_RANK(cr) MASTER(cr)
#else
#define ATGMX_IS_MAIN_RANK(cr) MAIN(cr)
#endif


#if GMX_VERSION >= 20160000
// (2021)
// t_mdatoms
#include "gromacs/mdtypes/mdatom.h"
#else
// (5.0)
#include "gromacs/legacyheaders/types/mdatom.h"
#endif

// gmx::ForceBuffersView
#if GMX_VERSION >= 20180000
// (2021)
#include "gromacs/mdtypes/forcebuffers.h"
#endif

// gmx_omp_nthreads_get();
#if GMX_VERSION >= 20160000
#include "gromacs/mdlib/gmx_omp_nthreads.h"
#endif

// getThreadAtomRange();
#if GMX_VERSION >= 20160000
#include "gromacs/mdlib/update.h"
#else 
#include "gromacs/legacyheaders/update.h"
#endif

// t_filenm
#if GMX_VERSION >= 20160000
// (2021)
#include "gromacs/commandline/filenm.h"
#else
// (5.0)
#include "gromacs/fileio/filenm.h"
#endif

// gmx_multisim_t
#if GMX_VERSION >= 20180000
// (2021)
#include "gromacs/mdrunutility/multisim.h"
#endif

// mpi.h or tmpi.h
#include "gromacs/utility/gmxmpi.h"

#include "gromacs/fileio/gmxfio.h"

// for formatStringV()
//#include "gromacs/utility/stringutil.h"

#endif
