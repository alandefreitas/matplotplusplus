#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto t = iota(0, pi / 20, 10 * pi);
    auto xt = transform(t, [](auto t) { return sin(t); });
    auto yt = transform(t, [](auto t) { return cos(t); });
    auto l = plot3(xt, yt, t, "-ob");
    l->marker_size(10);
    l->marker_face_color("#D9FFFF");
    xlabel("x(t)");
    ylabel("y(t)");
    zlabel("t");
    show();
    return 0;
}