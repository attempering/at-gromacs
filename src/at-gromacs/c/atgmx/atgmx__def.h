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

#ifndef ATGMX__DEF_H__
#define ATGMX__DEF_H__

#define AT__GROMACS 1
#include "at-engine/at-engine__def.h"

#include "context/atgmx_context.h"

typedef struct atgmx_t_
{
  at_bool_t enabled;
  at_bool_t is_main_node;

  at_t at[1];

#ifdef GMX_MPI
  MPI_Comm mpi_comm;
#endif

  int mpi_size;
  int mpi_rank;

} atgmx_t;



#endif
