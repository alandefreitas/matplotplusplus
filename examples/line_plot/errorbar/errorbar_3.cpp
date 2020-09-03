#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 10, 100);
    std::vector<double> y = {20, 30, 45, 40, 60, 65, 80, 75, 95, 90};
    std::vector<double> err = {5, 8, 2, 9, 3, 3, 8, 3, 9, 3};
    errorbar(x, y, err);
    axis({0, 100, 0, 110});
    show();
    return 0;
}