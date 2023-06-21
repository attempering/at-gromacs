CXX_OPTIONS = --std=c++17 -fopenmp -Wall -Wno-unused-function -Wno-stringop-truncation -DGMX_VERSION=20210007
CXX = g++ -O3 -g $(CXX_OPTIONS)
CXX_DBG = g++ -O0 -g -pg $(CXX_OPTIONS)
CXX_MPI = mpicxx -O3 -g $(CXX_OPTIONS) -DGMX_MPI -DGMX_LIB_MPI
CXX_TMPI = g++ -O3 -g $(CXX_OPTIONS) -DGMX_MPI -DGMX_THREAD_MPI -DTHREAD_PTHREADS -DHAVE_UNISTD_H -DHAVE_SCHED_H
LM = -lm

srcs = $(wildcard *.cpp)
bins = $(patsubst %.cpp, %.exe, $(srcs))
bins_dbg = $(patsubst %.cpp, %_d.exe, $(srcs))
bins_mpi = $(patsubst %.cpp, %_mpi.exe, $(srcs))
bins_tmpi = $(patsubst %.cpp, %_tmpi.exe, $(srcs))

dep_srcs = ../../../gromacs-versions/gromacs-2021/gmxmock/src/gmxmock.cpp
dep_srcs_mpi = $(dep_srcs)
dep_srcs_tmpi = $(dep_srcs_mpi) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/gmxroot/src/external/thread_mpi/src/*.cpp)

deps = $(dep_srcs) \
       $(wildcard *.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-2021/modified/src/kernel/at-gromacs/*/*/*/*/*/*/*.h) \
       Makefile
includes = -I../../../gromacs-versions/gromacs-2021/gmxmock/include \
           -I../../../gromacs-versions/gromacs-2021/modified/src/gromacs \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/gromacs \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/gromacs/gmxlib \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/gromacs/mdlib \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/gromacs/utility \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/gromacs/math \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/external \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/src/external/thread_mpi/include \
           -I../../../gromacs-versions/gromacs-2021/gmxroot/include

bins: $(bins)

$(bins): %.exe : %.cpp $(deps)
	$(CXX) $(includes) $< $(dep_srcs) -o $@ $(LM)

$(bins_dbg): %_d.exe : %.cpp $(deps)
	$(CXX_DBG) $(includes) $< $(dep_srcs) -o $@ $(LM)

$(bins_mpi): %_mpi.exe : %.cpp $(deps)
	$(CXX_MPI) $(includes) $< $(dep_srcs_mpi) -o $@ $(LM)

$(bins_tmpi): %_tmpi.exe : %.cpp $(deps)
	$(CXX_TMPI) $(includes) $< $(dep_srcs_tmpi) -o $@ $(LM) -lpthread

clean::
	$(RM) -rf $(bins) $(bins_dbg) $(bins_mpi) $(bins_tmpi) .*.swp *~ hist*.bin at.manifest* HMB* MTSEED* mb*.av ZE* ze.dat *_hist*.dat TRACE* trace.dat gmon.out data/* atdata*
