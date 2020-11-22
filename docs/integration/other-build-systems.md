---
layout: default
title: Other build systems
nav_order: 6
has_children: false
parent: Integration
has_toc: false
---
# Other build systems

If you want to use it in another build system you can either install the library (Section [*Binary Packages*](binary-packages.md) or Section [Installing Matplot++ from Source](build-from-source/installing-matplot-from-source.md) or you have to somehow rewrite the build script.

If you want to rewrite the build script, your project needs to 1) include the headers and compile all source files in the [`source`](https://github.com/alandefreitas/matplotplusplus/blob/master/source) directory, and 2) link with the dependencies described in [`source/3rd_party/CMakeLists.txt`](https://github.com/alandefreitas/matplotplusplus/blob/master/source/3rd_party/CMakeLists.txt).

Then add this header to your source files:

```cpp
#include <matplot/matplot.h>
```   




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
