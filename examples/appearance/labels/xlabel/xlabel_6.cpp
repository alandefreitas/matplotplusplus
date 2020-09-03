#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    plot(x);
    xlabel("Population");
    auto ax = gca();
    ax->x_axis().label_font_size(12);
    ax->x_axis().label_weight("bold");
    ax->x_axis().label_color({0, 1, 0, 0});

    show();
    return 0;
}