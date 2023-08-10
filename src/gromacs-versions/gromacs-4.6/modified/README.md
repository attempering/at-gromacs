# Modified files to the GROMACS source code

## List of main modifications

### Adding a file type for `.cfg` files

1. Add an enum `efCFG` before `efNR` in [include/types/filenm.h](include/types/filenm.h).

2. Add the following line to [src/gmxlib/filenm.c](src/gmxlib/filenm.c)
   at the end of the definition of `t_deffile`:

    ```C
    { eftASC, ".cfg", "at", NULL, "AT-Engine configuration file" }
    ```

    Remember to add a comma at the end of the previous line.

3. Add the following line to the definition of variable `fnm` in [src/kernel/mdrun.c](src/kernel/mdrun.c)

    ```C
    { efCFG, "-at",  NULL, ffOPTRD },
    ```

    Remember to add a comma at the end of the previous line.

### atgmx code to src/kernel/md.c

Modify the function `do_md()` in [src/kernel/md.c](src/kernel/md.c)

1. At the beginning of the function, before entering the MD loop, add

    ```C
    atgmx_t atgmx[1];

    atgmx__init(atgmx,
        atgmx__opt2fn_null("-at", nfile, fnm),
        ir, cr, opt2bSet("-multidir", nfile, fnm),
        Flags & MD_STARTFROMCPT,
        AT__INIT_VERBOSE);
    ```

2. Within the MD loop, right before the line (about line 1154) of assignment `bGStat = ...`, add

    ```C
        if (atgmx__do_tempering_on_step(atgmx, step, bNS)) {
            bCalcEner = TRUE;
            bCalcVir = TRUE;
        }
    ```

3. Still in the MD loop, after the call of `do_force()` and the call of `GMX_BARRIER(cr->mpi_comm_mygroup)`, add

    ```C
    atgmx__scale_force(atgmx, f, mdatoms);
    ```

4. Before the comment of `START TRAJECTORY OUTPUT`

    ```C
        atgmx__move(atgmx, enerd, step, bFirstStep, bLastStep,
            bGStat, do_per_step(step, ir->nstxtcout), bNS, cr);
    ```

5. At the end of function

    ```C
    atgmx__finish(atgmx);
    ```

#### Corrections to `md.c`

1. Rewrite the block, around line 1244,

    ```C
    if (ir->eI == eiVV && bInitStep)
    {
        /* if using velocity verlet with full time step Ekin,
            * take the first half step only to compute the
            * virial for the first step. From there,
            * revert back to the initial coordinates
            * so that the input is actually the initial step.
            */
        copy_rvecn(state->v, cbuf, 0, state->natoms); /* should make this better for parallelizing? */
    }
    ```

    as

    ```C
    rvec *vbuf = NULL;

    if (ir->eI == eiVV && bInitStep)
    {
        /* if using velocity verlet with full time step Ekin,
            * take the first half step only to compute the
            * virial for the first step. From there,
            * revert back to the initial coordinates
            * so that the input is actually the initial step.
            */
        snew(vbuf, state->natoms);
        copy_rvecn(state->v, vbuf, 0, state->natoms); /* should make this better for parallelizing? */
    }
    ```

2. Rewrite the following block, around line 1433,

    ```C
    if (bInitStep && ir->eI == eiVV)
    {
        copy_rvecn(cbuf, state->v, 0, state->natoms);
    }
    ```

    as

    ```C
    if (bInitStep && ir->eI == eiVV)
    {
        copy_rvecn(vbuf, state->v, 0, state->natoms);
        sfree(vbuf);
    }
    ```

## Script to sync modified files

```sh
./sync_mods.sh
```

Modified files are hard linked.
