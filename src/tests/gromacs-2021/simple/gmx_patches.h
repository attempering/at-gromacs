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

#include "at-gromacs/atgmx/context/atgmx_context.h"

t_inputrec::t_inputrec() {
  opts.ref_t = nullptr;
}

t_inputrec::~t_inputrec() {
  free(opts.ref_t);
}

ForeignLambdaTerms::ForeignLambdaTerms(int numLambdas) :
    numLambdas_(numLambdas),
    energies_(1 + numLambdas),
    dhdl_(1 + numLambdas)
{
}

gmx_enerdata_t::gmx_enerdata_t(int numEnergyGroups, int numFepLambdas) :
    grpp(numEnergyGroups),
    foreignLambdaTerms(numFepLambdas),
    foreign_grpp(numEnergyGroups)
{
}
