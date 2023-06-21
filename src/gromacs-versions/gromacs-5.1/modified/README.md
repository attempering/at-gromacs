# Modified files to the GROMACS source code

Using the C API as `md.c` is a C file instead of a C++ one.
Although the rest of file structure is C++.

## List of main modifications

### Symbolically linking the at-gromacs directory under `src/gromacs`

```sh
cd src/gromacs
ln -sf ../../../../at-engine/src/interface/gromacs/versions/gromacs-5.1/modified/src/gromacs/at-gromacs
```

Make sure there is a `CMakeLists.txt` under the `at-gromacs` directory,
even if it is an empty file.

Editing the `CMakeLists.txt` under the `gromacs`.

Adding the following piece of code at the very end of the file:

```cmake
######## Beginning of modifications for at-gromacs #######

add_subdirectory(at-gromacs)

# force rebuild `at-gromacs__src.h` every time
add_custom_target(
  update_at_gromacs
  ALL
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/at-gromacs/at-gromacs__src.h
)

# add the target `update_at_gromacs` as a dependency of libgromacs
add_dependencies(libgromacs update_at_gromacs)

######## End of modifications for at-gromacs #######
```

### Adding a file type for `.cfg` files

1. Add an enum `efCFG` before `efNR` in [src/gromacs/fileio/filenm.h](src/gromacs/fileio/filenm.h).

    ```C
    efRND,
    efCFG, // add this
    efNR
    ```

2. Add the following line to [src/gromacs/fileio/filenm.c](src/gromacs/fileio/filenm.c)
   at the end of the definition of `t_deffile deffile`:

    ```C
    { eftASC, ".cfg", "at", NULL, "AT-Engine configuration file" }
    ```

    Remember to add a comma at the end of the previous line.
    The position of addition should match that in `filenm.h`.

3. Add the following line to the definition of variable `fnm` in [src/programs/mdrun/mdrun.cpp](src/programs/mdrun/mdrun.cpp)

    ```C
    { efCFG, "-at",  NULL, ffOPTRD },
    ```

    Remember to add a comma at the end of the previous line.

### at-gromacs code to `md.c`

[src/programs/mdrun/md.c](src/programs/mdrun/md.c)

Adding a `#include` statement

```C
#include "gromacs/at-gromacs/at-gromacs__src.h"
```

Modify the function `do_md()`

1. At the beginning of the function, before entering the MD loop, add

    ```C
    atgmx_t atgmx[1];
    ```

    And probably on line 482, after `set_constraints()`, but before `init_replica_exchange()`, add

    ```C
    atgmx__init(atgmx,
        atgmx__opt2fn("-at", nfile, fnm),
        ir, cr,
        Flags & MD_STARTFROMCPT,
        AT__INIT_VERBOSE);
    ```

2. Within the MD loop, right before the line (about line 1037) of assignment `bGStat = ...`, add

    ```C
        if (atgmx__do_tempering_on_step(atgmx, step, bNS)) {
            bCalcEner = TRUE;
            bCalcVir = TRUE;
        }
    ```

3. Still in the MD loop, after the call of `do_force()`, add

    ```C
    atgmx__scale_force(atgmx, f, mdatoms);
    ```

    Roughly on line 1112.

4. On line 1353, after

    ```C++
    /* ########  END FIRST UPDATE STEP  ############## */
    /* ########  If doing VV, we now have v(dt) ###### */
    if (bDoExpanded)
    {
        ...
    }
    ```

    but before `do_md_trajectory_writing()`,
    add

    ```C
        atgmx__move(atgmx, enerd, step, bFirstStep, bLastStep,
            bGStat, do_per_step(step, ir->nstxout_compressed), bNS, cr);
    ```

5. At the end of function

    ```C
    atgmx__finish(atgmx);
    ```

## Script to sync modified files

```sh
./sync_mods.sh
```

Modified files are hard linked.
