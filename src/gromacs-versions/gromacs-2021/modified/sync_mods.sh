#!/bin/bash

export gmxroot=../gmxroot

mkdir -p src/gromacs

ln -f $gmxroot/src/gromacs/CMakeLists.txt src/gromacs/CMakeLists.txt

mkdir -p src/gromacs/fileio

ln -f $gmxroot/src/gromacs/fileio/filetypes.h src/gromacs/fileio/filetypes.h

ln -f $gmxroot/src/gromacs/fileio/filetypes.cpp src/gromacs/fileio/filetypes.cpp

mkdir -p src/gromacs/mdrun

ln -f $gmxroot/src/gromacs/mdrun/legacymdrunoptions.h src/gromacs/mdrun/legacymdrunoptions.h

ln -f $gmxroot/src/gromacs/mdrun/md.cpp src/gromacs/mdrun/md.cpp

ln -f $gmxroot/src/gromacs/mdrun/runner.cpp src/gromacs/mdrun/runner.cpp

mkdir -p src/gromacs/modularsimulator

ln -f $gmxroot/src/gromacs/modularsimulator/modularsimulator.h src/gromacs/modularsimulator/modularsimulator.h

ln -f $gmxroot/src/gromacs/modularsimulator/modularsimulator.cpp src/gromacs/modularsimulator/modularsimulator.cpp

