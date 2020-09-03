#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
    std::vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
    std::vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
    plot(x, y1, "g", x, y2, "b--o", x, y3, "c*");

    show();
    return 0;
}