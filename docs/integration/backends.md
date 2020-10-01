---
layout: default
title: Backends
nav_order: 5
has_children: false
parent: Integration
has_toc: false
---
# Backends

Coming up with new backends is a continuous process. See the complete [article](../README.md) for a description of t../../source/matplot/backend/backend_interf../../source/matplot/backend/gnuplo../README.md#backendscurrent def../../source/matplot/backend pipe](source/matplot/back../../test/backends/ogl_main.cpps involved in possible [new backends](docs/README.md#backends). See the directory [`source/matplot/backend`](source/matplot/backend) for some examples. Also, have a look at this example [`test/backends/main.cpp`](test/backends/ogl_main.cpp). 

If you're in a hurry, here is a summary of the backends we have and the backends we have been considering or working on:

* Gnuplot
    * Pros: It seems to be working for everyone.
    * Cons: Pipes are comparatively slow and unidirectional
    * *In practice, this is default backend you'll get right now.*
* OpenGL
    * Pros: Efficient for many FPS.
    * Cons: Blocks the main thread on some operating systems
    * *The experimental OpenGL backend already works for some plot categories.* <sup>see [1](../../test/backends/ogl_main.cpp) </sup>
* Qt
    * Same as OpenGL <sup>see [1](https://doc.qt.io/qt-5/qtgui-index.html#opengl-and-opengl-es-integration), [2](https://doc.qt.io/qt-5/qtopengl-index.html) </sup>
* AGG
    * Pros: Great for vector graphics
    * Cons: Unmaintained, 2D only, and non-interactive by itself <sup>see [1](https://github.com/ghaerr/agg-2.6#roadmap), [2](https://github.com/mapnik/mapnik/wiki/MapnikRenderers), [3](http://www.antigrain.com/) </sup>



