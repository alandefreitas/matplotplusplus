#include <thread>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fcontour([](double x, double y) { return sin(x) + cos(y); })->levels({-1,0,+1});

    wait();
    return 0;
}