#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 10, 150);
    std::vector<double> y = transform(x, [](auto x) { return cos(5 * x); });
    plot(x, y)->color({0.f, 0.7f, 0.9f});
    title("2-D Line Plot");
    xlabel("x");
    ylabel("cos(5x)");

    show();
    return 0;
}