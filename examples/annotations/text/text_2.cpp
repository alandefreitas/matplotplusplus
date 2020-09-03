#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(-5, +5);
    std::vector<double> y =
        transform(x, [](auto x) { return pow(x, 3) - 12 * x; });
    plot(x, y);

    std::vector<double> xt = {-2, +2};
    std::vector<double> yt = {16, -16};
    std::string str = "dy/dx = 0";
    text(xt, yt, str);

    show();
    return 0;
}