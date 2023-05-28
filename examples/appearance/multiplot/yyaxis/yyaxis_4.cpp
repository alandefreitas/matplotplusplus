#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    colororder({"b", "m"});

    vector_2d y = {{1, 3}, {2, 4}};
    auto p1 = plot(y);
    p1[0]->color("b");
    p1[1]->color("b").line_style("--");
    gca()->y_axis().color("b");

    hold(on);
    vector_2d z = {{4, 2}, {3, 1}};
    auto ps = plot(z);
    ps[0]->use_y2(true).color("m");
    ps[1]->use_y2(true).color("m").line_style("--");
    gca()->y2_axis().color("m");

    ::matplot::legend();

    show();
    return 0;
}