#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> t = iota(0, pi / 500, pi);
    std::vector<double> xt1 =
        transform(t, [](auto x) { return sin(x) * cos(10 * x); });
    std::vector<double> yt1 =
        transform(t, [](auto x) { return sin(x) * sin(10 * x); });
    std::vector<double> zt1 = transform(t, [](auto x) { return cos(x); });
    std::vector<double> xt2 =
        transform(t, [](auto x) { return sin(x) * cos(12 * x); });
    std::vector<double> yt2 =
        transform(t, [](auto x) { return sin(x) * sin(12 * x); });
    std::vector<double> zt2 = transform(t, [](auto x) { return cos(x); });
    plot3(xt1, yt1, zt1, xt2, yt2, zt2);
    show();
    return 0;
}