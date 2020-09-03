#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> X = linspace(0, 4 * pi, 20);
    std::vector<double> Y = transform(X, [](double x) { return sin(x); });

    auto s = stairs(Y)->line_width(2).marker("d").marker_face_color("c");

    show();
    return 0;
}