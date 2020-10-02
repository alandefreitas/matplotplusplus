---
layout: default
title: Member vs. Free-standing Functions
nav_order: 1
has_children: false
parent: Coding styles
has_toc: false
---
# Member vs. Free-standing Functions

Like in Matplotlib, we support two coding styles: Free-standing functions and an Object-oriented interface.
 
* Freestanding functions: 
    - We call functions to create plots on the current axes
    - The global current `axes` object is the current `axes` object in the current figure in the global figure registry
    - For instance, one can use `plot(y);` to create a line plot on the current axes (or create a new `axes` object if needed). 
    - Also, one can use `plot(ax,y);` to create a line plot on the `axes` object `ax`. 
    - This is less verbose for small projects and quick tests.
    - The library looks for existing axes to create the plot.

* Object-oriented interface: 
    - We explicitly create figures and call methods on them
    - For instance, one can use `ax->plot(y);` to plot on the `axes` object `ax`
    - We can create the same line plot on the current axes by `auto ax = gca(); ax->plot(y);`
    - This is less verbose and provides better control in large projects where we need to pass these objects around
    - The user manages axes handles containing plots.

Assuming the user is explicitly managing the axes to create plots in another function, a more complete example of these styles could be

```cpp
// Free-standing functions
auto ax = gca();
plot(ax, x, y)->color("red").line_width(2);
my_function(ax);
```

and

```cpp
// Object-oriented interface
auto ax = gca();
ax->plot(x, y)->color("red").line_width(2);
my_function(ax);
```

Both examples would generate the same plot. All free-standing functions are templated functions that use meta-programming to call the main function on the current `axes` object. If the first parameter is not an `axes_handle`, it will get an `axes_handle` from the figure registry with `gca` (Section [Axes Object](../examples/appearance/axes-object.md)) and forward all parameters to the function in this `axes` object. If the first parameter is an `axes_handle`, the template function will forward all parameters, but the first one, to this `axes` object. This use of templates for the free-standing functions keeps both coding styles maintainable by the developers.

Note that, because the example needs the `axes` object for the function `my_function`, we also need to get a reference to the `axes` object with the free-standing functions. In that case, the free-standing functions are not less verbose than the object-oriented interface.

To adhere to free-standing functions, we could create two versions of `my_function`: one that receives an `axes_handle`, and a second version that would get an `axes_handle` from the figure registry and call the first version. If `my_function` is going to be exposed to other users as a library, this could be a convenience to these users. However, notice that this is only moving the verbosity from the main function to `my_function`. In fact, this is how the free-standing functions in **Matplot++** work.




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
