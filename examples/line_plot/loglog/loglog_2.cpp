#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2);
    std::vector<double> y1 = transform(x, [](auto x) { return pow(10, x); });
    std::vector<double> y2 =
        transform(x, [](auto x) { return 1 / pow(10, x); });
    loglog(x, y1, x, y2);

    show();
    return 0;
}