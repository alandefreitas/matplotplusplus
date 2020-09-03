#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {75,  91,  105, 123.5, 131,  150,
                             179, 203, 226, 249,   281.5};
    bar(y)->face_color("r");

    show();
    return 0;
}