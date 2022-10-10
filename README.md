# scic/fft

This is a parallel implementation of the Fast Fourier Transform (FFT) algorithm. The algorithm is implemented in C and provides multiple implementations using multiple backends. The backends are:

- OpenMP
- Pthreads

- [scic/fft](#scicfft)
  - [Using the Library](#using-the-library)

## Using the Library

> NOTE: This will install in /usr/local. You probably don't want that. But this is a quick start. The best thing to do is to combine this library with your other code into a larger CMake project/solution.

Install with [clib](https://github.com/clibs/clib):

```shell
clib install ScientificC/errno
```

or,

```shell
git clone https://github.com/ScientificC/errno.git
cd errno
./install.sh
```
