#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto t = linspace(0, 1);
    auto y = transform(t, [](double t) { return exp(t); });
    plot(t, y);
    ylabel("t_{seconds}");
    ylabel("e^t");

    show();
    return 0;
}