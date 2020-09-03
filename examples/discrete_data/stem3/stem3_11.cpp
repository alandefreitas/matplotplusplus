#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, +2);
    std::vector<double> y = transform(x, [](double x) { return pow(x, 3); });
    std::vector<double> z =
        transform(x, y, [](double x, double y) { return exp(x) * cos(y); });

    stem3(x, y, z, "filled")->color("m").marker_face_color("y");
    view(-10, 35);

    show();
    return 0;
}