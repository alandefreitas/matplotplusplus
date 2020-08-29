#include <thread>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f = [](double x, double y) {return pow(x,2) - pow(y,2); };
    fcontour(f,"--")->line_width(2);

    wait();
    return 0;
}