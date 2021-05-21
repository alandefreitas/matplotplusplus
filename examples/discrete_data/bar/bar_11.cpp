#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {75,  91,  105, 123.5, 131,  150,
                             179, 203, 226, 249,   281.5};

    bar(y)->face_color({0.f, .5f, .5f}).edge_color({0.f, .9f, .9f}).line_width(1.5f);

    show();
    return 0;
}