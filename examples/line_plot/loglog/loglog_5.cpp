#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2, 10000);
    std::vector<double> y1 =
        transform(x, [](auto x) { return 5 + 3 * sin(x / 4); });
    std::vector<double> y2 =
        transform(x, [](auto x) { return 5 - 3 * sin(x / 4); });

    loglog(x, y1, x, y2, "--");

    ::matplot::legend({"Signal 1", "Signal 2"})
        ->location(legend::general_alignment::topleft);

    axis({0.1, 100, 2, 8});

    show();
    return 0;
}