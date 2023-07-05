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


// This header applies to the C++ code of GROMACS
// and it only applies to GROMACS 2016 or later
//
// GROMACS 4.6, 5.0, 5.1 are not supported.
//

#ifndef GMX_VERSION
#include "gromacs/version.h"
#endif

// To figure out the path of certain file
// cd to the directory containing all gromacs versions
// and run
//
// find . -name "vsite.h" | grep -v [45]. | grep -v 1-6
//

#include "gromacs/utility/smalloc.h"

#include "gromacs/gmxlib/network.h"

#include "gromacs/mdlib/vsite.h"

// for t_inputrec
#include "gromacs/mdtypes/inputrec.h"

// for t_commrec, MASTER(), PAR()
#include "gromacs/mdtypes/commrec.h"

#if GMX_VERSION >= 20230000
#define ATGMX__IS_MAIN_RANK(cr) MAIN(cr)
#else
#define ATGMX__IS_MAIN_RANK(cr) MASTER(cr)
#endif

#if GMX_VERSION >= 20190000
#include "gromacs/mdtypes/enerdata.h"
#endif

#if GMX_VERSION >= 20200000
#include "gromacs/mdlib/enerdata_utils.h"
#endif

#if GMX_VERSION >= 20200000
#include "gromacs/mdtypes/pull_params.h"
#endif

#if GMX_VERSION >= 20210000
#include "gromacs/mdtypes/multipletimestepping.h"
#endif

#if GMX_VERSION >= 20180000
#include "gromacs/utility/keyvaluetree.h"
#endif

#include "gromacs/mdtypes/mdatom.h"

// gmx::ForceBuffersView
#if GMX_VERSION >= 20210000
// (2021)
#include "gromacs/mdtypes/forcebuffers.h"
#endif

// gmx_omp_nthreads_get();
#include "gromacs/mdlib/gmx_omp_nthreads.h"

// for getThreadAtomRange();
#include "gromacs/mdlib/update.h"

// t_filenm
#include "gromacs/commandline/filenm.h"

// gmx_multisim_t
#if GMX_VERSION >= 20200000
#include "gromacs/mdrunutility/multisim.h"
#elif GMX_VERSION >= 20190000
#include "gromacs/mdrun/multisim.h"
#endif

// mpi.h or tmpi.h
#include "gromacs/utility/gmxmpi.h"

#include "gromacs/fileio/gmxfio.h"

// formatStringV() is defined only for GROMACS 2018 or later
#include "gromacs/utility/stringutil.h"

#endif
