# GROMACS notes

## Macros for the GROMACS version

In GROMACS 4.6, the `GMX_VERSION` is managed in `cmake` directory

`GMX_VERSION` is integral macro whose numerical value is something like `40607`.
The formula is `major*10000 + minor*100 + patch`.

GROMACS 2021 also has `GMX_VERSION_MAJOR`, `GMX_VERSION_MINOR`, `GMX_VERSION_PATCH`
defined as integers.

```cmake
set(GMX_VERSION_MAJOR 5)
set(GMX_VERSION_MINOR 1)
set(GMX_VERSION_PATCH 6)

set(GMX_VERSION "${GMX_VERSION_MAJOR}.${GMX_VERSION_MINOR}.${GMX_VERSION_PATCH}")
```
