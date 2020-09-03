#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2, 10000);
    std::vector<double> y = transform(x, [](auto x) { return 5 + 3 * sin(x); });
    loglog(x, y);
    yticks({3, 4, 5, 6, 7});
    xlabel("x");
    ylabel("5+3 sin(x)");

    show();
    return 0;
}