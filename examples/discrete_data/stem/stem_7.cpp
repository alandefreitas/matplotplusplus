#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi, 25);
    std::vector<double> y = transform(x, [](auto x) { return cos(2 * x); });

    auto s = stem(x, y)->line_style("-.").marker_face_color("red").marker_color(
        "green");

    show();
    return 0;
}