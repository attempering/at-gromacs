# Kernel code for AT-Engine

## Overview

The adaptive tempering algorithm is a single-copy simulated tempering
algorithm that is based on a continuous temperature range.
The algorithm adopts a few self-learning techniques
to adaptive the parameters related to statistical weights
(the partition function).

The current code is written in ANSI C,
its and is compatible with GROMACS 4.6.

## Getting started

A typical simulation goes like this:

```C
at_t* at = at__open("at.cfg", NULL, AT__INIT_VERBOSE);

for (step = 1; step <= nsteps; step++) {
  at__step(at, energy, step, NULL);
}

at__close(at);
```

Simulation parameters are provided in the configuration file `at.cfg`.

## Options

A brief manual on the configuration file `at.cfg`
is included under the [doc](doc) directory.

## New features

In comparision with the 2010 paper [1], the current code brings about
a few key improvements on the overall accuracy and performance.

* Metropolisation.  The Langevin equation used in the original
algorithm is approximate, and may be inaccurate for an overly
large step size. This limitation can be removed with the
Metropolisation technique, which gives an acceptance probability
for the destination temperature suggested by the Langevin equation.

* Moderation on the temperature diffusion.  Initial stages of the
simulation often pose stability problems because of lack of data to
guide an efficient diffusion in the temperature space.  A few
parameters `langevin_no_skip`, `langevin_bin_min_visits`
are now added to prevent unreasonably large temperature transitions
in early stages.

* Zero-filler. The integral identity draws estimates from
a window of bins. Sometimes the bin range contains missing data
that needs to be filled in order for the identity to work proper.
The zero-filling mechanism is now added.

## References

[1] C. Zhang, J. Ma, J. Chem. Phys. 132, 244101 (2010) [https://doi.org/10.1063/1.3435332](https://doi.org/10.1063/1.3435332)
