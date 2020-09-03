#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 10, 100);
    std::vector<double> y = {20, 30, 45, 40, 60, 65, 80, 75, 95, 90};
    std::vector<double> err = {4, 3, 5, 3, 5, 3, 6, 4, 3, 3};
    errorbar(x, y, err, error_bar::type::both, "o");
    axis({0, 100, 0, 100});
    axis(equal);
    show();
    return 0;
}