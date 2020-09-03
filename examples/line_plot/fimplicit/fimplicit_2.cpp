#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fimplicit([](double x, double y) { return pow(x, 2) + pow(y, 2) - 3; },
              std::array<double, 4>{-3, 0, -2, 2});

    show();
    return 0;
}