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

#ifndef ATGMX_UTILS_H__
#define ATGMX_UTILS_H__

#include "atgmx.h"

#include "at-engine/at-engine.h"
#include "context/atgmx_context.h"


/* to be used as a replacement of opt2fn(),
 * it will replace the file extension from .mdp to .cfg */
char *atgmx__opt2fn_null(const char *opt, int nfile, const t_filenm fnm[]);

void atgmx__update_force_scale(atgmx_t *atgmx, t_commrec *cr);

at_bool_t atgmx__do_tempering_on_step(atgmx_t *atgmx, at_llong_t step,
    at_bool_t is_ns_step);

int atgmx__move(atgmx_t *atgmx,
    gmx_enerdata_t *enerd,
    at_llong_t step,
    at_bool_t is_first_step,
    at_bool_t is_final_step,
    at_bool_t has_global_stats,
    at_bool_t is_xtc_step,
    at_bool_t is_ns_step,
    t_commrec *cr);

void atgmx__scale_force(atgmx_t *atgmx,
    rvec f[], t_mdatoms *mdatoms);

double atgmx__get_beta(const atgmx_t *atgmx);

#endif
