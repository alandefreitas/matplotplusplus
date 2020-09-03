#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> leftcolors = {
        {0, 0, 1}, {0, 0.5, 1}, {0, 0.8, 1}};
    colororder(leftcolors);
    plot(vector_2d{{1, 4}, {2, 5}, {3, 6}});
    auto ax = gca();
    ax->y_axis().color(ax->colororder()[0]);

    hold(on);
    auto p2 = plot(vector_2d{{4, 2}, {3, 1}}, "k-");
    p2[0]->use_y2(true);
    p2[1]->use_y2(true).line_style("--");
    ax->y_axis().color("k");

    show();
    return 0;
}