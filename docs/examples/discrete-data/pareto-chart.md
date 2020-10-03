---
layout: default
title: Pareto Chart
nav_order: 2
has_children: false
parent: Discrete Data
grand_parent: Examples
has_toc: false
---
# Pareto Chart

```cpp
pareto(y);
```


See result

[![example_pareto_1](../discrete_data/pareto/pareto_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/discrete_data/pareto/pareto_1.cpp)

More examples:
    
[![example_pareto_2](../discrete_data/pareto/pareto_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/discrete_data/pareto/pareto_2.cpp)  [![example_pareto_3](../discrete_data/pareto/pareto_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/discrete_data/pareto/pareto_3.cpp) [![example_pareto_4](../discrete_data/pareto/pareto_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/discrete_data/pareto/pareto_4.cpp)


Pareto Charts are a type of chart that uses both <img src="https://render.githubusercontent.com/render/math?math=y"> axes. The <img src="https://render.githubusercontent.com/render/math?math=y_1"> axis is used to represent bars with the data values in descending order. The <img src="https://render.githubusercontent.com/render/math?math=y_2"> axis is used to represent the cumulative distribution function of the data in the <img src="https://render.githubusercontent.com/render/math?math=y_1"> axis. By default, a Pareto Chart includes up to 10 items or as many items as needed to represent 95% of the cumulative distribution.

If you need Pareto *fronts* rather than Pareto *charts*, we refer to [Scatter Plots](../data-distribution/scatter-plot.md) for two-dimensional fronts, [Plot matrices](../data-distribution/plot-matrix.md) for three-dimensional fronts, or [Parallel Coordinate Plots](../data-distribution/parallel-coordinates.md) for many-objective fronts. These plot subcategories are described in Section [Data Distribution](../data-distribution.md). If you also need a tool to calculate these fronts efficiently, we refer to the [Pareto Front Library](https://github.com/alandefreitas/pareto-front).




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
