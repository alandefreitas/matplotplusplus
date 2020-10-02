---
layout: default
title: Image Show
nav_order: 1
has_children: false
parent: Images
grand_parent: Examples
has_toc: false
---
# Image Show

```cpp
imshow(image);
```


See result

[![example_imshow_1](imshow/imshow_1.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_1.cpp)

More examples:
    
[![example_imshow_2](imshow/imshow_2_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_2.cpp)  [![example_imshow_3](imshow/imshow_3_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_3.cpp)  [![example_imshow_4](imshow/imshow_4_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_4.cpp)  [![example_imshow_5](imshow/imshow_5_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_5.cpp)  [![example_imshow_6](imshow/imshow_6_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_6.cpp)  [![example_imshow_7](imshow/imshow_7_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_7.cpp)  [![example_imshow_8](imshow/imshow_8_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_8.cpp)  [![example_imshow_9](imshow/imshow_9_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_9.cpp)  [![example_imshow_10](imshow/imshow_10_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_10.cpp)  [![example_imshow_11](imshow/imshow_11_thumb.png)](https://github.com/alandefreitas/matplotplusplus/blob/master/examples/images/imshow/imshow_11.cpp)


These subcategories depend on the `matrix` class. The `matrix` class can have up to four matrices. If it has only one matrix, it is represented with a colormap. If it has three matrices, they represent the red, green, and blue channels. If it has four matrices, the fourth matrix represents an alpha channel to control the transparency of each pixel.

We use the CImg library to load and save images. CImg can handle many common image formats as long as it has access to the appropriate libraries. The **Matplot++** build script will look at compile-time for the following optional libraries: JPEG, TIFF, ZLIB, PNG, LAPACK, BLAS, OpenCV, X11, fftw3, OpenEXR, and Magick++. The build script will attempt to link all libraries from this list to **Matplot++**.

**Matplot++** includes a few convenience functions to manipulate matrices with images: `imread`, `rgb2gray`, `gray2rgb`, `imresize`, and `imwrite`. All these functions work with lists of matrices. 




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
