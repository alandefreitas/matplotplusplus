#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi, 50);
    std::vector<double> y = transform(x, [](auto x) { return cos(x); });

    stem(x, y);

    show();
    return 0;
}