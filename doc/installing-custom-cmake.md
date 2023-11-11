# Installing a custom version of CMake

## Introduction

GROMACS 2023 requires a CMake version of at least 3.18,
but Ubuntu 20.04 only has 3.16.
To compile the latest GROMACS, you need to install a newer version
of CMake, here is how.

## Tutorials

[Installing from source (Chinese)](https://zhuanlan.zhihu.com/p/519732843)

[Installing the pre-built binary](https://askubuntu.com/questions/829310/how-to-upgrade-cmake-in-ubuntu)

## Instructions for installing the pre-built binary

1. Download the latest version of CMake (3.26.4 at the time being)

    [Files](https://cmake.org/files/)

    [Download page](https://cmake.org/download/)

    ```sh
    wget https://cmake.org/files/v3.26/cmake-3.26.4-linux-x86_64.tar.gz
    ```

2. Decompress the package

    ```sh
    tar -zxvf cmake-3.26.4.tar.gz
    cd cmake-3.26.4
    ```

3. Move the directory to a custom location:

    ```sh
    mv cmake-3.26.4-linux-x86_64 $HOME/app/cmake-3.26
    ```

