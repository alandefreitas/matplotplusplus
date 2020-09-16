#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    auto x = randn(100000, 0., 1.);
    auto y = randn(100000, 0., 1.);

    binscatter(x, y, 20, 30, bin_scatter_style::heatmap);
    axis(tight);

    show();
    return 0;
}
