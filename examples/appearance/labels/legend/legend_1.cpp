#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, pi);
    std::vector<double> y1 = transform(x, [](auto x) { return cos(x); });
    plot(x, y1);

    hold(on);
    std::vector<double> y2 = transform(x, [](auto x) { return cos(2 * x); });
    plot(x, y2);

    legend("cos(x)", "cos(2x)");

    std::vector<double> y3 = transform(x, [](auto x) { return cos(3 * x); });
    auto p = plot(x, y3);
    p->display_name("cos(3x)");
    hold(off);

    wait();

    legend(off);
    wait();

    return 0;
}