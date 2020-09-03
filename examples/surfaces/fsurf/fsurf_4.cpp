#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto fn = [](double x, double y) { return y * sin(x) - x * cos(y); };
    fsurf(fn, {-2 * pi, 2 * pi});
    title("ysin(x) - xcos(y) for x and y in [-2π,2π]");
    xlabel("x");
    ylabel("y");
    zlabel("z");
    box(on);

    auto ax = gca();
    ax->xticks(iota(-2 * pi, pi / 2, 2 * pi));
    ax->xticklabels(
        {"-2π", "-3π/2", "-π", "-π/2", "0", "π/2", "π", "3π/2", "2π"});

    ax->yticks(iota(-2 * pi, pi / 2, 2 * pi));
    ax->yticklabels(
        {"-2π", "-3π/2", "-π", "-π/2", "0", "π/2", "π", "3π/2", "2π"});

    show();
    return 0;
}