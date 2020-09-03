#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    fcontour([](double x, double y) {
        return sin(x) + cos(y);
    })->levels({-1, 0, +1});

    show();
    return 0;
}