#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 10, 15);
    std::vector<double> y = transform(x, [](double x) { return sin(x / 2); });
    std::vector<double> err(y.size(), 0.3);

    auto e = errorbar(x, y, err, "-s")
                 ->marker_size(10)
                 .marker_color("red")
                 .marker_face_color("red");

    show();
    return 0;
}