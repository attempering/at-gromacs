#!/bin/bash

export gmxroot=../gmxroot

ln -f $gmxroot/src/gromacs/CMakeLists.txt src/gromacs/CMakeLists.txt

ln -f $gmxroot/src/gromacs/fileio/filenm.h src/gromacs/fileio/filenm.h

ln -f $gmxroot/src/gromacs/fileio/filenm.c src/gromacs/fileio/filenm.c

ln -f $gmxroot/src/programs/mdrun/mdrun.cpp src/programs/mdrun/mdrun.cpp

ln -f $gmxroot/src/programs/mdrun/md.c src/programs/mdrun/md.c

ln -f $gmxroot/src/programs/mdrun/runner.c src/programs/mdrun/runner.c
