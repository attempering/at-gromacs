#include "gromacs/mdtypes/enerdata.h"

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
