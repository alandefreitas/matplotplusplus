#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> Y = {
        {0.0010, 0.0100, 0.1000, 1.0000, 10.0000},
        {0.0100, 0.1000, 1.0000, 10.0000, 100.0000},
        {0.1000, 1.0000, 10.0000, 100.0000, 1000.0000}};
    loglog(Y);

    show();
    return 0;
}