---
layout: default
title: Histogram
nav_order: 1
has_children: false
parent: Data Distribution
grand_parent: Examples
has_toc: false
---
# Histogram

```cpp
hist(data);
```


See result
    
[![e../../../examples/data_distribution/histogram/histogram_1.cppistogram_1.png)](examples/data_distribution/histogram/histogram_1.cpp)

More examples:
    
[![e......................../../../examples/data_distribution/histogram/histogram_14.cppistogram_14_thumb.pngram_2.cpp)  [![example_histogram_3](docs/examples/data_distribution/histogram/histogram_3_thumb.png)](examples/data_distribution/histogram/histogram_3.cpp)  [![example_histogram_4](docs/examples/data_distribution/histogram/histogram_4_thumb.png)](examples/data_distribution/histogram/histogram_4.cpp)  [![example_histogram_5](docs/examples/data_distribution/histogram/histogram_5_thumb.png)](examples/data_distribution/histogram/histogram_5.cpp)  [![example_histogram_6](docs/examples/data_distribution/histogram/histogram_6_thumb.png)](examples/data_distribution/histogram/histogram_6.cpp)  [![example_histogram_7](docs/examples/data_distribution/histogram/histogram_7_thumb.png)](examples/data_distribution/histogram/histogram_7.cpp)  [![example_histogram_8](docs/examples/data_distribution/histogram/histogram_8_thumb.png)](examples/data_distribution/histogram/histogram_8.cpp)  [![example_histogram_9](docs/examples/data_distribution/histogram/histogram_9_thumb.png)](examples/data_distribution/histogram/histogram_9.cpp)  [![example_histogram_10](docs/examples/data_distribution/histogram/histogram_10_thumb.png)](examples/data_distribution/histogram/histogram_10.cpp)  [![example_histogram_11](docs/examples/data_distribution/histogram/histogram_11_thumb.png)](examples/data_distribution/histogram/histogram_11.cpp)  [![example_histogram_12](docs/examples/data_distribution/histogram/histogram_12_thumb.png)](examples/data_distribution/histogram/histogram_12.cpp)  [![example_histogram_14](docs/examples/data_distribution/histogram/histogram_14_thumb.png)](examples/data_distribution/histogram/histogram_14.cpp)
  

The `histogram` object creates the histogram edges and bins when the `draw` function is called for the first time with lazy evaluation. Lazy evaluation avoids calculating edges unnecessarily in case the user changes the object parameters before calling `draw`. This object includes several algorithms for automatically delimiting the edges and bins for the histograms. 



