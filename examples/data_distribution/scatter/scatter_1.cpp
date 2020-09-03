#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto x = linspace(0, 3 * pi, 200);
    auto y = transform(x, [&](double x) { return cos(x) + rand(0, 1); });

    scatter(x, y);

    show();
    return 0;
}