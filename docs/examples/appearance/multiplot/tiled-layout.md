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

[![e../../../../examples/appearance/multiplot/tiledlayout/tiledlayout_1.cpples/appearance/multiplot/tiledlayout/tiledlayout_1.cpp)
    
More examples:
    
[![e............../../../../examples/appearance/multiplot/tiledlayout/tiledlayout_8.cppthumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_3.cpp)  [![example_tiledlayout_4](docs/examples/appearance/multiplot/tiledlayout/tiledlayout_4_thumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_4.cpp)  [![example_tiledlayout_5](docs/examples/appearance/multiplot/tiledlayout/tiledlayout_5_thumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_5.cpp)  [![example_tiledlayout_6](docs/examples/appearance/multiplot/tiledlayout/tiledlayout_6_thumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_6.cpp)  [![example_tiledlayout_7](docs/examples/appearance/multiplot/tiledlayout/tiledlayout_7_thumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_7.cpp)  [![example_tiledlayout_8](docs/examples/appearance/multiplot/tiledlayout/tiledlayout_8_thumb.png)](examples/appearance/multiplot/tiledlayout/tiledlayout_8.cpp)


Our tiling functions are convenience shortcuts for the subplot functions. If there is no room for the next tile, we automatically rearrange the axes and increase the number of subplot rows or columns to fit the next tile. Use subplots for more control over the subplots. 
  


