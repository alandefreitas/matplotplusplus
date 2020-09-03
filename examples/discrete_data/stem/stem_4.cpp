#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x1 = linspace(0, 2 * pi, 50);
    std::vector<double> y1 = transform(x1, [](auto x) { return cos(x); });

    std::vector<double> x2 = linspace(pi, 3 * pi, 50);
    std::vector<double> y2 = transform(x2, [](auto x) { return 0.5 * sin(x); });

    stem(x1, y1);
    hold(on);
    stem(x2, y2);

    show();
    return 0;
}