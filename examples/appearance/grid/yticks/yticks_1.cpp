#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    plot(x, y);
    yticks({0, 50, 100});
    yticklabels({"y=0", "y=50", "y=100"});

    show();
    return 0;
}