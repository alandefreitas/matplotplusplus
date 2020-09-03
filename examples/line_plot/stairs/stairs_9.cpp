#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 1, 30);
    std::vector<std::vector<double>> Y(2);
    Y[0] = transform(x, [](double x) { return cos(10 * x); });
    Y[1] = transform(x, [](double x) { return exp(x) * sin(10 * x); });

    auto h = stairs(x, Y);
    h[0]->marker(line_spec::marker_style::circle).marker_size(4);
    h[1]->marker(line_spec::marker_style::circle).marker_face_color("m");

    show();
    return 0;
}