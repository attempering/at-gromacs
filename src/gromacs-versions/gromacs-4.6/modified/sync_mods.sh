#!/bin/bash

export gmxroot=../gmxroot

ln -f $gmxroot/include/types/filenm.h include/types/filenm.h

ln -f $gmxroot/src/gmxlib/filenm.c src/gmxlib/filenm.c

ln -f $gmxroot/src/kernel/md.c src/kernel/md.c

ln -f $gmxroot/src/kernel/mdrun.c src/kernel/mdrun.c
