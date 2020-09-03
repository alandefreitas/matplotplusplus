#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fimplicit([](double x, double y) { return pow(x, 2) - pow(y, 2) - 1; });

    show();
    return 0;
}