#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(-5, +5, 60);
    std::vector<double> y = transform(x, [](double x) { return cos(x); });
    std::vector<double> z = transform(x, [](double x) { return pow(x, 2); });

    stem3(x, y, z);
    view(-8, 30);

    wait();
    return 0;
}