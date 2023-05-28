#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, pi);
    auto y1 = transform(x, [](double x) { return cos(x); });
    auto y2 = transform(x, [](double x) { return cos(2 * x); });
    auto y3 = transform(x, [](double x) { return cos(3 * x); });
    auto y4 = transform(x, [](double x) { return cos(4 * x); });

    plot(x, y1);
    hold(on);
    plot(x, y2);
    plot(x, y3);
    plot(x, y4);
    hold(off);

    auto l = ::matplot::legend({"cos(x)", "cos(2x)", "cos(3x)", "cos(4x)"});
    l->location(legend::general_alignment::topleft);
    l->num_rows(2);

    show();
    return 0;
}