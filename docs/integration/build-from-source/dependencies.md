---
layout: default
title: Dependencies
nav_order: 1
has_children: false
parent: Build from Source
grand_parent: Integration
has_toc: false
---
# Dependencies

This section lists the dependencies you need before installing Matplot++ from source. The build script will try to find all these dependencies for you:

* C++17
* CMake 3.14+
* Gnuplot 5.2.6+ (Required at runtime)


Instructions: Linux/Ubuntu/GCC

Check your GCC version:

```bash
g++ --version
```

The output should be something like:

```console
g++-8 (Ubuntu 8.4.0-1ubuntu1~18.04) 8.4.0
```

If you see a version before GCC-8, update it with

```bash
sudo apt update
sudo apt install gcc-8
sudo apt install g++-8
```

To update to any other version, like GCC-9 or GCC-10:

```bash
sudo apt install build-essential
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt install g++-10
```

Once you installed a newer version of GCC, you can link it to `update-alternatives`. For instance, if you have GCC-7 and GCC-10, you can link them with:

```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 7
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
```

You can now use `update-alternatives` to set your default `gcc` and `g++` to a more recent version:

```bash
update-alternatives --config g++
update-alternatives --config gcc
```

Also check your CMake version:

```bash
cmake --version
```

If it's older than CMake 3.14, update it with

```bash
sudo apt upgrade cmake
```

or download the most recent version from [cmake.org](https://cmake.org/).

[Later](build-the-examples.md) when running CMake, make sure you are using GCC-8 or higher by appending the following options:

```bash
-DCMAKE_C_COMPILER=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER=/usr/bin/g++-8
```

Install Gnuplot 5.2.6+

```bash
sudo apt update
sudo apt install gnuplot
```

or download the latest version from [www.gnuplot.info](www.gnuplot.info). If you're using an installer, make sure you mark the option "Add application directory to your PATH environment variable".




Instructions: Mac Os/Clang

Check your Clang version:

```bash
clang --version
```

The output should have something like

```console
Apple clang version 11.0.0 (clang-1100.0.33.8)
```

If you see a version before Clang 11, update XCode in the App Store or update clang with homebrew. 

Check your CMake version:

```bash
cmake --version
```

If it's older than CMake 3.14, update it with

```bash
sudo brew upgrade cmake
```

or download the most recent version from [cmake.org](https://cmake.org/).

If the last command fails because you don't have [Homebrew](https://brew.sh) on your computer, you can install it with

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

or you can follow the instructions in [https://brew.sh](https://brew.sh).

Install Gnuplot 5.2.6+

```bash
sudo brew install gnuplot
```

or download the latest version from [www.gnuplot.info](www.gnuplot.info). If you're using an installer, make sure you mark the option "Add application directory to your PATH environment variable".




Instructions: Windows/MSVC

* Make sure you have a recent version of [Visual Studio](https://visualstudio.microsoft.com)
* Download Git from [https://git-scm.com/download/win](https://git-scm.com/download/win) and install it
* Download CMake from [https://cmake.org/download/](https://cmake.org/download/) and install it
* Download Gnuplot from [www.gnuplot.info](www.gnuplot.info) and install it (if Matplot++ examples don't display with no console errors and gnuplot running, try to re-install with wxt terminal).

If you're using the Gnuplot installer, make sure you mark the option "Add application directory to your PATH environment variable".



The build script will also look for these *optional* dependencies for manipulating images:

* JPEG
* TIFF
* ZLIB
* PNG
* LAPACK
* BLAS
* FFTW
* OpenCV
    
There are two dependencies in [`source/3rd_party`](https://github.com/alandefreitas/matplotplusplus/blob/master/source/3rd_party). These dependencies are bundled, so you don't have to worry about them:
 
* olvb/nodesoup
* dtschump/CImg

You can define `WITH_SYSTEM_NODESOUP=ON` or `WITH_SYSTEM_CIMG=ON` in the cmake command line to use a system-provided version of these dependencies.

There's an extra target `matplot_opengl` with the experimental [OpenGL backend](../backends.md). You need to define `BUILD_EXPERIMENTAL_OPENGL_BACKEND=ON` in the CMake command line to build that target. In that case, the build script will also look for these extra dependencies:

* OpenGL
* GLAD
* GLFW3


Instructions: Linux/Ubuntu/GCC

```bash
sudo apt-get install libglfw3-dev
```




Instructions: Mac Os/Clang

Download GLFW3 from https://www.glfw.org




Instructions: Windows/MSVC

Download GLFW3 from https://www.glfw.org



You can see all dependencies in [`source/3rd_party/CMakeLists.txt`](https://github.com/alandefreitas/matplotplusplus/blob/master/source/3rd_party/CMakeLists.txt).




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
