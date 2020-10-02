---
layout: default
title: Line Plot
nav_order: 1
has_children: false
parent: Line Plots
grand_parent: Examples
has_toc: false
---
# Line Plot

```cpp
plot(x,y); 
```

[![example_plot_1](../line_plot/plot/plot_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_1.cpp)

Where `x` and `y` are are any value ranges.


More examples
    
[![example_plot_2](../line_plot/plot/plot_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_2.cpp)  [![example_plot_3](../line_plot/plot/plot_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_3.cpp)  [![example_plot_4](../line_plot/plot/plot_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_4.cpp)  [![example_plot_5](../line_plot/plot/plot_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_5.cpp)  [![example_plot_6](../line_plot/plot/plot_6_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_6.cpp)  [![example_plot_7](../line_plot/plot/plot_7_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_7.cpp)  [![example_plot_8](../line_plot/plot/plot_8_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_8.cpp)  [![example_plot_9](../line_plot/plot/plot_9_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_9.cpp)  [![example_plot_10](../line_plot/plot/plot_10_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_10.cpp)  [![example_plot_11](../line_plot/plot/plot_11_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_11.cpp)  [![example_plot_12](../line_plot/plot/plot_12_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/plot/plot_12.cpp)
  

Setters return a reference to `*this` to allow method chaining:

```cpp
plot(x,y)->line_width(2).color("red");
```

The examples use free-standing functions to create plots. You can also use a object-oriented style for plots. We discuss these coding styles in the Section [*Coding Styles*](../../coding-styles.md).




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
