#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 0.1, 10);
    std::vector<double> y = transform(x, [](auto x) { return exp(x); });
    semilogy(x, y);

    show();
    return 0;
}