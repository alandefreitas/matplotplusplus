#include <random>
#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> y = {10,21,33,52};
    bar(y);
    gca()->x_axis().ticklabels({"Small", "Medium", "Large", "Extra Large"});

    wait();
    return 0;
}