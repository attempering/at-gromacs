# GROMACS Adapter for AT-Engine (C API)

## Applicability

Applicable to GROMACS 4.6, 5.0, 5.1.

## Notes

### gmx_multisim_t

In all versions applicable to the C API,
the `gmx_multisim_t` handle (common variable name: `ms`) is
a member of the struct `t_commrec` (common variable name: `cr`).

The macro `MULTISIM(cr)` for testing whether the multiple-simulation option is turned on in the runtime accepts a handle of the struct `t_commrec` not that of `gmx_multisim_t`.
