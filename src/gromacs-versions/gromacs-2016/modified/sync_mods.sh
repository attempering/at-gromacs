#!/bin/bash

export gmxroot=../gmxroot

ln -f $gmxroot/src/gromacs/CMakeLists.txt src/gromacs/CMakeLists.txt

ln -f $gmxroot/src/gromacs/fileio/filetypes.h src/gromacs/fileio/filetypes.h

ln -f $gmxroot/src/gromacs/fileio/filetypes.cpp src/gromacs/fileio/filetypes.cpp

ln -f $gmxroot/src/programs/mdrun/md.cpp src/programs/mdrun/md.cpp

ln -f $gmxroot/src/programs/mdrun/mdrun.cpp src/programs/mdrun/mdrun.cpp
