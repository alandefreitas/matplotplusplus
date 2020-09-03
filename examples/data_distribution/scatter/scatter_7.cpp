#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto x = linspace(0, 3 * pi, 200);
    auto y = transform(x, [&](double x) { return cos(x) + rand(0, 1); });

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    scatter(ax1, x, y);

    auto ax2 = nexttile();
    auto l = scatter(ax2, x, y);
    l->marker_face(true);
    l->marker_style(line_spec::marker_style::diamond);

    show();
    return 0;
}