#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    fcontour([](double x, double y) { return sin(x) + cos(y); });
    hold(on);
    fcontour([](double x, double y) { return x - y; });
    hold(off);

    show();
    return 0;
}