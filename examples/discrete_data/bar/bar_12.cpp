#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x1 = concat({1}, iota(3, 10));
    bar(x1, rand(9, 0, 1));
    hold(on);
    bar(std::vector<double>{2}, rand(1, 0, 1));
    gca()->x_axis().tick_values(iota(1, 10));

    show();
    return 0;
}