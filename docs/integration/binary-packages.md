---
layout: default
title: Binary Packages
nav_order: 1
has_children: false
parent: Integration
has_toc: false
---
# Binary Packages

Get the binary package from the [release section](https://github.com/alandefreitas/matplotplusplus/releases). These binaries refer to the last release version of Matplot++. 

If you need a more recent version of Matplot++, you can download the [binary packages from the CI artifacts](https://github.com/alandefreitas/matplotplusplus/actions?query=workflow%3AMatplotplusplus+event%3Apush) or build the library [from the source files](). 

Once the package is installed, you can link your C++ program to the library and include the directories where you installed Matplot++. Unless you changed the default options, the library is likely to be in `/usr/local/` (Linux / Mac OS) or `C:/Program Files/` (Windows). The installer will try to find the directory where you usually keep your  libraries but that's not always perfect.

If you are using CMake, you can then find Matplot++ with the usual `find_package` command:

```cmake
find_package(Matplot++ REQUIRED)
# Binary Packages
target_link_libraries(my_target PUBLIC Matplot++::matplot)
```

CMake should be able to locate the `matplot++-config.cmake` script automatically if you installed the library under `/usr/local/` (Linux / Mac OS) or `C:/Program Files/` (Windows). Otherwise, you need to include your installation directory in `CMAKE_MODULE_PATH` first: 

```cmake
list(APPEND CMAKE_MODULE_PATH put/your/installation/directory/here)
find_package(Matplot++ REQUIRED)
# Binary Packages
target_link_libraries(my_target PUBLIC Matplot++::matplot)
```





<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
