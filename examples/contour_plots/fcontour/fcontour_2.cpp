#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    fcontour([](double x, double y) { return erf(x) + cos(y); },
             {-5, 0, -5, +5}, iota(-2, 0.5, 2));
    hold(on);
    fcontour([](double x, double y) { return sin(x) + cos(y); }, {0, 5, -5, +5},
             iota(-2, 0.5, 2));
    hold(off);
    axis({-5, +5, -5, +5});
    grid(on);

    show();
    return 0;
}