#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 2, 15);
    std::vector<double> y = transform(x, [](double x) { return exp(x); });
    std::vector<double> err(y.size(), 0.3);

    auto e = errorbar(x, y, err)->cap_size(18);

    show();
    return 0;
}