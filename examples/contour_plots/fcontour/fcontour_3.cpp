#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto f = [](double x, double y) { return pow(x, 2) - pow(y, 2); };
    fcontour(f, "--")->line_width(2);

    show();
    return 0;
}