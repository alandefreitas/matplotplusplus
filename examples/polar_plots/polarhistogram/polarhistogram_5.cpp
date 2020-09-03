#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> y = randp(100, 1.0, 0.5);
    polarhistogram(y, 25);

    show();
    return 0;
}