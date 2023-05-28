#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_2d magic = {{8, 3, 4}, {1, 5, 9}, {6, 7, 2}};
    bar(magic);
    auto ax = gca();
    ax->y_axis().color(ax->colororder()[0]);

    hold(on);
    scatter(std::vector{1., 2., 3.}, std::vector{2., 5., 2.})
        ->marker_face(true)
        .use_y2(true);
    scatter(std::vector{1., 2., 3.}, std::vector{3., 4., 1.})
        ->marker_face(true)
        .use_y2(true);
    scatter(std::vector{1., 2., 3.}, std::vector{4., 2., 4.})
        ->marker_face(true)
        .use_y2(true);
    ax->y2_axis().color(ax->colororder()[1]);
    hold(off);

    ::matplot::legend();

    show();
    return 0;
}