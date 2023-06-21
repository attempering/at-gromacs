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

#ifndef ATGMX_BASIC_H__
#define ATGMX_BASIC_H__

#include "atgmx.h"

#include "at-engine/at-engine.h"
#include "context/atgmx_context.h"

int atgmx__init(
    atgmx_t *atgmx,
    const char *fn_cfg,
    t_inputrec *ir,
    t_commrec *cr,
    at_bool_t is_continuation,
    at_flags_t flags);

void atgmx__finish(atgmx_t *atgmx);

#endif
