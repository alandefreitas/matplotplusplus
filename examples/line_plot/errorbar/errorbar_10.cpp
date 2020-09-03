#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 10, 10);
    std::vector<double> y = transform(x, [](double x) { return sin(x / 2.); });
    std::vector<double> err(y.size(), 0.3);

    auto e =
        errorbar(x, y, err)->cap_size(15).marker("*").marker_size(10).color(
            "red");

    show();
    return 0;
}