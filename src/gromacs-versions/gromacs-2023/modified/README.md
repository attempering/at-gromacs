# Modified files to the GROMACS source code

## List of main modifications

### Symbolically linking the at-gromacs directory under `src/gromacs`

```sh
cd src/gromacs
ln -sf ../../../../at-engine/src/interface/gromacs/versions/gromacs-2023/modified/src/gromacs/at-gromacs
```

Make sure there is a `CMakeLists.txt` under the `at-gromacs` directory,
even if it is an empty file.

Editing the [CMakeLists.txt](src/gromacs/CMakeLists.txt) under the `gromacs`.

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

A possible position is after on line 417:

```cmake
target_link_libraries(libgromacs PRIVATE lmfit)
```

### Adding a file type for `.cfg` files

1. Add an enum `efCFG` before `efNR` in [api/legacy/include/gromacs/fileio/filetypes.h](api/legacy/include/gromacs/fileio/filetypes.h).

    ```C
    efCSV,
    efQMI,
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

3. Add the following line to the definition of variable `fnm` in [src/gromacs/mdrun/legacymdrunoptions.h](src/gromacs/mdrun/legacymdrunoptions.h) (on about line 127)

    ```C
    { efCFG, "-at",  NULL, ffOPTRD },
    ```

    Remember to add a comma at the end of the previous line if necessary.

### at-gromacs code to `src/gromacs/mdrun/md.cpp`

[src/gromacs/mdrun/md.cpp](src/gromacs/mdrun/md.cpp)

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
        startingBehavior != StartingBehavior::NewSimulation,
        isMultiSim(ms),
        AT__INIT_VERBOSE);
    ```

    A proper location of addition is probably on line 527, after `prepareAwhModule()`, but before `init_replica_exchange()`

2. Within the MD loop, right before the line (about line 1104) of assignment `bGStat = ...`, add

    ```C++
    if (atGmx.do_tempering_on_step(step, bNS)) {
        bCalcEner = TRUE;
        bCalcVir = TRUE;
    }
    ```

3. Still in the MD loop, after the call of `do_force()` and the call of `GMX_BARRIER(cr->mpi_comm_mygroup)`, add

    ```C++
    atGmx.scale_force(f.view(), mdAtoms->mdatoms());
    ```

    Roughly on line 1262.

4. Inserting the code for `atGmx.move()`

    ```C++
    atGmx.move(enerd, static_cast<at_llong_t>(step), bFirstStep, bLastStep,
        bGStat, do_per_step(step, ir->nstxout_compressed), bNS, cr);
    ```

    on line 1322, after the block

    ```C++
    /* ########  END FIRST UPDATE STEP  ############## */
    /* ########  If doing VV, we now have v(dt) ###### */
    if (bDoExpanded) {
        ...
    }
    ```

### Disabling the modular simulator

By default, GROMACS uses the modular simulator,
but for `atgmx` we want to use the legacy simulator instead.

1. In [src/gromacs/modularsimulator/modularsimulator.h](src/gromacs/modularsimulator/modularsimulator.h),

    In the declaration of the function `isInputCompatible()`, on roughly line 88,
    before the line for `bool doEssentialDynamics,`
    add a parameter

    ```C++
    bool doAt,
    ```

2. In [src/gromacs/modularsimulator/modularsimulator.cpp](src/gromacs/modularsimulator/modularsimulator.cpp),

    In the definition of function `isInputCompatible()`:
    On roughly line 352, before the line `bool doEssentialDynamics,`
    add the parameter for `doAt`,

    ```C++
    bool doAt,
    ```

    Then on roughly line 471, before the line `doEssentialDynamics`,
    add the following piece of code,

    ```C++
    isInputCompatible =
            isInputCompatible
            && conditionalAssert(!doAt,
                                 "Adaptive tempering is not supported by the modular simulator.");
    ```

    In the function of `void ModularSimulator::checkInputForDisabledFunctionality()`,
    on roughly line 538, right before the line for
    `opt2bSet("-ei", legacySimulatorData_->nfile, legacySimulatorData_->fnm),`
    add a line
    `opt2bSet("-at", legacySimulatorData_->nfile, legacySimulatorData_->fnm),`

3. In [src/gromacs/mdrun/runner.cpp](src/gromacs/mdrun/runner.cpp)

    In `mdrunner()`, in the line 1055 before
    `const bool useModularSimulator = checkUseModularSimulator(...)`,
    add the following line:

    ```C++
    const bool doAt = opt2bSet("-at", filenames.size(), filenames.data());
    ```

    and then add the argument `doAt` to the function call to `checkUseModularSimulator`

    ```C++
    const bool useModularSimulator = checkUseModularSimulator(false,
                                                              inputrec.get(),
                                                              doRerun,
                                                              mtop,
                                                              ms,
                                                              replExParams,
                                                              nullptr,
                                                              doAt,
                                                              doEssentialDynamics,
                                                              membedHolder.doMembed(),
                                                              updateTarget == TaskTarget::Gpu);
    ```

## Script to sync modified files

```sh
./sync_mods.sh
```

Modified files are hard linked.
