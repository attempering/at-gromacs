#ifndef GMX_LIB_MPI
#define GMX_LIB_MPI 0
#endif

#ifndef GMX_THREAD_MPI
#define GMX_THREAD_MPI 0
#endif

#ifndef GMX_MPI
#define GMX_MPI (GMX_LIB_MPI || GMX_THREAD_MPI)
#endif

#ifndef GMX_DOUBLE
#define GMX_DOUBLE 0
#endif

#ifndef GMX_GPU_CUDA
#define GMX_GPU_CUDA 0
#endif

#ifndef GMX_GPU_OPENCL
#define GMX_GPU_OPENCL 0
#endif

#ifndef GMX_GPU_SYCL
#define GMX_GPU_SYCL 0
#endif

#ifndef GMX_INTERNAL_XDR
#define GMX_INTERNAL_XDR 0
#endif

#ifndef TMPI_WAIT_FOR_NO_ONE
#define TMPI_WAIT_FOR_NO_ONE 0
#endif



#include "smalloc.cpp"
#include "network.cpp"
#include "multisim.cpp"
#include "enerdata_utils.cpp"
#include "inputrec.cpp"
#include "exceptions.cpp"
#include "seed.cpp"
#include "collect.cpp"
#include "gmxfio.cpp"
#include "update.cpp"
#include "gmx_omp_nthreads.cpp"

