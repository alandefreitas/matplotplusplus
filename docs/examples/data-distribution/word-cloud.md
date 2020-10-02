---
layout: default
title: Word Cloud
nav_order: 10
has_children: false
parent: Data Distribution
grand_parent: Examples
has_toc: false
---
# Word Cloud

```cpp
wordcloud(text, black_list);
```


See result

[![example_wordcloud_1](../data_distribution/wordcloud/wordcloud_1.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/data_distribution/wordcloud/wordcloud_1.cpp)

More examples:
    
[![example_wordcloud_3](../data_distribution/wordcloud/wordcloud_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/data_distribution/wordcloud/wordcloud_3.cpp)  [![example_wordcloud_4](../data_distribution/wordcloud/wordcloud_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/data_distribution/wordcloud/wordcloud_4.cpp)  [![example_wordcloud_4](../data_distribution/wordcloud/wordcloud_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/data_distribution/wordcloud/wordcloud_4.cpp)


Word clouds are generated from text or pairs of words and their frequency. After attributing a size proportional to each word frequency, the algorithm to position the labels iterates words from the largest to the smallest. For each word, it spins the word in polar coordinates converted to Cartesian coordinates until it does not overlap with any other word.

By default, the colors and the sizes depend on the word frequencies. We can customize the colors by passing a third parameter to the `wordcloud` function.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
