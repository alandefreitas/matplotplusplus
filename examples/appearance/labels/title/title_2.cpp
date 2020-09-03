#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(x);
    double f = 70.;
    double c = (f - 32) / 1.8;
    title("Temperature is " + num2str(c) + " C");

    show();
    return 0;
}