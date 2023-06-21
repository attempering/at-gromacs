# AT-Engine GROMACS C API

## Applicability

Applicable to GROMACS 4.6, 5.0, 5.1.

## Notes

### gmx_multisim_t

In all versions applicable to the C API,
the gmx_multisim_t handle `ms` is
a member of `t_commrec cr`,
and the macro `MULTISIM(cr)` is defined.
