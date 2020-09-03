#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(x);
    xlabel("Population");
    gca()->x_axis().label_color("red");

    show();
    return 0;
}