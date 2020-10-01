---
layout: default
title: Function Plot
nav_order: 9
has_children: false
parent: Line Plots
grand_parent: Examples
has_toc: false
---
# Function Plot

```cpp
fplot(fx);
```


See result
    
[![e../../../examples/line_plot/fplot/fplot_1.cppplot_1.svg)](examples/line_plot/fplot/fplot_1.cpp)

More examples:
    
[![e........../../../examples/line_plot/fplot/fplot_6.cppplot_6_thumb.png_plot/fplot/fplot_2.cpp)  [![example_fplot_3](docs/examples/line_plot/fplot/fplot_3_thumb.png)](examples/line_plot/fplot/fplot_3.cpp)  [![example_fplot_4](docs/examples/line_plot/fplot/fplot_4_thumb.png)](examples/line_plot/fplot/fplot_4.cpp)  [![example_fplot_5](docs/examples/line_plot/fplot/fplot_5_thumb.png)](examples/line_plot/fplot/fplot_5.cpp)  [![example_fplot_6](docs/examples/line_plot/fplot/fplot_6_thumb.png)](examples/line_plot/fplot/fplot_6.cpp)
  

Instead of storing data points, the objects `function line` and `string function` store a function as a lambda function or as a string with an expression. These objects use lazy evaluation to generate absolute data points. The data is generated only when the `draw` function is called.



