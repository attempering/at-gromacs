# GROMACS Adapter for AT-Engine (C++ API)

## Applicability

Applicable to GROMACS 2016, 2021, 2022, 2023.

## Notes

### gmx_multisim_t

In versions 2016, 2018 of GROMACS,
the `gmx_multisim_t` handle `ms` is
a member of `t_commrec cr`,
and the macro `MULTISIM(cr)` is defined.

In version 2019 of GROMACS,
the `gmx_multisim_t` handle `ms` is
no longer a member of `t_commrec cr`,
but still declared in `commrec.h`.
The macro `MULTISIM(cr)` is gone.
An inline function `isMultiSim(ms)` is defined in `commrec.h`.

In versions 2020 of GROMACS,
the `gmx_multisim_t` handle `ms` is
declared in `gromacs/mdrunutility/multisim.h`.
The macro `MULTISIM(cr)` is gone.
An inline function `isMultiSim(ms)` is defined in there.
