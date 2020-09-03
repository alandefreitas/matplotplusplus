#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> Y = {
        {2, 2, 2, 2}, {2, 5, 8, 11}, {3, 6, 9, 12}};
    bar(Y);

    show();
    return 0;
}