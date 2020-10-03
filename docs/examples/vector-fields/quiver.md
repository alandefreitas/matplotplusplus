---
layout: default
title: Quiver
nav_order: 1
has_children: false
parent: Vector Fields
grand_parent: Examples
has_toc: false
---
# Quiver

```cpp
quiver(x, y, u, v);
```


See result

[![example_quiver_1](../vector_fields/quiver/quiver_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/vector_fields/quiver/quiver_1.cpp)

More examples:
    
[![example_quiver_2](../vector_fields/quiver/quiver_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/vector_fields/quiver/quiver_2.cpp)  [![example_quiver_3](../vector_fields/quiver/quiver_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/vector_fields/quiver/quiver_3.cpp)  [![example_quiver_4](../vector_fields/quiver/quiver_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/vector_fields/quiver/quiver_4.cpp)


All these subcategories depend on the `vectors` object type. In a two-dimensional plot, for each value of <img src="https://render.githubusercontent.com/render/math?math=x"> and <img src="https://render.githubusercontent.com/render/math?math=y"> with the position of a vector, it also requires the value of <img src="https://render.githubusercontent.com/render/math?math=u"> and <img src="https://render.githubusercontent.com/render/math?math=v"> indicating its direction and magnitude. In a three-dimensional plot, the direction and magnitude are defined by <img src="https://render.githubusercontent.com/render/math?math=u">, <img src="https://render.githubusercontent.com/render/math?math=v">, and <img src="https://render.githubusercontent.com/render/math?math=w">.


A quiver plot (or velocity plot) shows a grid of vectors whose direction and magnitude are scaled to prevent the overlap between vectors in subsequent quads. 




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
