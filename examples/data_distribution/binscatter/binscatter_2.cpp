#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f = figure(true);

    f->width(f->width() * 2);
    f->height(f->height() * 2);
    f->x_position(200);
    f->y_position(100);

    auto x = randn(1000000, 0., 1.);
    auto y = transform(x, [](double x) { return 2 * x + randn(0, 1); });
    std::vector<double> x_line(x.begin(), x.begin() + 1000);
    std::vector<double> y_line(y.begin(), y.begin() + 1000);

    subplot(2, 3, 0);
    scatter(x_line, y_line);
    title("Scatter");

    subplot(2, 3, 1);
    binscatter(x, y, bin_scatter_style::point_size);
    title("Bin Scatter: Point size");

    subplot(2, 3, 2);
    binscatter(x, y, bin_scatter_style::point_alpha);
    title("Bin Scatter: Point alpha");

    subplot(2, 3, 3);
    binscatter(x, y, bin_scatter_style::point_colormap);
    title("Bin Scatter: Colormap");

    subplot(2, 3, 4);
    binscatter(x, y, bin_scatter_style::heatmap);
    axis(tight);
    title("Bin Scatter: Heatmap");

    subplot(2, 3, 5);
    binscatter(x, y, bin_scatter_style::jitter);
    title("Bin Scatter: Jitter");

    f->show();

    return 0;
}