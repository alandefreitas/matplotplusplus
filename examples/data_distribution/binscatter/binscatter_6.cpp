#include <tuple>
#include <random>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = randn(1e5,0,1);
    auto y = randn(1e5,0,1);

    binscatter(x, y, bin_scatter_style::point_colormap);
    colormap(gca(), palette::parula());

    wait();
    return 0;
}
