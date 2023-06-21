#!/bin/bash

export gmxroot=../gmxroot

ln -f $gmxroot/src/gromacs/CMakeLists.txt src/gromacs/CMakeLists.txt

ln -f $gmxroot/api/legacy/include/gromacs/fileio/filetypes.h api/legacy/include/gromacs/fileio/filetypes.h

ln -f $gmxroot/src/gromacs/fileio/filetypes.cpp src/gromacs/fileio/filetypes.cpp

ln -f $gmxroot/src/gromacs/mdrun/legacymdrunoptions.h src/gromacs/mdrun/legacymdrunoptions.h

ln -f $gmxroot/src/gromacs/mdrun/md.cpp src/gromacs/mdrun/md.cpp

ln -f $gmxroot/src/gromacs/mdrun/runner.cpp src/gromacs/mdrun/runner.cpp

ln -f $gmxroot/src/gromacs/modularsimulator/modularsimulator.h src/gromacs/modularsimulator/modularsimulator.h

ln -f $gmxroot/src/gromacs/modularsimulator/modularsimulator.cpp src/gromacs/modularsimulator/modularsimulator.cpp
