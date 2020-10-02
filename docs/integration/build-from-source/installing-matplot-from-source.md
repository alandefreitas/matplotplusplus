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
cmake -version
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2" -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF 
cmake --build . -j 2 --config Release
cmake --install .
```

On windows, replace `-O2` with `/O2`. You might need `sudo` for this last command.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
