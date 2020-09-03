#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fplot([](double t) { return cos(3 * t); },
          [](double t) { return sin(2 * t); });

    show();
    return 0;
}