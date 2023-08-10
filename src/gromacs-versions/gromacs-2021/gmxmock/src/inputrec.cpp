#include "gromacs/mdtypes/inputrec.h"
#include "gromacs/mdtypes/multipletimestepping.h"
#include "gromacs/mdtypes/pull_params.h"
#include "gromacs/utility/keyvaluetree.h"
#include "gromacs/utility/smalloc.h"


t_inputrec::t_inputrec()
{
  snew(fepvals, 1);
  snew(expandedvals, 1);
  snew(simtempvals, 1);
}

void done_inputrec(t_inputrec* ir)
{
  sfree(ir->fepvals);
  sfree(ir->expandedvals);
  sfree(ir->simtempvals);
  //delete ir->params;
}

t_inputrec::~t_inputrec()
{
  done_inputrec(this);
}
