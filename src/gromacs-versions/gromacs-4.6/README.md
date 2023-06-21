# Interface to GROMACS 4.6

This directory contains files relevant to using at-engine in GROMACS.

## Directory structure

* [gmxroot](gmxroot) symbolic link to the external modified source code tree.
* [gmxmock](gmxmock) mock files to GROMACS files so that test programs can be compiled without the actual GROMACS code.
* [modified](modified) modified files, [list of modifications](modified/README.md).

## Building GROMACS

### Building a regular version

```sh
mkdir build
cd build
cmake .. -DGMX_GPU=OFF -DGMX_CPU_ACCELERATION=SSE4.1 -DGMX_BUILD_OWN_FFTW=ON
make
```

* The official release no longer works with current Nvidia GPUs (as of 2022-11-26).
  So we disable it with `-DGMX_GPU=OFF`

* On R9000P, the automatically detected CPU acceleration AVX_128_FMA appears to faulty.
  That is why we use `-DGMX_CPU_ACCELERATION=SSE4.1`.

### Building an MPI version

First install OpenMPI or an alternative

Then, without GPU support

```sh
mkdir build_mpi
cd build_mpi
cmake .. -DGMX_MPI=ON -DGMX_GPU=OFF -DGMX_CPU_ACCELERATION=SSE4.1 -DGMX_BUILD_OWN_FFTW=ON
make
```
