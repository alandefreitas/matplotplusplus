#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    auto f = figure(true);
    f->width(f->width() * 2);
    f->height(f->height() * 2);
    f->x_position(200);
    f->y_position(100);

    auto x = randn(1000000, 0., 1.);
    auto y = transform(x, [](double x) { return 2 * x + randn(0, 1); });

    bin_scatter_style b = bin_scatter_style::automatic;

    subplot(2, 3, 0);
    binscatter(x, y, histogram::binning_algorithm::automatic, b);
    axis(tight);
    title("Automatic");

    subplot(2, 3, 1);
    binscatter(x, y, histogram::binning_algorithm::integers, b);
    axis(tight);
    title("Integers");

    subplot(2, 3, 2);
    binscatter(x, y, histogram::binning_algorithm::scott, b);
    axis(tight);
    title("Scott's rule");

    subplot(2, 3, 3);
    binscatter(x, y, histogram::binning_algorithm::fd, b);
    axis(tight);
    title("Freedman-Diaconis rule");

    subplot(2, 3, 4);
    binscatter(x, y, histogram::binning_algorithm::sqrt, b);
    axis(tight);
    title("Square root rule");

    subplot(2, 3, 5);
    binscatter(x, y, histogram::binning_algorithm::sturges, b);
    axis(tight);
    title("Sturges' rule");

    f->show();

    return 0;
}