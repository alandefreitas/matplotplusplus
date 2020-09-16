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

    bin_scatter_style b = bin_scatter_style::heatmap;
    histogram::binning_algorithm a = histogram::binning_algorithm::automatic;

    subplot(2, 3, 0);
    binscatter(x, y, a, b, histogram::normalization::count);
    axis(tight);
    title("Normalization: Count");

    subplot(2, 3, 1);
    binscatter(x, y, a, b, histogram::normalization::probability);
    axis(tight);
    title("Normalization: Probability");

    subplot(2, 3, 2);
    binscatter(x, y, a, b, histogram::normalization::cummulative_count);
    axis(tight);
    title("Normalization: Cummulative count");

    subplot(2, 3, 3);
    binscatter(x, y, a, b, histogram::normalization::count_density);
    axis(tight);
    title("Normalization: Count density");

    subplot(2, 3, 4);
    binscatter(x, y, a, b, histogram::normalization::pdf);
    axis(tight);
    title("Normalization: PDF");

    subplot(2, 3, 5);
    binscatter(x, y, a, b, histogram::normalization::cdf);
    axis(tight);
    title("Normalization: CDF");

    f->show();

    return 0;
}