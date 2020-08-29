#include <matplot/matplot.h>

#include <cmath>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = {1980, 1990, 2000};
    std::vector<std::vector<double>> Y = {{15, 10, -10}, {20, -17, 5}, {-5, 21, 15}};
    barstacked(x, Y);

    wait();
    return 0;
}