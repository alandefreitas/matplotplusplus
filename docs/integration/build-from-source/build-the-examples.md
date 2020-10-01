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
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2"
cmake --build . -j 2 --config Release
```

On windows, replace `-O2` with `/O2`.





<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
