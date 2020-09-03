#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = {1980, 1990, 2000};
    std::vector<std::vector<double>> Y = {
        {15, 10, -10}, {20, -17, 5}, {-5, 21, 15}};
    barstacked(x, Y);

    show();
    return 0;
}