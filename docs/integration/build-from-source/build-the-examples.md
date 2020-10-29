---
layout: default
title: Build the Examples
nav_order: 2
has_children: false
parent: Build from Source
grand_parent: Integration
has_toc: false
---
# Build the Examples

This will build the examples in the `build/examples` directory:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2"
cmake --build . --parallel 2 --config Release
```

* Replace `--parallel 2` with `--parallel <number of cores in your machine>`
* On Windows, replace `-O2` with `/O2` 
* On Linux, you might need `sudo` for this last command




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
