#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_2d newcolors = {{0.83, 0.14, 0.14},
                           {1.00, 0.54, 0.00},
                           {0.47, 0.25, 0.80},
                           {0.25, 0.80, 0.54}};
    colororder(newcolors);

    auto x = linspace(0, 10);
    auto y1 = transform(x, [](double x) { return sin(x); });
    auto y2 = transform(x, [](double x) { return sin(x - 0.5); });
    auto y3 = transform(x, [](double x) { return sin(x - 1); });
    auto y4 = transform(x, [](double x) { return sin(x - 1.5); });

    plot(x, y1)->line_width(2);
    hold(on);
    plot(x, y2)->line_width(2);
    plot(x, y3)->line_width(2);
    plot(x, y4)->line_width(2);
    hold(off);

    show();
    return 0;
}