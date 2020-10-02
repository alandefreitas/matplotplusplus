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
    
[![example_fplot_1](../line_plot/fplot/fplot_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_1.cpp)

More examples:
    
[![example_fplot_2](../line_plot/fplot/fplot_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_2.cpp)  [![example_fplot_3](../line_plot/fplot/fplot_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_3.cpp)  [![example_fplot_4](../line_plot/fplot/fplot_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_4.cpp)  [![example_fplot_5](../line_plot/fplot/fplot_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_5.cpp)  [![example_fplot_6](../line_plot/fplot/fplot_6_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/line_plot/fplot/fplot_6.cpp)
  

Instead of storing data points, the objects `function line` and `string function` store a function as a lambda function or as a string with an expression. These objects use lazy evaluation to generate absolute data points. The data is generated only when the `draw` function is called.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
