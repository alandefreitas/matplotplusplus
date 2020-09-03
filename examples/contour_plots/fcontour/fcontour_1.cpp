#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f = [](double x, double y) { return sin(x) + cos(y); };
    fcontour(f);

    show();
    return 0;
}