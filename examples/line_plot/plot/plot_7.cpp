#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 10, 150);
    std::vector<double> y = transform(x, [](auto x) { return cos(5 * x); });
    plot(x, y)->color({0, 0.7, 0.9});
    title("2-D Line Plot");
    xlabel("x");
    ylabel("cos(5x)");

    wait();
    return 0;
}