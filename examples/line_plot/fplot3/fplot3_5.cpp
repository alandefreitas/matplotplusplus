#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto xt = [](double t) { return t; };
    auto yt = [](double t) { return t / 2.; };
    auto zt = [](double t) { return sin(6. * t); };
    fplot3(xt, yt, zt, std::array<double, 2>{-2 * pi, 2 * pi})
        ->mesh_density(300)
        .line_width(1);

    title("x=t, y=t/2, z=sin(6t) for -2π<t<2π");
    xlabel("x");
    ylabel("y");
    view(52.5, 30);
    box(on);
    xrange({-2 * pi, +2 * pi});
    yrange({-pi, +pi});

    auto ax = gca();
    ax->xticks(iota(-2 * pi, pi / 2, 2 * pi));
    ax->x_axis().ticklabels(
        {"-2π", "-3π/2", "-π", "-π/2", "0", "π/2", "π", "3π/2", "π"});
    ax->yticks(iota(-pi, pi / 2, pi));
    ax->y_axis().ticklabels({"-π", "-π/2", "0", "π/2", "π"});

    show();
    return 0;
}