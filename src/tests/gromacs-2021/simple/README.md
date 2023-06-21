# Testing compiling case

## Regular version

```sh
make
valgrind --leak-check=full ./test.exe
```

## Library MPI version

```sh
make test_mpi.exe
mpirun -np 8 ./test_mpi.exe
```

If there is a warning message of `Invalid MIT-MAGIC-COOKIE-1 key`, try

```sh
export HWLOC_COMPONENTS="-gl"
```

## Thread-MPI version

```sh
make test_tmpi.exe
./test_simple_tmpi.exe -nt 4
```
