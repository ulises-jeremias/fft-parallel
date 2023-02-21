# Fast Fourier Transform (FFT) algorithm

This is a parallel implementation of the Fast Fourier Transform (FFT) algorithm. The algorithm is implemented in C and provides multiple implementations using multiple backends. The backends are:

- OpenMP
- Pthreads

## Table of Contents

- [Fast Fourier Transform (FFT) algorithm](#fast-fourier-transform-fft-algorithm)
  - [Table of Contents](#table-of-contents)
  - [Using the library](#using-the-library)
    - [Install Dependencies](#install-dependencies)
  - [Running Tests](#running-tests)
  - [Build Options](#build-options)

## Using the library

> NOTE: This will install in /usr/local. You probably don't want that. But this is a quick start. The best thing to do is to combine this library with your other code into a larger CMake project/solution.

Install the last release with [clib](https://github.com/clibs/clib):

```shell
clib install ulises-jeremias/fft-parallel
```

or,

install from source code: First install the library [ScientificC/errno](https://github.com/ScientificC/errno). [_How to install the dependencies_](#install-dependencies)?

Then, execute the following commands on the cli:

```shell
git clone https://github.com/ulises-jeremias/fft-parallel.git
cd fft-parallel
sudo ./install
```

or you install it using different [_Build Options_](#build-options) with the following commands:

```sh
git clone https://github.com/ulises-jeremias/fft-parallel.git
cd fft-parallel
mkdir build
cd build
cmake .. <build options>
make && sudo make install
```

### Install Dependencies

To install `ScientificC/errno` you can run the following commands on the cli:

```sh
cd /tmp

# to install scic/errno

git clone https://github.com/ScientificC/errno.git
cd errno
mkdir build && cd build
cmake ..
make && sudo make install
```

## Running Tests

To run the test suite from a command prompt, using a Makefile-like generator, execute the following:

```shell
git clone https://github.com/ulises-jeremias/fft-parallel.git
cd fft-parallel
mkdir build
cd build
cmake .. <build options>
make && ctest
```

[_Build Options_](#build-options)

## Build Options

- FFT_BUILD_SHARED: (Default ON) Controls if the shared library is built

```shell
cmake .. -DFFT_BUILD_SHARED=ON
cmake .. -DFFT_BUILD_SHARED=OFF
```

- FFT_BUILD_STATIC: (Default ON) Controls if the static library is built

```shell
cmake .. -DFFT_BUILD_STATIC=ON
cmake .. -DFFT_BUILD_STATIC=OFF
```

- FFT_BUILD_TESTS: (Default ON) Build the unit tests

```shell
cmake .. -DFFT_BUILD_TESTS=ON
cmake .. -DFFT_BUILD_TESTS=OFF
```

- CMAKE_BUILD_TYPE: (Default Release) Set this to 'Release' or 'Debug'

```shell
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

- CMAKE_INSTALL_PREFIX: (Default /usr/local) Allows you to specify where `make install` sends the output.

```shell
cmake .. -DCMAKE_INSTALL_PREFIX=~/fft/
cmake .. -DCMAKE_INSTALL_PREFIX=~/Projects/myproject/
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/
```
