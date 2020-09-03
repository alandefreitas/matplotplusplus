#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> grid_ticks = iota(0, .1, 1);
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    for (const auto &x_i : grid_ticks) {
        for (const auto &y_i : grid_ticks) {
            x.emplace_back(x_i);
            y.emplace_back(y_i);
            z.emplace_back(exp(x_i + y_i));
        }
    }

    stem3(x, y, z);

    show();
    return 0;
}