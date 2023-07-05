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

#ifndef ATGMX_CONTEXT_H__
#define ATGMX_CONTEXT_H__


#include <string.h>
#include <signal.h>
#include "typedefs.h"
#include "physics.h"

#if GMX_VERSION >= 50000
  #include "gromacs/utility/smalloc.h"
#else
  #include "smalloc.h"
#endif

#if GMX_VERSION >= 50000
  #include "gromacs/legacyheaders/network.h"
  #include "gromacs/legacyheaders/vsite.h"
#else
  #include "network.h"
  #include "vsite.h"
#endif

#ifdef GMX_LIB_MPI
#include <mpi.h>
#endif
#ifdef GMX_THREADS
#include "tmpi.h"
#endif

#endif
