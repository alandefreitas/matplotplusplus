#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 10, 100);
    std::vector<double> y = {20, 30, 45, 40, 60, 65, 80, 75, 95, 90};
    std::vector<double> err(y.size(), 10.);
    errorbar(x, y, err)->filled_curve(true);

    axis({0, 100, 0, 110});

    show();
    return 0;
}