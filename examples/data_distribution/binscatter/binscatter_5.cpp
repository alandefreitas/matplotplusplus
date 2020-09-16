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
    f->quiet_mode(true);

    auto x = randn(10000, 0., 1.);
    auto y = randn(10000, 0., 1.);

    subplot(2, 3, 0);
    scatter(x, y);
    title("Scatter plot");

    subplot(2, 3, 1);
    binscatter(x, y, 30, 10, bin_scatter_style::point_size);
    title("Binned scatter plot: Point size");

    subplot(2, 3, 2);
    binscatter(x, y, 30, 10, bin_scatter_style::point_alpha);
    title("Binned scatter plot: Point alpha");

    subplot(2, 3, 3);
    binscatter(x, y, 30, 10, bin_scatter_style::jitter);
    title("Binned scatter plot: Jitter");

    subplot(2, 3, 4);
    binscatter(x, y, 30, 10, bin_scatter_style::point_colormap);
    title("Binned scatter plot: Colormap");

    subplot(2, 3, 5);
    binscatter(x, y, 30, 10, bin_scatter_style::heatmap);
    title("Binned scatter plot: Heatmap");
    axis(tight);

    f->draw();

    show();
    return 0;
}
