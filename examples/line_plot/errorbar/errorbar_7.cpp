#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 10, 100);
    std::vector<double> y = {20, 30, 45, 40, 60, 65, 80, 75, 95, 90};
    std::vector<double> yneg = {1, 3, 5, 3, 5, 3, 6, 4, 3, 3};
    std::vector<double> ypos = {2, 5, 3, 5, 2, 5, 2, 2, 5, 5};
    std::vector<double> xneg = {1, 3, 5, 3, 5, 3, 6, 4, 3, 3};
    std::vector<double> xpos = {2, 5, 3, 5, 2, 5, 2, 2, 5, 5};

    errorbar(x, y, yneg, ypos, xneg, xpos, "o");
    axis({0, 100, 0, 100});

    show();
    return 0;
}