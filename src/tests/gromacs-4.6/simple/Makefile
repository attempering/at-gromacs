CC_OPTIONS = -Wall -Wno-unused-function -Wno-stringop-truncation -DGMX_VERSION=40607
CC = gcc -O3 -g $(CC_OPTIONS)
CC_DBG = gcc -O0 -g -pg $(CC_OPTIONS)
CC_MPI = mpicc -O3 -g $(CC_OPTIONS) -DGMX_MPI -DGMX_LIB_MPI
CC_TMPI = gcc -O3 -g $(CC_OPTIONS) -DGMX_MPI -DGMX_THREAD_MPI -DTHREAD_PTHREADS -DHAVE_UNISTD_H -DHAVE_SCHED_H
LM = -lm

srcs = $(wildcard *.c)
bins = $(patsubst %.c, %.exe, $(srcs))
bins_dbg = $(patsubst %.c, %_d.exe, $(srcs))
bins_mpi = $(patsubst %.c, %_mpi.exe, $(srcs))
bins_tmpi = $(patsubst %.c, %_tmpi.exe, $(srcs))

dep_srcs =
dep_srcs_mpi = ../../../gromacs-versions/gromacs-4.6/gmxmock/src/gmxmock.c
dep_srcs_tmpi = $(dep_srcs_mpi) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/gmxroot/src/gmxlib/thread_mpi/*.c)

deps = $(dep_srcs) \
       $(wildcard *.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*/*/*/*/*.h) \
       $(wildcard ../../../gromacs-versions/gromacs-4.6/modified/src/kernel/at-gromacs/*/*/*/*/*/*/*.h) \
       Makefile
includes = -I../../../gromacs-versions/gromacs-4.6/modified/src/kernel \
           -I../../../gromacs-versions/gromacs-4.6/gmxmock/include \
           -I../../../gromacs-versions/gromacs-4.6/gmxroot/include \

bins: $(bins)

$(bins): %.exe : %.c $(deps)
	$(CC) $(includes) $< $(dep_srcs) -o $@ $(LM)

$(bins_dbg): %_d.exe : %.c $(deps)
	$(CC_DBG) $(includes) $< $(dep_srcs) -o $@ $(LM)

$(bins_mpi): %_mpi.exe : %.c $(deps)
	$(CC_MPI) $(includes) $< $(dep_srcs_mpi) -o $@ $(LM)

$(bins_tmpi): %_tmpi.exe : %.c $(deps)
	$(CC_TMPI) $(includes) $< $(dep_srcs_tmpi) -o $@ $(LM) -lpthread

clean::
	$(RM) -rf $(bins) $(bins_dbg) $(bins_mpi) $(bins_tmpi) .*.swp *~ hist*.bin at.manifest* HMB* MTSEED* mb*.av ZE* ze.dat *_hist*.dat TRACE* trace.dat gmon.out data/* atdata*
