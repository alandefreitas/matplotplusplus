#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto t = iota(0, pi / 20, 10 * pi);
    auto xt1 = transform(t, [](auto t) { return sin(t); });
    auto yt1 = transform(t, [](auto t) { return cos(t); });
    auto xt2 = transform(t, [](auto t) { return sin(2 * t); });
    auto yt2 = transform(t, [](auto t) { return cos(2 * t); });
    plot3(xt1, yt1, t, xt2, yt2, t, "--");
    show();
    return 0;
}