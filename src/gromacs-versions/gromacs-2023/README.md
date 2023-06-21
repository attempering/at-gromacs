# Interface to GROMACS 2023

This directory contains files relevant to using at-engine in GROMACS.

## Directory structure

* [gmxroot](gmxroot) symbolic link to the external modified source code tree.
* [gmxmock](gmxmock) mock files to GROMACS files so that test programs can be compiled without the actual GROMACS code.
* [modified](modified) modified files, [list of modifications](modified/README.md)

## Building GROMACS 2023

Requires CMake 3.18, use `~/app/cmake-3.26/bin/cmake` instead of `cmake`

See [installing a custom version of CMake](../../../../../doc/installing-custom-cmake.md).

### Regular CPU version

```sh
mkdir build
cd build
~/app/cmake-3.26/bin/cmake .. -DGMX_BUILD_OWN_FFTW=ON
make -j 8
```

### Building GPU version

```sh
mkdir build_gpu
cd build_gpu
~/app/cmake-3.26/bin/cmake .. -DGMX_GPU=CUDA -DGMX_BUILD_OWN_FFTW=ON
make -j 8
```

### Building MPI version

First install OpenMPI or an alternative

```sh
mkdir build_mpi
cd build_mpi
cmake .. -DGMX_MPI=ON -DGMX_BUILD_OWN_FFTW=ON
make
```
