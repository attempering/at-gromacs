# Modified files to the GROMACS source code

## List of main modifications

### Symbolically linking the at-gromacs directory under `src/gromacs`

```sh
cd src/gromacs
ln -sf ../../../../at-engine/src/interface/gromacs/versions/gromacs-2016/modified/src/gromacs/at-gromacs
```

Make sure there is a [CMakeLists.txt](src/gromacs/CMakeLists.txt) under the `at-gromacs` directory,
even if it is an empty file.

Editing the `CMakeLists.txt` under the `gromacs`.

Adding the following piece of code:

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

A possible position is the very end.

### Adding a file type for `.cfg` files

1. Add an enum `efCFG` before `efNR` in [src/gromacs/fileio/filetypes.h](src/gromacs/fileio/filetypes.h).

    ```C
    efCSV,
    efCFG, // add this
    efNR
    ```

2. Add the following line to [src/gromacs/fileio/filetypes.cpp](src/gromacs/fileio/filetypes.cpp)
   at the end of the definition of `t_deffile deffile`:

    ```C
    { eftASC, ".cfg", "at", NULL, "AT-Engine configuration file" }
    ```

    Remember to add a comma at the end of the previous line.
    The position of addition should match that in `filetypes.h`.

3. Add the following line to the definition of variable `fnm` in [src/programs/mdrun/mdrun.cpp](src/programs/mdrun/mdrun.cpp)

    ```C
    { efCFG, "-at",  NULL, ffOPTRD },
    ```

    Remember to add a comma at the end of the previous line.

### at-gromacs code to `md.cpp`

[src/programs/mdrun/md.cpp](src/programs/mdrun/md.cpp)

Adding a `#include` statement

```C++
#include "gromacs/at-gromacs/at-gromacs__src.h"
```

Modify the function `do_md()`

1. At the beginning of the function, before entering the MD loop, add

    ```C++
    auto atGmx = AtGmx(
        opt2fn_null("-at", nfile, fnm),
        ir, cr,
        Flags & MD_STARTFROMCPT,
        (MULTISIM(cr) != NULL),
        AT__INIT_VERBOSE);
    ```

    A proper location of addition is probably on line 503, after `set_constraints()`, but before `init_replica_exchange()`

2. Within the MD loop, right before the line (about line 1076) of assignment `bGStat = ...`, add

    ```C++
    if (atGmx.do_tempering_on_step(step, bNS)) {
        bCalcEner = TRUE;
        bCalcVir = TRUE;
    }
    ```

3. Still in the MD loop, roughly on line 1120, after the call of `do_force()` and the call of `if (EI_VV(ir->eI) && !startingFromCheckpoint && !bRerunMD)`, add

    ```C++
    atGmx.scale_force(f, mdatoms);
    ```

4. Before the comment of `START TRAJECTORY OUTPUT`

    ```C++
    atGmx.move(enerd, static_cast<at_llong_t>(step), bFirstStep, bLastStep,
        bGStat, do_per_step(step, ir->nstxout_compressed), bNS, cr);
    ```

    on line 1276, after

    ```C++
    /* ########  END FIRST UPDATE STEP  ############## */
    /* ########  If doing VV, we now have v(dt) ###### */
    if (bDoExpanded) {
        ...
    }
    ```

## Script to sync modified files

```sh
./sync_mods.sh
```

Modified files are hard linked.
