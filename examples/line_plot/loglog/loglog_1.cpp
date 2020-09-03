#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2);
    std::vector<double> y = transform(x, [](auto x) { return pow(2, x); });
    loglog(x, y);

    show();
    return 0;
}