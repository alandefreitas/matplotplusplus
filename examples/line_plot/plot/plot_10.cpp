#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(-2 * pi, 3);
    std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
    std::vector<double> y2 = transform(x, [](auto x) { return cos(x); });
    auto p = plot(x, y1, x, y2);
    p[0]->line_width(2);
    p[1]->marker(line_spec::marker_style::asterisk);

    show();
    return 0;
}