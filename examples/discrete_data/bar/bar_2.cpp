#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(1900, 10, 2000);
    std::vector<double> y = {75,  91,  105, 123.5, 131,  150,
                             179, 203, 226, 249,   281.5};
    bar(x, y);

    show();
    return 0;
}