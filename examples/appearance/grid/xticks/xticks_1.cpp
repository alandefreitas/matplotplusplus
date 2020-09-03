#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    plot(x, y);
    xticks({0, 5, 10});
    xticklabels({"x=0", "x=5", "x=10"});

    show();
    return 0;
}