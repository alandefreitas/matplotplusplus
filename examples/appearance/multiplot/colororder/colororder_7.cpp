#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout();
    nexttile();
    auto p1 = plot(vector_2d{{1, 4}, {2, 5}, {3, 6}});
    p1[0]->line_width(2);
    p1[1]->line_width(2);
    p1[2]->line_width(2);

    auto ax = nexttile();
    auto p2 = plot(ax, vector_2d{{4, 1}, {5, 2}, {6, 3}});
    p2[0]->line_width(2);
    p2[1]->line_width(2);
    p2[2]->line_width(2);

    auto c = ax->colororder();
    c[0] = {0., 0.5, 0., 1.};
    p2[0]->color(c[0]);

    show();
    return 0;
}