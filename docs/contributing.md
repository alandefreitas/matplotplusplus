---
layout: default
title: Contributing
nav_order: 6
has_children: true
has_toc: false
---
# Contributing

There are many ways in which you can contribute to this library:

* Testing the library in new environments <sup>see [1](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22cross-platform+issue+-+windows%22), [2](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22cross-platform+issue+-+linux%22), [3](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22cross-platform+issue+-+macos%22) </sup>
* Contributing with interesting examples <sup>see [1](https://github.com/alandefreitas/matplotplusplus/blob/master/source/examples)</sup>
* Designing new backends <sup>see [1](https://github.com/alandefreitas/matplotplusplus/blob/master/source/matplot/backend/backend_interface.h), [2](https://github.com/alandefreitas/matplotplusplus/blob/master/test/backends/ogl_main.cpp), [3](integration/backends.md), [4](README.html#backends)</sup>
* Finding problems in this documentation <sup>see [1](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22enhancement+-+documentation%22) </sup>
* Writing algorithms for new plot categories <sup>see [1](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22enhancement+-+plot+categories%22) </sup>
* Finding bugs in general <sup>see [1](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22bug+-+compilation+error%22), [2](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22bug+-+compilation+warning%22), [3](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22bug+-+runtime+error%22), [4](https://github.com/alandefreitas/matplotplusplus/issues?q=is%3Aopen+is%3Aissue+label%3A%22bug+-+runtime+warning%22) </sup>
* Whatever idea seems interesting to you

If contributing with code, please leave the OpenGL backend and pedantic mode ON (`-DBUILD_EXPERIMENTAL_OPENGL_BACKEND=ON -DBUILD_WITH_PEDANTIC_WARNINGS=ON`).


Example: CLion
    
![CLion Settings with Pedantic Mode](img/pedantic_clion.png)
    


If contributing to the documentation, please edit [`README.md`](https://github.com/alandefreitas/matplotplusplus/blob/master/README.md) directly, as the files in [`./docs`](.) are automatically generated with [mdsplit](https://github.com/alandefreitas/mdsplit).


- [Contributors](contributing/contributors.md)


<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
