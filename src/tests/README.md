# Testing for GROMACS adapters

Testing for GROMACS adapters are done using a mock-patching approach.

The testing programs are independent ANSI C/C++ programs
that compile and run quickly.
Although these programs call adapter functions that use GROMACS functions,
the ``GROMACS'' actually used are mocked or redefined.

For example, the C version of the adapter uses the function `gmx_bcast()`,
which is a classic function defined in GROMACS 4.6.
But in the testing program, this function is redefined in
$(ROOT)/src/gromacs-versions/gromacs-4.6/gmxmock/src/gmxmock.c
or files included there. And by a carefully written Makefile,
the compiler will link the testing programs to the `gmxmock.c` mentioned above.

This allows us to avoid linking the GROMACS library every time
we compile a test program.

The mocked versions are GROMACS-version dependent, so for each version of GROMACS,
we have a file like

$(ROOT)/src/gromacs-versions/gromacs-{VERSION}/gmxmock/src/gmxmock.c or .cpp

The mocked functions usually have empty bodies, or contain only simplest implementations.

Unlike the GROMACS functions, we usually don't redefine GROMACS structures.
Instead, we simply link the original GROMACS headers that contain these structures.
But the functions to initialize the structures are redefined
in simplest possible ways.
