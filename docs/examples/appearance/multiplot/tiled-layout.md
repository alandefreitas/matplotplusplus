---
layout: default
title: Tiled Layout
nav_order: 5
has_children: false
parent: Multiplot
grand_parent: Appearance
has_toc: false
---
# Tiled Layout

```cpp
tiledlayout(rows, cols);
nexttile();
```


See result

[![example_tiledlayout_1](tiledlayout/tiledlayout_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_1.cpp)
    
More examples:
    
[![example_tiledlayout_2](tiledlayout/tiledlayout_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_2.cpp)  [![example_tiledlayout_3](tiledlayout/tiledlayout_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_3.cpp)  [![example_tiledlayout_4](tiledlayout/tiledlayout_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_4.cpp)  [![example_tiledlayout_5](tiledlayout/tiledlayout_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_5.cpp)  [![example_tiledlayout_6](tiledlayout/tiledlayout_6_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_6.cpp)  [![example_tiledlayout_7](tiledlayout/tiledlayout_7_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_7.cpp)  [![example_tiledlayout_8](tiledlayout/tiledlayout_8_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/appearance/multiplot/tiledlayout/tiledlayout_8.cpp)


Our tiling functions are convenience shortcuts for the subplot functions. If there is no room for the next tile, we automatically rearrange the axes and increase the number of subplot rows or columns to fit the next tile. Use subplots for more control over the subplots. 
  



<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
