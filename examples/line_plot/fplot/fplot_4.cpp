#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto f = fplot([](double x) { return sin(x + pi / 5); });
    f->line_width(2);
    hold(on);
    fplot([](double x) { return sin(x - pi / 5); }, "--or");
    fplot([](double x) { return sin(x); }, "-.*c");
    hold(off);

    wait();
    return 0;
}