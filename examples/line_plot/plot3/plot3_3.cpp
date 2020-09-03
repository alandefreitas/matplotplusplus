#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> t = iota(0, pi / 500, pi);
    std::vector<std::vector<double>> X(3);
    std::vector<std::vector<double>> Y(3);

    X[0] = transform(t, [](auto x) { return sin(x) * cos(10 * x); });
    X[1] = transform(t, [](auto x) { return sin(x) * cos(12 * x); });
    X[2] = transform(t, [](auto x) { return sin(x) * cos(20 * x); });
    Y[0] = transform(t, [](auto x) { return sin(x) * sin(10 * x); });
    Y[1] = transform(t, [](auto x) { return sin(x) * sin(12 * x); });
    Y[2] = transform(t, [](auto x) { return sin(x) * sin(20 * x); });
    std::vector<double> z = transform(t, [](auto x) { return cos(x); });
    plot3(X, Y, z);

    show();
    return 0;
}