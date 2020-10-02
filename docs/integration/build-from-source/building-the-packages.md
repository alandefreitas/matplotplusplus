---
layout: default
title: Building the packages
nav_order: 4
has_children: false
parent: Build from Source
grand_parent: Integration
has_toc: false
---
# Building the packages

This will create the binary packages you can use to install Matplot++ on your system:

```bash
mkdir build
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2" -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF
cmake --build . -j 2 --config Release
cmake --install .
cpack .
```

On windows, replace `-O2` with `/O2`. You might need `sudo` for this last command.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
