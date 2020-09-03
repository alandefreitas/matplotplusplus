#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2, 20);
    std::vector<double> y = transform(x, [](auto x) { return pow(10, x); });
    loglog(x, y, "s")->marker_face_color({0, 0.447, 0.741});
    xlabel("x");
    ylabel("10^x");

    show();
    return 0;
}