#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fplot([](double x) { return exp(x); }, std::array<double, 2>{-3, 0}, "b");
    hold(on);
    fplot([](double x) { return cos(x); }, std::array<double, 2>{0, 3}, "b");
    hold(off);
    grid(on);

    show();
    return 0;
}