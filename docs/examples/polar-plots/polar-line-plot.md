---
layout: default
title: Polar Line Plot
nav_order: 1
has_children: false
parent: Polar Plots
grand_parent: Examples
has_toc: false
---
# Polar Line Plot

```cpp
polarplot(theta, rho);
```


See result

[![example_polarplot_1](../polar_plots/polarplot/polarplot_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_1.cpp)

More examples:
    
[![example_polarplot_2](../polar_plots/polarplot/polarplot_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_2.cpp)  [![example_polarplot_3](../polar_plots/polarplot/polarplot_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_3.cpp)  [![example_polarplot_4](../polar_plots/polarplot/polarplot_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_4.cpp)  [![example_polarplot_5](../polar_plots/polarplot/polarplot_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_5.cpp)  [![example_polarplot_6](../polar_plots/polarplot/polarplot_6_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_6.cpp)  [![example_polarplot_7](../polar_plots/polarplot/polarplot_7_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_7.cpp)  [![example_polarplot_8](../polar_plots/polarplot/polarplot_8_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/polar_plots/polarplot/polarplot_8.cpp)

  
By emplacing a polar plot in the `axes`, the `axes` move to a polar mode, where we use the $r$ and $t$ axis instead of the $x$ and $y$ axis.

From the backend point of view, these axes are an abstraction to the user. The data points in the $r$ and $t$ axis are drawn by converting the positions from the polar coordinates $r$ and $t$ to the Cartesian coordinates $x$ and $y$ with the relationships $x=r \cos{t}$ and $y=r \sin{t}$.

Aside from this conversion, these plot subcategories are analogous to line plots, scatter plots, histograms, quiver plots, and line functions. 
  



<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
