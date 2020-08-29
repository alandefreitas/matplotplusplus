#include <matplot/matplot.h>

#include <cmath>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {75, 91, 105, 123.5, 131, 150, 179, 203, 226, 249, 281.5};

    bar(y)->face_color({0, .5, .5}).edge_color({0, .9, .9}).line_width(1.5);

    wait();
    return 0;
}