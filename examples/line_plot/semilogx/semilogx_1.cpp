#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = iota(0, 1000);
    std::vector<double> y = transform(x, [](auto x) { return log(x); });
    semilogx(x, y);

    show();
    return 0;
}