#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    auto f = figure();
    f->width(f->width() * 2);

    auto x_reduced = randn(1000, 0, 1);
    auto y_reduced =
        transform(x_reduced, [](double x) { return 2 * x + randn(0, 1); });
    auto ax1 = subplot(1, 2, 0);
    scatter(ax1, x_reduced, y_reduced);
    title(ax1, "Scatter plot (n=1000)");

    auto x = randn(1000000, 0., 1.);
    auto y = transform(x, [](double x) { return 2 * x + randn(0, 1); });
    auto ax2 = subplot(1, 2, 1);
    binscatter(ax2, x, y);
    axis(tight);
    title(ax2, "Binned Scatter plot (n=1000000)");

    show();
    return 0;
}