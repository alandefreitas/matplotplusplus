#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    fplot([](double t) { return cos(3 * t); }, [](double t) { return sin(2 * t); });

    wait();
    return 0;
}