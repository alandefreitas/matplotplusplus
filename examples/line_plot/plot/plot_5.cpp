#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 10);
    std::vector<double> y = transform(x, [](auto x) { return sin(x); });
    plot(x, y, "-o")->marker_indices({0,  5,  10, 15, 20, 25, 30, 35, 40, 45,
                                      50, 55, 60, 65, 70, 75, 80, 85, 90, 95});

    show();
    return 0;
}