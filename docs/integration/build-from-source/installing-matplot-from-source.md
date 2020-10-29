---
layout: default
title: Installing Matplot++ from Source
nav_order: 3
has_children: false
parent: Build from Source
grand_parent: Integration
has_toc: false
---
# Installing Matplot++ from Source

This will install Matplot++ on your system:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2" -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF 
cmake --build . --parallel 2 --config Release
cmake --install .
```

* Replace `--parallel 2` with `--parallel <number of cores in your machine>` 
* On Windows, replace `-O2` with `/O2` 
* On Linux, you might need `sudo` for this last command




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
