---
layout: default
title: Undirected Graph
nav_order: 1
has_children: false
parent: Graphs
grand_parent: Examples
has_toc: false
---
# Undirected Graph

```cpp
graph(edges);
```


See result

[![example_graph_1](graph/graph_1.svg)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/graphs/graph/graph_1.cpp)

More examples:
    
[![example_graph_2](graph/graph_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/graphs/graph/graph_2.cpp)  [![example_graph_3](graph/graph_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/graphs/graph/graph_3.cpp)  [![example_graph_4](graph/graph_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/graphs/graph/graph_4.cpp)  [![example_graph_5](graph/graph_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/graphs/graph/graph_5.cpp)
  

All these subcategories depend on the `network` class. Graphs are abstract structures that represent objects and relationships between these objects. The objects are represented as vertices and the relationships are depicted as edges.

In an abstract graph, the vertices have no specific position in space. Mathematically, a graph does not depend on its layout. However, the graph layout has a large impact on its understandability. The `network` class can calculate appropriate positions for graph vertices with several algorithms: Kamada Kawai algorithm, Fruchterman-Reingold algorithm, circle layout, random layout, and automatic layout.

The implementation of the Kamada Kawai and Fruchterman-Reingold algorithms depend on the NodeSoup library. The automatic layout uses the Kamada Kawai algorithm for small graphs and the Fruchterman-Reingold algorithm for larger graphs.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
