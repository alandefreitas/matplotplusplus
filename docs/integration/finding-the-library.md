---
layout: default
title: Finding the Library
nav_order: 2
has_children: false
parent: Integration
has_toc: false
---
# Finding the Library

Once the package is installed, you can link your C++ program to the library and include the directories where you installed Matplot++. Unless you changed the default options, the library is likely to be in `/usr/local/` (Linux / Mac OS) or `C:/Program Files/` (Windows). The installer will try to find the directory where you usually keep your libraries but that's not always perfect.

If you are using CMake, you can then find Matplot++ with the usual `find_package` command:

```cmake
find_package(Matplot++ REQUIRED)
# ...
target_link_libraries(my_target PUBLIC Matplot++::matplot)
```

You can see a complete example in [`test/integration/CMakeLists.txt`](https://github.com/alandefreitas/matplotplusplus/blob/master/test/integration/CMakeLists.txt). CMake should be able to locate the `matplot++-config.cmake` script automatically if you installed the library under `/usr/local/` (Linux / Mac OS).

Unfortunately, CMake does not have a single default directory for packages on Windows like `/usr/local/lib`. If CMake cannot find Matplot++ on Windows or if you installed the library outside the default directory on Linux/Mac OS, there are a few [options](https://stackoverflow.com/questions/21314893/what-is-the-default-search-path-for-find-package-in-windows-using-cmake):

* **Environment Variables**: The most reliable way to set this default directory is through environment variables. You can create an environment variable `MATPLOTPP_DIR` and then add `$ENV{MATPLOTPP_DIR}` to the `HINTS` section of the `find_package` command. This tends to be more convenient than requiring the path on the command line every time. Starting with version 3.12, CMake now implicitly considers the `<PackageName>_Root` environment variable a HINT for every `find_package` call.
* **Package Registry**: CMake offers the [Package Registry](https://cmake.org/cmake/help/v3.5/manual/cmake-packages.7.html#package-registry) as an alternative mechanism for finding package locations. CMake maintains a list of package information in the Windows registry under `HKEY_CURRENT_USER\Software\Kitware\CMake\Packages\`.
* **Append CMAKE_MODULE_PATH**: You can append more directories to [`CMAKE_MODULE_PATH`](https://cmake.org/cmake/help/latest/variable/CMAKE_MODULE_PATH.html) with something like `list(APPEND CMAKE_MODULE_PATH "C:\\Program Files\\matplotplusplus 1.0.1")`. `CMAKE_MODULE_PATH` is a list of search paths for CMake modules to be loaded by the `include()` or `find_package()` commands.
* **Set the DIR variable directly**: Directly set the `Matplot++_DIR` variable with something like `set(Matplot++_DIR "C:\\Program Files\\matplotplusplus 1.0.1\\lib\\cmake\\Matplot++")`. This might be good enough for small local projects but it is hard-coding the directory in your build script. When your library gets out of your local environment, you need to choose one of the other options above (better) or make this variable an option and require the user to provide the directory on the command line every time (worse).




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
