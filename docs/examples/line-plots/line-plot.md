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

[![e../../../examples/line_plot/plot/plot_1.cpplot_1.svg)](examples/line_plot/plot/plot_1.cpp)

Where `x` and `y` are are any value ranges.


More examples
    
[![e....................../../../examples/line_plot/plot/plot_12.cpplot_12_thumb.png[example_plot_3](docs/examples/line_plot/plot/plot_3_thumb.png)](examples/line_plot/plot/plot_3.cpp)  [![example_plot_4](docs/examples/line_plot/plot/plot_4_thumb.png)](examples/line_plot/plot/plot_4.cpp)  [![example_plot_5](docs/examples/line_plot/plot/plot_5_thumb.png)](examples/line_plot/plot/plot_5.cpp)  [![example_plot_6](docs/examples/line_plot/plot/plot_6_thumb.png)](examples/line_plot/plot/plot_6.cpp)  [![example_plot_7](docs/examples/line_plot/plot/plot_7_thumb.png)](examples/line_plot/plot/plot_7.cpp)  [![example_plot_8](docs/examples/line_plot/plot/plot_8_thumb.png)](examples/line_plot/plot/plot_8.cpp)  [![example_plot_9](docs/examples/line_plot/plot/plot_9_thumb.png)](examples/line_plot/plot/plot_9.cpp)  [![example_plot_10](docs/examples/line_plot/plot/plot_10_thumb.png)](examples/line_plot/plot/plot_10.cpp)  [![example_plot_11](docs/examples/line_plot/plot/plot_11_thumb.png)](examples/line_plot/plot/plot_11.cpp)  [![example_plot_12](docs/examples/line_plot/plot/plot_12_thumb.png)](examples/line_plot/plot/plot_12.cpp)
  

Setters return a reference to `*this` to allow method chaining:

```cpp
plot(x,y)->line_width(2).color("red");
```

The examples use free-standing functions to create plots. You can also use a object-oriented style for plots. We discuss these coding styles in the Section [*Coding Styles*](../../coding-styles.md).



