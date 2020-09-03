#include <cmath>
#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;
    auto t = iota(0, pi / 500, 40 * pi);
    auto xt =
        transform(t, [](auto t) { return (3. + cos(sqrt(32.) * t)) * cos(t); });
    auto yt = transform(t, [](auto t) { return sin(sqrt(32.) * t); });
    auto zt =
        transform(t, [](auto t) { return (3. + cos(sqrt(32.) * t)) * sin(t); });
    plot3(xt, yt, zt);
    axis(equal);
    xlabel("x(t)");
    ylabel("y(t)");
    zlabel("z(t)");

    auto ax = gca();
    float da = ax->azimuth();
    for (size_t i = 0; i <= 360; ++i) {
        ax->azimuth(da + 2 * i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    show();
    return 0;
}